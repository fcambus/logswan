###############################################################################
#                                                                             #
# Logswan (c) by Frederic Cambus 2015                                         #
# https://github.com/fcambus/logswan                                          #
#                                                                             #
# Last Updated: 2015/06/07                                                    #
#                                                                             # 
###############################################################################

cmake_minimum_required (VERSION 2.6)

project (logswan C)

find_library(LIB_GEOIP NAMES GeoIP REQUIRED)
include_directories(/usr/local/include)

add_definitions(-Wall -Wextra -Werror -std=c99 -pedantic)

add_executable(logswan src/logswan.c src/parse.c)

target_link_libraries(logswan ${LIB_GEOIP})
