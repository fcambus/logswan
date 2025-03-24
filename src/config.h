/*
 * Logswan 2.1.15
 * Copyright (c) 2015-2025, Frederic Cambus
 * https://www.logswan.org
 *
 * Created:      2015-05-31
 * Last Updated: 2021-11-16
 *
 * Logswan is released under the BSD 2-Clause license.
 * See LICENSE file for details.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef CONFIG_H
#define CONFIG_H

#define VERSION "Logswan 2.1.15"

enum {
	HLL_BITS = 20,
	STATUS_CODE_MAX = 512,

	CONTINENTS = 7,
	COUNTRIES = 250,
	METHODS = 9,
	PROTOCOLS = 4
};

extern char *methods_names[];
extern char *protocols_names[];

#endif /* CONFIG_H */
