###############################################################################
#                                                                             #
# Logswan (c) by Frederic Cambus 2015                                         #
# https://github.com/fcambus/logswan                                          #
#                                                                             #
# Last Updated: 2015/06/07                                                    #
#                                                                             # 
###############################################################################

CC = gcc
TARGET = all
CFLAGS = -Wall -ansi -pedantic

all :
	$(CC) $(CFLAGS) -o logswan logswan.c

clean:
	rm -f logswan *.o *.core core
