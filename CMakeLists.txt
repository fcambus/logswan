###############################################################################
#                                                                             #
# Logswan 1.03                                                                #
# Copyright (c) 2015-2016, Frederic Cambus                                    #
# http://www.logswan.org                                                      #
#                                                                             #
# Created:      2015-05-31                                                    #
# Last Updated: 2016-01-01                                                    #
#                                                                             #
# Logswan is released under the BSD 3-Clause license.                         #
# See LICENSE file for details.                                               #
#                                                                             #
###############################################################################

cmake_minimum_required (VERSION 2.6)

project (logswan C)

include(CheckFunctionExists)
include_directories(/usr/local/include /usr/pkg/include)

check_function_exists(strtonum HAVE_STRTONUM)

find_library(LIB_GEOIP NAMES GeoIP REQUIRED)
find_library(LIB_JANSSON NAMES jansson REQUIRED)

set (DEPS deps/hll/hll.c deps/MurmurHash3/MurmurHash3.c)
set (SRC src/logswan.c src/config.c src/output.c src/parse.c)

if(NOT HAVE_STRTONUM)
  set (SRC ${SRC} compat/strtonum.c)
endif()

if(NOT DEFINED DATADIR)
  set(DATADIR ${CMAKE_INSTALL_PREFIX}/share/GeoIP)
endif()

add_definitions(-Wall -Wextra -Werror -std=c99 -pedantic)
add_definitions(-DDATADIR="${DATADIR}")
add_executable(logswan ${SRC} ${DEPS})

target_link_libraries(logswan ${LIB_GEOIP} ${LIB_JANSSON} m)

install(TARGETS logswan DESTINATION bin)
