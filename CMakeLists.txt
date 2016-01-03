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

include(GNUInstallDirs)
include(FindPkgConfig)

project (logswan C)

include(CheckFunctionExists)
include_directories(SYSTEM)

list(APPEND CMAKE_REQUIRED_DEFINITIONS -D_OPENBSD_SOURCE)
check_function_exists(strtonum HAVE_STRTONUM)

# LibM
find_path(LIBM_INCLUDE_DIR math.h)
find_library(LIBM_LIBRARIES NAMES m)
include_directories(${LIBM_INCLUDE_DIR})
message(STATUS "LIBM_INCLUDE_DIR ${LIBM_INCLUDE_DIR}")
message(STATUS "LIBM_LIBRARIES ${LIBM_LIBRARIES}")

# GeoIP (missing pkg-config)
find_path(GEOIP_INCLUDE_DIR GeoIP.h)
find_library(GEOIP_LIBRARIES NAMES GeoIP REQUIRED)
include_directories(${GEOIP_INCLUDE_DIR})
message(STATUS "GEOIP_INCLUDE_DIR ${GEOIP_INCLUDE_DIR}")
message(STATUS "GEOIP_LIBRARIES ${GEOIP_LIBRARIES}")

# jansson
pkg_search_module(JANSSON REQUIRED jansson)
include_directories(${JANSSON_INCLUDE_DIRS})
link_directories(${JANSSON_LIBRARY_DIRS})
message(STATUS "JANSSON_INCLUDE_DIRS ${JANSSON_INCLUDE_DIRS}")
message(STATUS "JANSSON_LIBRARIES ${JANSSON_LIBRARIES}")
message(STATUS "JANSSON_LIBRARY_DIRS ${JANSSON_LIBRARY_DIRS}")

set (DEPS deps/hll/hll.c deps/MurmurHash3/MurmurHash3.c)
set (SRC src/logswan.c src/config.c src/output.c src/parse.c)

if(NOT HAVE_STRTONUM)
  set (SRC ${SRC} compat/strtonum.c)
endif()

SET(GEOIPDATA ${CMAKE_INSTALL_PREFIX}/share/GeoIP CACHE PATH "Path to GeoIP data files")

add_definitions(-Wall -Wextra -Werror -std=c99 -pedantic)
add_definitions(-DDATADIR="${GEOIPDATA}")
add_executable(logswan ${SRC} ${DEPS})

target_link_libraries(logswan ${GEOIP_LIBRARIES} ${JANSSON_LIBRARIES} ${LIBM_LIBRARIES})

install(TARGETS logswan DESTINATION bin)
install(FILES logswan.1 DESTINATION ${CMAKE_INSTALL_MANDIR}/man1/)
