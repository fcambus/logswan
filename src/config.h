/*****************************************************************************/
/*                                                                           */
/* Logswan 1.03                                                              */
/* Copyright (c) 2015-2016, Frederic Cambus                                  */
/* http://www.logswan.org                                                    */
/*                                                                           */
/* Created:      2015-05-31                                                  */
/* Last Updated: 2016-01-01                                                  */
/*                                                                           */
/* Logswan is released under the BSD 3-Clause license.                       */
/* See LICENSE file for details.                                             */
/*                                                                           */
/*****************************************************************************/

#ifndef CONFIG_H
#define CONFIG_H

#define VERSION "Logswan 1.03"

enum {
	HLL_BITS = 20,
	LINE_MAX_LENGTH = 65536,
	STATUS_CODE_MAX = 512,

	CONTINENTS = 6,
	COUNTRIES = 512,
	METHODS = 9,
	PROTOCOLS = 2
};

extern char *continentsId[];
extern char *continentsNames[];
extern char *methods[];
extern char *protocols[];

#endif /* CONFIG_H */
