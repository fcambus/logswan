/*****************************************************************************/
/*                                                                           */
/* Logswan 2.0.2                                                             */
/* Copyright (c) 2015-2018, Frederic Cambus                                  */
/* https://www.logswan.org                                                   */
/*                                                                           */
/* Created:      2015-05-31                                                  */
/* Last Updated: 2018-08-05                                                  */
/*                                                                           */
/* Logswan is released under the BSD 2-Clause license.                       */
/* See LICENSE file for details.                                             */
/*                                                                           */
/*****************************************************************************/

#ifndef CONFIG_H
#define CONFIG_H

#define VERSION "Logswan 2.0.2"

enum {
	HLL_BITS = 20,
	LINE_LENGTH_MAX = 65536,
	STATUS_CODE_MAX = 512,

	CONTINENTS = 7,
	COUNTRIES = 251,
	METHODS = 9,
	PROTOCOLS = 3
};

extern char *continentsId[];
extern char *continentsNames[];
extern char *countriesId[];
extern char *countriesNames[];
extern char *methodsNames[];
extern char *protocolsNames[];

#endif /* CONFIG_H */
