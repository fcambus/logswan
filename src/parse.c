/*****************************************************************************/
/*                                                                           */
/* Logswan 1.06                                                              */
/* Copyright (c) 2015-2016, Frederic Cambus                                  */
/* http://www.logswan.org                                                    */
/*                                                                           */
/* Created:      2015-05-31                                                  */
/* Last Updated: 2016-12-17                                                  */
/*                                                                           */
/* Logswan is released under the BSD 2-Clause license.                       */
/* See LICENSE file for details.                                             */
/*                                                                           */
/*****************************************************************************/

#define _POSIX_C_SOURCE 200112L

#include <string.h>

#include "parse.h"

void parseDate(struct date* parsedDate, char *date) {
	char *last;

	parsedDate->day = strtok_r(date, "/", &last);
	parsedDate->month = strtok_r(NULL, "/", &last);
	parsedDate->year = strtok_r(NULL, ":", &last);
	parsedDate->hour = strtok_r(NULL, ":", &last);
	parsedDate->minute = strtok_r(NULL, ":", &last);
	parsedDate->second = strtok_r(NULL, " ", &last);
}

void parseLine(struct logLine* parsedLine, char *lineBuffer) {
	char *last;

	if (*lineBuffer) {
		/* Remote host */
		parsedLine->remoteHost = strtok_r(lineBuffer, " ", &last);

		/* User-identifier */
		strtok_r(NULL, " ", &last);

		/* User ID */
		strtok_r(NULL, "[", &last);

		/* Date */
		parsedLine->date = strtok_r(NULL, "]", &last);

		/* Requested resource */
		strtok_r(NULL, "\"", &last);
		parsedLine->request = strtok_r(NULL, "\"", &last);

		/* HTTP status codes */
		parsedLine->statusCode = strtok_r(NULL, " ", &last);

		/* Returned object size */
		parsedLine->objectSize = strtok_r(NULL, " \"", &last);
	}
}

void parseRequest(struct request* parsedRequest, char *request) {
	char *last;
	char *pch = strrchr(request, ' ');

	if (pch) {
		parsedRequest->protocol = pch + 1;
		parsedRequest->method = strtok_r(request, " ", &last);
	}
}
