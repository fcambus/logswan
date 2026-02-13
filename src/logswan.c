/*
 * Logswan 2.1.15
 * Copyright (c) 2015-2026, Frederic Cambus
 * https://www.logswan.org
 *
 * Created:      2015-05-31
 * Last Updated: 2023-03-13
 *
 * Logswan is released under the BSD 2-Clause license.
 * See LICENSE file for details.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <err.h>
#include <getopt.h>
#include <inttypes.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#ifdef HAVE_SECCOMP
#include <sys/prctl.h>
#include <linux/seccomp.h>
#include "seccomp.h"
#endif

#include <maxminddb.h>

#include "compat.h"
#include "config.h"
#include "continents.h"
#include "countries.h"
#include "hll.h"
#include "output.h"
#include "parse.h"

static void
usage(void)
{
	printf("logswan [-ghv] [-d db] logfile\n\n"
	    "The options are as follows:\n\n"
	    "	-d db	Specify path to a GeoIP database.\n"
	    "	-g	Enable GeoIP lookups.\n"
	    "	-h	Display usage.\n"
	    "	-v	Display version.\n");
}

int
main(int argc, char *argv[])
{
	struct timespec begin, end, elapsed;
	struct HLL unique_ipv4, unique_ipv6;
	struct results results;
	struct date parsed_date;
	struct logline parsed_line;
	struct request parsed_request;
	struct stat logfile_stat;

	struct sockaddr_in ipv4;
	struct sockaddr_in6 ipv6;

	uint64_t bandwidth;
	uint32_t status_code;
	uint32_t hour;
	int gai_error, mmdb_error;
	int opt;

	const char *errstr;
	char *linebuffer = NULL;
	size_t linesize = 0;
	char *input;
	char *db = NULL;

	bool geoip = false;
	bool is_ipv4, is_ipv6;

	MMDB_s geoip2;
	MMDB_lookup_result_s lookup;

	FILE *logfile;

	if (pledge("stdio rpath", NULL) == -1) {
		err(EXIT_FAILURE, "pledge");
	}

#ifdef HAVE_SECCOMP
	if (prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0)) {
		perror("Can't initialize seccomp");
		return EXIT_FAILURE;
	}

	if (prctl(PR_SET_SECCOMP, SECCOMP_MODE_FILTER, &logswan)) {
		perror("Can't load seccomp filter");
		return EXIT_FAILURE;
	}
#endif

	while ((opt = getopt(argc, argv, "d:ghv")) != -1) {
		switch (opt) {
		case 'd':
			db = optarg;
			break;

		case 'g':
			geoip = true;
			break;

		case 'h':
			usage();
			return EXIT_SUCCESS;

		case 'v':
			printf("%s\n", VERSION);
			return EXIT_SUCCESS;
		}
	}

	if (optind < argc) {
		input = argv[optind];
	} else {
		usage();
		return EXIT_SUCCESS;
	}

	hll_init(&unique_ipv4, HLL_BITS);
	hll_init(&unique_ipv6, HLL_BITS);

	/* Starting timer */
	clock_gettime(CLOCK_MONOTONIC, &begin);

	/* Initializing GeoIP */
	if (geoip) {
		if (!db)
			db = GEOIP2DIR GEOIP2DB;

		if (MMDB_open(db, MMDB_MODE_MMAP, &geoip2) != MMDB_SUCCESS)
			err(EXIT_FAILURE, "Can't open database (%s)", db);
	}

	/* Open log file */
	if (!strcmp(input, "-")) {
		/* Read from standard input */
		logfile = stdin;
	} else {
		/* Attempt to read from file */
		if (!(logfile = fopen(input, "r"))) {
			perror("Can't open log file");
			return EXIT_FAILURE;
		}
	}

	/* Get log file size */
	if (fstat(fileno(logfile), &logfile_stat)) {
		perror("Can't stat log file");
		return EXIT_FAILURE;
	}

	memset(&results, 0, sizeof(struct results));
	results.file_name = input;
	results.file_size = logfile_stat.st_size;

	while (getline(&linebuffer, &linesize, logfile) != -1) {
		/* Parse and tokenize line */
		parse_line(&parsed_line, linebuffer);

		/* Detect if remote host is IPv4 or IPv6 */
		if (parsed_line.remote_host) { /* Do not feed NULL tokens to inet_pton */
			if ((is_ipv4 = inet_pton(AF_INET, parsed_line.remote_host, &ipv4.sin_addr))) {
				is_ipv6 = false;
			} else {
				is_ipv6 = inet_pton(AF_INET6, parsed_line.remote_host, &ipv6.sin6_addr);

				if (!is_ipv6) {
					results.invalid_lines++;
					continue;
				}
			}
		} else {
			/* Invalid line */
			results.invalid_lines++;
			continue;
		}

		if (is_ipv4) {
			/* Increment hits counter */
			results.hits_ipv4++;

			/* Unique visitors */
			hll_add(&unique_ipv4, parsed_line.remote_host, strlen(parsed_line.remote_host));
		}

		if (is_ipv6) {
			/* Increment hits counter */
			results.hits_ipv6++;

			/* Unique visitors */
			hll_add(&unique_ipv6, parsed_line.remote_host, strlen(parsed_line.remote_host));
		}

		if (geoip) {
			MMDB_entry_data_s entry_data;
			memset(&entry_data, 0, sizeof(MMDB_entry_data_s));

			lookup = MMDB_lookup_string(&geoip2, parsed_line.remote_host, &gai_error, &mmdb_error);

			MMDB_get_value(&lookup.entry, &entry_data, "country", "iso_code", NULL);

			if (entry_data.has_data) {
				/* Increment countries array */
				for (size_t loop = 0; loop < COUNTRIES; loop++) {
					if (!strncmp(countries_id[loop], entry_data.utf8_string, 2)) {
						results.countries[loop]++;
						break;
					}
				}
			}

			MMDB_get_value(&lookup.entry, &entry_data, "continent", "code", NULL);

			if (entry_data.has_data) {
				/* Increment continents array */
				for (size_t loop = 0; loop < CONTINENTS; loop++) {
					if (!strncmp(continents_id[loop], entry_data.utf8_string, 2)) {
						results.continents[loop]++;
						break;
					}
				}
			}
		}

		/* Hourly distribution */
		if (parsed_line.date) {
			parse_date(&parsed_date, parsed_line.date);

			if (parsed_date.hour) {
				hour = strtonum(parsed_date.hour, 0, 23, &errstr);

				if (!errstr) {
					results.hours[hour]++;
				}
			}
		}

		/* Parse request */
		if (parsed_line.request) {
			parse_request(&parsed_request, parsed_line.request);

			if (parsed_request.method) {
				for (size_t loop = 0; loop < METHODS; loop++) {
					if (!strcmp(methods_names[loop], parsed_request.method)) {
						results.methods[loop]++;
						break;
					}
				}
			}

			if (parsed_request.protocol) {
				for (size_t loop = 0; loop < PROTOCOLS; loop++) {
					if (!strcmp(protocols_names[loop], parsed_request.protocol)) {
						results.protocols[loop]++;
						break;
					}
				}
			}
		}

		/* Count HTTP status codes occurrences */
		if (parsed_line.status_code) {
			status_code = strtonum(parsed_line.status_code, 0, STATUS_CODE_MAX-1, &errstr);

			if (!errstr) {
				results.status[status_code]++;
			}
		}

		/* Increment bandwidth usage */
		if (parsed_line.object_size) {
			bandwidth = strtonum(parsed_line.object_size, 0, INT64_MAX, &errstr);

			if (!errstr) {
				results.bandwidth += bandwidth;
			}
		}
	}

	/* Counting hits and processed lines */
	results.hits = results.hits_ipv4 + results.hits_ipv6;
	results.processed_lines = results.hits + results.invalid_lines;

	/* Counting unique visitors */
	results.visits_ipv4 = hll_count(&unique_ipv4);
	results.visits_ipv6 = hll_count(&unique_ipv6);
	results.visits = results.visits_ipv4 + results.visits_ipv6;

	/* Stopping timer */
	clock_gettime(CLOCK_MONOTONIC, &end);

	timespecsub(&end, &begin, &elapsed);
	results.runtime = elapsed.tv_sec + elapsed.tv_nsec / 1E9;

	/* Generate timestamp */
	time_t now = time(NULL);
	strftime(results.timestamp, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));

	/* Printing results */
	fprintf(stdout, "%s\n", output(&results));
	fprintf(stderr, "Processed %" PRIu64 " lines in %f seconds.\n", results.processed_lines, results.runtime);

	/* Clean up */
	free(linebuffer);
	fclose(logfile);

	if (geoip)
		MMDB_close(&geoip2);

	hll_destroy(&unique_ipv4);
	hll_destroy(&unique_ipv6);

	return EXIT_SUCCESS;
}
