/*****************************************************************************/
/*                                                                           */
/* Logswan 1.06                                                              */
/* Copyright (c) 2015-2017, Frederic Cambus                                  */
/* http://www.logswan.org                                                    */
/*                                                                           */
/* Created:      2015-05-31                                                  */
/* Last Updated: 2017-01-12                                                  */
/*                                                                           */
/* Logswan is released under the BSD 2-Clause license.                       */
/* See LICENSE file for details.                                             */
/*                                                                           */
/*****************************************************************************/

#define _XOPEN_SOURCE 600
#define _POSIX_SOURCE

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <err.h>
#include <getopt.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifndef HAVE_PLEDGE
#include "pledge.h"
#endif

#ifndef HAVE_STRTONUM
#include "strtonum.h"
#endif

#include "hll.h"

#include <GeoIP.h>

#include "config.h"
#include "output.h"
#include "parse.h"

bool geoip;
GeoIP *geoipv4 = NULL, *geoipv6 = NULL;

clock_t begin, end;

char lineBuffer[LINE_MAX_LENGTH];

struct results results;
struct date parsedDate;
struct logLine parsedLine;
struct request parsedRequest;

struct sockaddr_in ipv4;
struct sockaddr_in6 ipv6;
uint32_t isIPv4, isIPv6;

uint64_t bandwidth;
uint32_t statusCode;
uint32_t hour;
uint32_t countryId;

FILE *logFile;
struct stat logFileStat;

const char *errstr;

int8_t getoptFlag;

struct HLL uniqueIPv4, uniqueIPv6;
char *intputFile;

void
displayUsage() {
	printf("USAGE : logswan [options] inputfile\n\n" \
	       "Options are :\n\n" \
	       "	-g Enable GeoIP lookups\n" \
	       "	-h Display usage\n" \
	       "	-v Display version\n\n");
}

