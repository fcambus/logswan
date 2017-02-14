###############################################################################
#                                                                             #
# Logswan 1.07                                                                #
# Copyright (c) 2015-2017, Frederic Cambus                                    #
# https://www.logswan.org                                                     #
#                                                                             #
# Created:      2015-05-31                                                    #
# Last Updated: 2017-02-14                                                    #
#                                                                             #
# Logswan is released under the BSD 2-Clause license.                         #
# See LICENSE file for details.                                               #
#                                                                             #
###############################################################################

cmake_minimum_required (VERSION 2.6)

project(logswan C)

include(CheckFunctionExists)
include(GNUInstallDirs)

# Check if system has pledge and strtonum
list(APPEND CMAKE_REQUIRED_DEFINITIONS -D_OPENBSD_SOURCE)
check_function_exists(pledge HAVE_PLEDGE)
check_function_exists(strtonum HAVE_STRTONUM)

# Additional include directories for compat functions + dependencies
include_directories("compat")
include_directories("deps/hll")

# GeoIP
find_path(GEOIP_INCLUDE_DIRS GeoIP.h)
find_library(GEOIP_LIBRARIES NAMES GeoIP REQUIRED)
include_directories(${GEOIP_INCLUDE_DIRS})

# Jansson
find_path(JANSSON_INCLUDE_DIRS jansson.h)
find_library(JANSSON_LIBRARIES NAMES jansson REQUIRED)
include_directories(${JANSSON_INCLUDE_DIRS})

set(CMAKE_BUILD_TYPE Release)
set(DEPS deps/hll/hll.c deps/MurmurHash3/MurmurHash3.c)
set(SRC src/logswan.c src/config.c src/output.c src/parse.c)

if(NOT HAVE_PLEDGE)
  set (SRC ${SRC} compat/pledge.c)
endif()

if(NOT HAVE_STRTONUM)
  set (SRC ${SRC} compat/strtonum.c)
endif()

SET(GEOIPDIR ${CMAKE_INSTALL_PREFIX}/share/GeoIP CACHE PATH "Path to GeoIP databases")

add_definitions(-Wall -Wextra -Werror -std=c99 -pedantic)
add_definitions(-DGEOIPDIR="${GEOIPDIR}/")
add_executable(logswan ${SRC} ${DEPS})

target_link_libraries(logswan ${GEOIP_LIBRARIES} ${JANSSON_LIBRARIES} m)

install(TARGETS logswan DESTINATION bin)
install(FILES logswan.1 DESTINATION ${CMAKE_INSTALL_MANDIR}/man1/)
