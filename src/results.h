/*****************************************************************************/
/*                                                                           */
/* Logswan (c) by Frederic Cambus 2015                                       */
/* https://github.com/fcambus/logswan                                        */
/*                                                                           */
/* Created:      2015/05/31                                                  */
/* Last Updated: 2015/06/24                                                  */
/*                                                                           */
/* Logswan is released under the BSD 3-Clause license.                       */
/* See LICENSE file for details.                                             */
/*                                                                           */
/*****************************************************************************/

#ifndef RESULTS_H
#define RESULTS_H

#define STATUS_CODE_MAX 512

struct results {
	uint64_t fileSize;
	uint64_t invalidLines;
	uint64_t processedLines;
	uint64_t bandwidth;
	uint64_t hits;
	uint64_t hitsIPv4;
	uint64_t hitsIPv6;
	uint64_t countries[255];
	int hours[24];
	int httpStatus[STATUS_CODE_MAX];
	double runtime;
	char timeStamp[20];
	int protocols[2];
};

typedef struct results Results;

#endif