int
main(int argc, char *argv[]) {
	if (pledge("stdio rpath", NULL) == -1) {
		err(1, "pledge");
	}

	hll_init(&uniqueIPv4, HLL_BITS);
	hll_init(&uniqueIPv6, HLL_BITS);

	while ((getoptFlag = getopt(argc, argv, "ghv")) != -1) {
		switch(getoptFlag) {
		case 'g':
			geoip = true;
			break;

		case 'h':
			displayUsage();
			return 0;

		case 'v':
			printf("%s\n\n", VERSION);
			return 0;
		}
	}

	if (optind < argc) {
		intputFile = argv[optind];
	} else {
		displayUsage();
		return 0;
	}

	argc -= optind;
	argv += optind;

	/* Starting timer */
	begin = clock();

	/* Initializing GeoIP */
	if (geoip) {
		geoipv4 = GeoIP_open(GEOIPDIR "GeoIP.dat", GEOIP_MEMORY_CACHE);
		geoipv6 = GeoIP_open(GEOIPDIR "GeoIPv6.dat", GEOIP_MEMORY_CACHE);
	}

	/* Open log file */
	if (!strcmp(intputFile, "-")) {
		/* Read from standard input */
		logFile = stdin;
	} else {
		/* Attempt to read from file */
		if (!(logFile = fopen(intputFile, "r"))) {
			perror("Can't open log file");
			return 1;
		}
	}

	/* Get log file size */
	if (fstat(fileno(logFile), &logFileStat)) {
		perror("Can't stat log file");
		return 1;
	}

	results.fileName = intputFile;
	results.fileSize = (uint64_t)logFileStat.st_size;

	while (fgets(lineBuffer, LINE_MAX_LENGTH, logFile)) {
		/* Parse and tokenize line */
		parseLine(&parsedLine, lineBuffer);

		/* Detect if remote host is IPv4 or IPv6 */
		if (parsedLine.remoteHost) { /* Do not feed NULL tokens to inet_pton */
			isIPv4 = inet_pton(AF_INET, parsedLine.remoteHost, &(ipv4.sin_addr));
			isIPv6 = inet_pton(AF_INET6, parsedLine.remoteHost, &(ipv6.sin6_addr));
		} else {
			/* Invalid line */
			results.invalidLines++;
			continue;
		}

		if (isIPv4 || isIPv6) {
			if (isIPv4) {
				/* Increment hits counter */
				results.hitsIPv4++;

				/* Unique visitors */
				hll_add(&uniqueIPv4, parsedLine.remoteHost, strlen(parsedLine.remoteHost));

				if (geoipv4) {
					countryId = GeoIP_id_by_addr(geoipv4, parsedLine.remoteHost);
				}
			}

			if (isIPv6) {
				/* Increment hits counter */
				results.hitsIPv6++;

				/* Unique visitors */
				hll_add(&uniqueIPv6, parsedLine.remoteHost, strlen(parsedLine.remoteHost));

				if (geoipv6) {
					countryId = GeoIP_id_by_addr_v6(geoipv6, parsedLine.remoteHost);
				}
			}

			if (geoipv4 || geoipv6) {
				/* Increment countries array */
				results.countries[countryId]++;

				/* Increment continents array */
				for (size_t loop = 0; loop<CONTINENTS; loop++) {
					if (!strcmp(continentsId[loop], GeoIP_continent_by_id(countryId))) {
						results.continents[loop] ++;
						break;
					}
				}
			}

			/* Hourly distribution */
			if (parsedLine.date) {
				parseDate(&parsedDate, parsedLine.date);

				if (parsedDate.hour) {
					hour = strtonum(parsedDate.hour, 0, 23, &errstr);

					if (!errstr) {
						results.hours[hour] ++;
					}
				}
			}

			/* Parse request */
			if (parsedLine.request) {
				parseRequest(&parsedRequest, parsedLine.request);

				if (parsedRequest.method) {
					for (size_t loop = 0; loop<METHODS; loop++) {
						if (!strcmp(methodsNames[loop], parsedRequest.method)) {
							results.methods[loop] ++;
							break;
						}
					}
				}

				if (parsedRequest.protocol) {
					for (size_t loop = 0; loop<PROTOCOLS; loop++) {
						if (!strcmp(protocolsNames[loop], parsedRequest.protocol)) {
							results.protocols[loop] ++;
							break;
						}
					}
				}
			}

			/* Count HTTP status codes occurences */
			if (parsedLine.statusCode) {
				statusCode = strtonum(parsedLine.statusCode, 0, STATUS_CODE_MAX-1, &errstr);

				if (!errstr) {
					results.status[statusCode] ++;
				}
			}

			/* Increment bandwidth usage */
			if (parsedLine.objectSize) {
				bandwidth = strtonum(parsedLine.objectSize, 0, INT64_MAX, &errstr);

				if (!errstr) {
					results.bandwidth += bandwidth;
				}
			}
		} else {
			/* Invalid line */
			results.invalidLines++;
		}
	}

	/* Counting hits and processed lines */
	results.hits = results.hitsIPv4 + results.hitsIPv6;
	results.processedLines = results.hits + results.invalidLines;

	/* Counting unique visitors */
	results.visitsIPv4 = hll_count(&uniqueIPv4);
	results.visitsIPv6 = hll_count(&uniqueIPv6);
	results.visits = results.visitsIPv4 + results.visitsIPv6;

	/* Stopping timer */
	end = clock();
	results.runtime = (double)(end - begin) / CLOCKS_PER_SEC;

	/* Generate timestamp */
	time_t now = time(NULL);
	strftime(results.timeStamp, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));

	/* Printing results */
	fprintf(stderr, "Processed %" PRIu64 " lines in %f seconds\n", results.processedLines, results.runtime);
	fputs(output(&results), stdout);

	/* Clean up */
	fclose(logFile);

	GeoIP_delete(geoipv4);
	GeoIP_delete(geoipv6);

	hll_destroy(&uniqueIPv4);
	hll_destroy(&uniqueIPv6);

	return 0;
}
