/*
 * Logswan 2.1.11
 * Copyright (c) 2015-2021, Frederic Cambus
 * https://www.logswan.org
 *
 * Created:      2015-05-31
 * Last Updated: 2021-02-15
 *
 * Logswan is released under the BSD 2-Clause license.
 * See LICENSE file for details.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef OUTPUT_H
#define OUTPUT_H

#include <sys/types.h>
#include <stdint.h>

#include "config.h"

struct results {
	char *file_name;
	off_t file_size;
	uint64_t invalid_lines;
	uint64_t processed_lines;
	uint64_t bandwidth;
	uint64_t hits;
	uint64_t hits_ipv4;
	uint64_t hits_ipv6;
	uint64_t visits;
	uint64_t visits_ipv4;
	uint64_t visits_ipv6;
	uint64_t continents[CONTINENTS];
	uint64_t countries[COUNTRIES];
	uint64_t hours[24];
	uint64_t methods[METHODS];
	uint64_t protocols[PROTOCOLS];
	uint64_t status[STATUS_CODE_MAX];
	double runtime;
	char timestamp[20];
};

char *output(struct results *);

#endif /* OUTPUT_H */
