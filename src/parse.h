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
	char *request;
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
