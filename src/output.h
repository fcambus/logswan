/*
 * Logswan 2.1.1
 * Copyright (c) 2015-2019, Frederic Cambus
 * https://www.logswan.org
 *
 * Created:      2015-05-31
 * Last Updated: 2019-01-19
 *
 * Logswan is released under the BSD 2-Clause license.
 * See LICENSE file for details.
 */

#ifndef OUTPUT_H
#define OUTPUT_H

#include <sys/types.h>
#include <stdint.h>

#include "config.h"

struct results {
	char *fileName;
	off_t fileSize;
	uint64_t invalidLines;
	uint64_t processedLines;
	uint64_t bandwidth;
	uint64_t hits;
	uint64_t hitsIPv4;
	uint64_t hitsIPv6;
	uint64_t visits;
	uint64_t visitsIPv4;
	uint64_t visitsIPv6;
	uint64_t continents[CONTINENTS];
	uint64_t countries[COUNTRIES];
	uint64_t hours[24];
	uint64_t methods[METHODS];
	uint64_t protocols[PROTOCOLS];
	uint64_t status[STATUS_CODE_MAX];
	double runtime;
	char timeStamp[20];
};

char *output(struct results *);

#endif /* OUTPUT_H */
