/*****************************************************************************/
/*                                                                           */
/* Logswan (c) by Frederic Cambus 2015                                       */
/* https://github.com/fcambus/logswan                                        */
/*                                                                           */
/* Created:      2015/05/31                                                  */
/* Last Updated: 2015/06/22                                                  */
/*                                                                           */
/* Logswan is released under the BSD 3-Clause license.                       */
/* See LICENSE file for details.                                             */
/*                                                                           */
/*****************************************************************************/

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <getopt.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <GeoIP.h>
#include <jansson.h>

#include "parse.h"

#define VERSION "Logswan"
#define LINE_MAX_LENGTH 4096

GeoIP *geoip;

clock_t begin, end;
double runtime;
char timeStamp[20];

char lineBuffer[LINE_MAX_LENGTH];

uint64_t invalidLines, processedLines = 0;
uint64_t objectSize = 0;
uint64_t bandwidth = 0;
uint64_t hits = 0;
uint64_t hitsIPv4 = 0;
uint64_t hitsIPv6 = 0;
uint64_t countries[255];

struct date parsedDate;
struct logLine parsedLine;

struct sockaddr_in ipv4;
struct sockaddr_in6 ipv6;

int isIPv4, isIPv6;

int httpStatus[512];
int statusCode;

int hours[24];
int hour;

struct stat logFileSize;
FILE *logFile;

char *endptr;
int getoptFlag;

int main (int argc, char *argv[]) {
	printf("-------------------------------------------------------------------------------\n" \
	       "                      Logswan (c) by Frederic Cambus 2015                      \n" \
	       "-------------------------------------------------------------------------------\n\n");

	if (argc != 2) {
		printf("ERROR : No input file specified.\n");
		return EXIT_FAILURE;
	}

	while ((getoptFlag = getopt(argc, argv, "v")) != -1) {
		switch(getoptFlag) {
		case 'v':
			printf(VERSION);
			return 0;
		}
	}

	/* Starting timer */
	begin = clock();

	/* Initializing GeoIP */
	geoip = GeoIP_open("GeoIP.dat", GEOIP_MEMORY_CACHE);

	/* Get log file size */
	stat(argv[1], &logFileSize);

	printf("Processing file : %s\n\n", argv[1]);

	logFile = fopen(argv[1], "r");
	if(!logFile) {
		perror("Can't open log file");
		return 1;
	}

	while (fgets(lineBuffer, LINE_MAX_LENGTH, logFile) != NULL) {
		/* Parse and tokenize line */
		parseLine(&parsedLine, lineBuffer);

		/* Detect if remote host is IPv4 or IPv6 */
		if (parsedLine.remoteHost) { /* Do not feed NULL tokens to inet_pton */
			isIPv4 = inet_pton(AF_INET, parsedLine.remoteHost, &(ipv4.sin_addr));
			isIPv6 = inet_pton(AF_INET6, parsedLine.remoteHost, &(ipv6.sin6_addr));
		}

		if (isIPv4 || isIPv6) {
			/* Increment countries array */
			if (geoip && isIPv4) {
				countries[GeoIP_id_by_addr(geoip, parsedLine.remoteHost)]++;
			}

			/* Hourly distribution */
			parseDate(&parsedDate, parsedLine.date);

			if (parsedDate.hour) { /* Do not feed NULL tokens to atoi */
				hour = atoi(parsedDate.hour);

				if (hour < 24) {
					hours[hour] ++;
				}
			}

			/* Count HTTP status codes occurences */
			if (parsedLine.statusCode) { /* Do not feed NULL tokens to strtol */
				statusCode = strtol(parsedLine.statusCode, &endptr, 10);

				if (statusCode < 512) {
					httpStatus[statusCode] ++;
				}
			}

			/* Increment bandwidth usage */
			if (parsedLine.objectSize) { /* Do not feed NULL tokens to strtol */
				bandwidth += strtol(parsedLine.objectSize, &endptr, 10);
			}

			/* Increment hits counter */
			hitsIPv4 += isIPv4;
			hitsIPv6 += isIPv6;
			hits++;
		} else {
			/* Invalid line */

			invalidLines++;
		}

		/* Increment processed lines counter */
		processedLines++;
	}

	/* Stopping timer */
	end = clock();
	runtime = (double)(end - begin) / CLOCKS_PER_SEC;

	/* Generate timestamp */
	time_t now = time(0);
	strftime(timeStamp, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));

	/* Printing results */
	printf("Processed %" PRIu64 " lines in %f seconds\n", processedLines, runtime);
	fclose(logFile);

	json_t *jsonObject = json_object();
	json_t *hitsObject = json_object();
	json_t *countriesObject = json_object();

	for (int loop=0; loop<255; loop++) {
		if (countries[loop] != 0) {
			json_object_set_new(countriesObject, GeoIP_code_by_id(loop), json_integer(countries[loop]));
		}
	}

	json_object_set_new(hitsObject, "ipv4", json_integer(hitsIPv4));
	json_object_set_new(hitsObject, "ipv6", json_integer(hitsIPv6));
	json_object_set_new(hitsObject, "total", json_integer(hits));
	json_object_set_new(hitsObject, "countries", countriesObject);

	json_object_set_new(jsonObject, "date", json_string(timeStamp));
	json_object_set_new(jsonObject, "file_size", json_integer((uint64_t)logFileSize.st_size));
	json_object_set_new(jsonObject, "processed_lines", json_integer(processedLines));
	json_object_set_new(jsonObject, "invalid_lines", json_integer(invalidLines));
	json_object_set_new(jsonObject, "bandwidth", json_integer(bandwidth));
	json_object_set_new(jsonObject, "runtime", json_real(runtime));
	json_object_set_new(jsonObject, "hits", hitsObject);

	printf("%s", json_dumps(jsonObject, JSON_INDENT(3) | JSON_PRESERVE_ORDER));

	json_decref(jsonObject);
	json_decref(hitsObject);
	json_decref(countriesObject);

	return EXIT_SUCCESS;
}
