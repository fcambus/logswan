/*****************************************************************************/
/*                                                                           */
/* Logswan 1.02 (c) by Frederic Cambus 2015                                  */
/* https://github.com/fcambus/logswan                                        */
/*                                                                           */
/* Created:      2015/05/31                                                  */
/* Last Updated: 2015/12/15                                                  */
/*                                                                           */
/* Logswan is released under the BSD 3-Clause license.                       */
/* See LICENSE file for details.                                             */
/*                                                                           */
/*****************************************************************************/

#ifndef LOGSWAN_H
#define LOGSWAN_H

#define VERSION "Logswan 1.02"

enum {
	HLL_BITS = 20,
	LINE_MAX_LENGTH = 65536,
	STATUS_CODE_MAX = 512,

	METHODS = 9,
	PROTOCOLS = 2	
};

#endif /* LOGSWAN_H */
