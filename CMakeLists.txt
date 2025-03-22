#
# Logswan 2.1.14
# Copyright (c) 2015-2025, Frederic Cambus
# https://www.logswan.org
#
# Created:      2015-05-31
# Last Updated: 2023-03-13
#
# Logswan is released under the BSD 2-Clause license.
# See LICENSE file for details.
#
# SPDX-License-Identifier: BSD-2-Clause
#

cmake_minimum_required(VERSION 3.1)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

project(logswan C)

include(CheckFunctionExists)
include(GNUInstallDirs)

# Conditional build options
set(ENABLE_SECCOMP 0 CACHE BOOL "Enable building with seccomp")

# Check if system has pledge and strtonum
list(APPEND CMAKE_REQUIRED_DEFINITIONS -D_OPENBSD_SOURCE)
check_function_exists(pledge HAVE_PLEDGE)
check_function_exists(strtonum HAVE_STRTONUM)

if(ENABLE_SECCOMP)
  # Check if system has seccomp
  message(STATUS "Looking for seccomp")
  find_path(SECCOMP NAMES "linux/seccomp.h")
  if(SECCOMP)
    message(STATUS "Looking for seccomp - found")
    add_definitions(-DHAVE_SECCOMP)
  else()
    message(STATUS "Looking for seccomp - not found")
  endif()
endif(ENABLE_SECCOMP)

# Additional include directories for compat functions + dependencies
include_directories("compat")
include_directories("deps/hll")

# libmaxminddb
find_path(GEOIP2_INCLUDE_DIRS maxminddb.h)
find_library(GEOIP2_LIBRARIES NAMES maxminddb REQUIRED)
include_directories(${GEOIP2_INCLUDE_DIRS})

# Jansson
find_path(JANSSON_INCLUDE_DIRS jansson.h)
find_library(JANSSON_LIBRARIES NAMES jansson REQUIRED)
include_directories(${JANSSON_INCLUDE_DIRS})

set(DEPS deps/hll/hll.c deps/MurmurHash3/MurmurHash3.c)
set(SRC src/logswan.c src/config.c src/continents.c src/countries.c
    src/output.c src/parse.c)

if(NOT HAVE_PLEDGE)
  set(SRC ${SRC} compat/pledge.c)
endif()

if(NOT HAVE_STRTONUM)
  set(SRC ${SRC} compat/strtonum.c)
endif()

set(GEOIP2DIR ${CMAKE_INSTALL_PREFIX}/share/dbip
    CACHE PATH "Path to GeoIP2 databases")
set(GEOIP2DB "dbip-country-lite.mmdb" CACHE PATH "GeoIP2 database file")

add_definitions(-D_GNU_SOURCE -Wall -Wextra -pedantic)
add_definitions(-DGEOIP2DIR="${GEOIP2DIR}/")
add_definitions(-DGEOIP2DB="${GEOIP2DB}")
add_executable(logswan ${SRC} ${DEPS})

target_link_libraries(logswan ${GEOIP2_LIBRARIES} ${JANSSON_LIBRARIES} m)

install(TARGETS logswan DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES man/logswan.1 DESTINATION ${CMAKE_INSTALL_MANDIR}/man1/)

enable_testing()
add_test(logswan logswan)
add_test(usage logswan -h)
add_test(version logswan -v)
add_test(processing logswan ${PROJECT_SOURCE_DIR}/tests/logswan.log)
add_test(invalid logswan ${PROJECT_SOURCE_DIR}/tests/invalid.log)
add_test(geolocation logswan -g -d ${PROJECT_SOURCE_DIR}/tests/logswan.mmdb
         ${PROJECT_SOURCE_DIR}/tests/logswan.log)
