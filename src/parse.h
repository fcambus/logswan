/*****************************************************************************/
/*                                                                           */
/* Logswan (c) by Frederic Cambus 2015                                       */
/* https://github.com/fcambus/logswan                                        */
/*                                                                           */
/* Created:      2015/05/31                                                  */
/* Last Updated: 2015/07/19                                                  */
/*                                                                           */
/* Logswan is released under the BSD 3-Clause license.                       */
/* See LICENSE file for details.                                             */
/*                                                                           */
/*****************************************************************************/

#ifndef PARSE_H
#define PARSE_H

struct date {
	char *day;
	char *month;
	char *year;
	char *hour;
	char *minute;
	char *second;
};

struct logLine {
	char *remoteHost;
	char *date;
	char *resource;
	char *statusCode;
	char *objectSize;
};

struct request {
	char *method;
	char *resource;
	char *protocol;
};

void parseDate(struct date*, char *);
void parseLine(struct logLine*, char *);
void parseRequest(struct request*, char *);

#endif /* PARSE_H */
