/*****************************************************************************/
/*                                                                           */
/* Logswan 1.05                                                              */
/* Copyright (c) 2015-2016, Frederic Cambus                                  */
/* http://www.logswan.org                                                    */
/*                                                                           */
/* Created:      2015-05-31                                                  */
/* Last Updated: 2016-07-18                                                  */
/*                                                                           */
/* Logswan is released under the BSD 2-Clause license.                       */
/* See LICENSE file for details.                                             */
/*                                                                           */
/*****************************************************************************/

#ifndef CONFIG_H
#define CONFIG_H

#define VERSION "Logswan 1.05"

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

extern char *months[];

#endif /* CONFIG_H */
