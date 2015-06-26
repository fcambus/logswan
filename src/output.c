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

#include <GeoIP.h>
#include <jansson.h>

#include "results.h"

void output(Results results) {
	json_t *jsonObject = json_object();
	json_t *hitsObject = json_object();
	json_t *countriesObject = json_object();
	json_t *hoursObject = json_object();

	for (int loop=0; loop<255; loop++) {
		if (results.countries[loop] != 0) {
			json_object_set_new(countriesObject, GeoIP_code_by_id(loop), json_integer(results.countries[loop]));
		}
	}

	char *hoursString[] = {
		"00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23"
	};

	for (int loop=0; loop<24; loop++) {
		if (results.hours[loop] != 0) {
			json_object_set_new(hoursObject, hoursString[loop], json_integer(results.hours[loop]));
		}
	}

	json_object_set_new(hitsObject, "ipv4", json_integer(results.hitsIPv4));
	json_object_set_new(hitsObject, "ipv6", json_integer(results.hitsIPv6));
	json_object_set_new(hitsObject, "total", json_integer(results.hits));
	json_object_set_new(hitsObject, "countries", countriesObject);
	json_object_set_new(hitsObject, "hours", hoursObject);

	json_object_set_new(jsonObject, "date", json_string(results.timeStamp));
	json_object_set_new(jsonObject, "file_size", json_integer(results.fileSize));
	json_object_set_new(jsonObject, "processed_lines", json_integer(results.processedLines));
	json_object_set_new(jsonObject, "invalid_lines", json_integer(results.invalidLines));
	json_object_set_new(jsonObject, "bandwidth", json_integer(results.bandwidth));
	json_object_set_new(jsonObject, "runtime", json_real(results.runtime));
	json_object_set_new(jsonObject, "hits", hitsObject);

	printf("%s", json_dumps(jsonObject, JSON_INDENT(3) | JSON_PRESERVE_ORDER));
}
