/*****************************************************************************/
/*                                                                           */
/* Logswan (c) by Frederic Cambus 2015                                       */
/* https://github.com/fcambus/logswan                                        */
/*                                                                           */
/* Created:      2015/05/31                                                  */
/* Last Updated: 2015/06/21                                                  */
/*                                                                           */
/* Logswan is released under the BSD 3-Clause license.                       */
/* See LICENSE file for details.                                             */
/*                                                                           */
/*****************************************************************************/

#include <string.h>

#include "parse.h"

void parseLine(struct logLine* parsedLine, char *lineBuffer) {
	/* Remote host */
	parsedLine->remoteHost = strtok(lineBuffer, " ");

	/* User-identifier */
	strtok(NULL, " ");

	/* User ID */
	strtok(NULL, "[");

	/* Date */
	parsedLine->date = strtok(NULL, "]");

	/* Requested resource */
	strtok(NULL, "\"");
	parsedLine->resource = strtok(NULL, "\"");

	/* HTTP status codes */
	parsedLine->statusCode = strtok(NULL, " ");

	/* Returned object size */
	parsedLine->objectSize = strtok(NULL, "\"");
}
