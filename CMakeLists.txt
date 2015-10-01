###############################################################################
#                                                                             #
# Logswan 1.01 (c) by Frederic Cambus 2015                                    #
# https://github.com/fcambus/logswan                                          #
#                                                                             #
# Created:      2015/05/31                                                    #
# Last Updated: 2015/10/01                                                    #
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
set (SRC src/logswan.c src/definitions.c src/output.c src/parse.c)

if(NOT HAVE_STRTONUM)
  set (SRC ${SRC} compat/strtonum.c)
endif()

add_definitions(-Wall -Wextra -Werror -std=c99 -pedantic)
add_executable(logswan ${SRC} ${DEPS})

target_link_libraries(logswan ${LIB_GEOIP} ${LIB_JANSSON} m)

install(TARGETS logswan DESTINATION bin)
