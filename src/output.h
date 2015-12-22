/*****************************************************************************/
/*                                                                           */
/* Logswan 1.02 (c) by Frederic Cambus 2015                                  */
/* https://github.com/fcambus/logswan                                        */
/*                                                                           */
/* Created:      2015/05/31                                                  */
/* Last Updated: 2015/12/22                                                  */
/*                                                                           */
/* Logswan is released under the BSD 3-Clause license.                       */
/* See LICENSE file for details.                                             */
/*                                                                           */
/*****************************************************************************/

#ifndef OUTPUT_H
#define OUTPUT_H

struct results {
	char *fileName;
	uint64_t fileSize;
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
	uint64_t status[STATUS_CODE_MAX];
	double runtime;
	char timeStamp[20];
	uint64_t protocols[PROTOCOLS];
	uint64_t methods[METHODS];
};

typedef struct results Results;

char *output(Results);

#endif /* OUTPUT_H */
