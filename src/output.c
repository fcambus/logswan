/*
 * Logswan 2.1.8
 * Copyright (c) 2015-2021, Frederic Cambus
 * https://www.logswan.org
 *
 * Created:      2015-05-31
 * Last Updated: 2019-01-19
 *
 * Logswan is released under the BSD 2-Clause license.
 * See LICENSE file for details.
 */

#include <stddef.h>
#include <stdint.h>

#include <jansson.h>

#include "config.h"
#include "continents.h"
#include "countries.h"
#include "output.h"

char
*output(struct results *results)
{
	json_t *output = json_object();
	json_t *hits = json_object();
	json_t *visits = json_object();
	json_t *continents = json_array();
	json_t *countries = json_array();
	json_t *hours = json_array();
	json_t *status = json_array();
	json_t *methods = json_array();
	json_t *protocols = json_array();

	for (size_t loop = 0; loop < CONTINENTS; loop++) {
		if (results->continents[loop]) {
			json_array_append_new(
			    continents,
			    json_pack("{s:s, s:s, s:i}",
			    "data", continentsId[loop],
			    "name", continentsNames[loop],
			    "hits", results->continents[loop]));
		}
	}

	for (size_t loop = 0; loop < COUNTRIES; loop++) {
		if (results->countries[loop]) {
			json_array_append_new(
			    countries,
			    json_pack("{s:s, s:s, s:i}",
			    "data", countriesId[loop],
			    "name", countriesNames[loop],
			    "hits", results->countries[loop]));
		}
	}

	for (size_t loop = 0; loop < 24; loop++) {
		if (results->hours[loop]) {
			json_array_append_new(
			    hours,
			    json_pack("{s:i, s:i}",
			    "data", loop,
			    "hits", results->hours[loop]));
		}
	}

	for (size_t loop = 0; loop < STATUS_CODE_MAX; loop++) {
		if (results->status[loop]) {
			json_array_append_new(
			    status,
			    json_pack("{s:i, s:i}",
			    "data", loop,
			    "hits", results->status[loop]));
		}
	}

	for (size_t loop = 0; loop < METHODS; loop++) {
		if (results->methods[loop]) {
			json_array_append_new(
			    methods,
			    json_pack("{s:s, s:i}",
			    "data", methodsNames[loop],
			    "hits", results->methods[loop]));
		}
	}

	for (size_t loop = 0; loop < PROTOCOLS; loop++) {
		if (results->protocols[loop]) {
			json_array_append_new(
			    protocols,
			    json_pack("{s:s, s:i}",
			    "data", protocolsNames[loop],
			    "hits", results->protocols[loop]));
		}
	}

	json_object_set_new(hits, "ipv4", json_integer(results->hitsIPv4));
	json_object_set_new(hits, "ipv6", json_integer(results->hitsIPv6));
	json_object_set_new(hits, "total", json_integer(results->hits));

	json_object_set_new(visits, "ipv4", json_integer(results->visitsIPv4));
	json_object_set_new(visits, "ipv6", json_integer(results->visitsIPv6));
	json_object_set_new(visits, "total", json_integer(results->visits));

	json_object_set_new(output, "date", json_string(results->timeStamp));
	json_object_set_new(output, "generator", json_string(VERSION));
	json_object_set_new(output, "file_name", json_string(results->fileName));
	json_object_set_new(output, "file_size", json_integer(results->fileSize));
	json_object_set_new(output, "processed_lines", json_integer(results->processedLines));
	json_object_set_new(output, "invalid_lines", json_integer(results->invalidLines));
	json_object_set_new(output, "bandwidth", json_integer(results->bandwidth));
	json_object_set_new(output, "runtime", json_real(results->runtime));
	json_object_set_new(output, "hits", hits);
	json_object_set_new(output, "visits", visits);
	json_object_set_new(output, "continents", continents);
	json_object_set_new(output, "countries", countries);
	json_object_set_new(output, "hours", hours);
	json_object_set_new(output, "methods", methods);
	json_object_set_new(output, "protocols", protocols);
	json_object_set_new(output, "status", status);

	return json_dumps(output, JSON_INDENT(3) | JSON_PRESERVE_ORDER);
}
