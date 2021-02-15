/*
 * Logswan 2.1.8
 * Copyright (c) 2015-2021, Frederic Cambus
 * https://www.logswan.org
 *
 * Created:      2015-05-31
 * Last Updated: 2021-02-15
 *
 * Logswan is released under the BSD 2-Clause license.
 * See LICENSE file for details.
 */

#include <string.h>

#include "parse.h"

void
parse_date(struct date *parsed_date, char *date)
{
	parsed_date->day = strtok(date, "/");
	parsed_date->month = strtok(NULL, "/");
	parsed_date->year = strtok(NULL, ":");
	parsed_date->hour = strtok(NULL, ":");
	parsed_date->minute = strtok(NULL, ":");
	parsed_date->second = strtok(NULL, " ");
}

void
parse_line(struct logline *parsed_line, char *linebuffer)
{
	if (*linebuffer) {
		/* Remote host */
		parsed_line->remote_host = strtok(linebuffer, " ");

		/* User-identifier */
		strtok(NULL, " ");

		/* User ID */
		strtok(NULL, "[");

		/* Date */
		parsed_line->date = strtok(NULL, "]");

		/* Requested resource */
		strtok(NULL, "\"");
		parsed_line->request = strtok(NULL, "\"");

		/* HTTP status codes */
		parsed_line->status_code = strtok(NULL, " ");

		/* Returned object size */
		parsed_line->object_size = strtok(NULL, " \"");
	}
}

void
parse_request(struct request *parsed_request, char *request)
{
	char *pch = strrchr(request, ' ');

	if (pch) {
		parsed_request->protocol = pch + 1;
		parsed_request->method = strtok(request, " ");
	}
}
