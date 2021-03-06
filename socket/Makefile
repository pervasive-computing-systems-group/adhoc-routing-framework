#
# Makefile
# SmallSat Routing Protocol 
#
#  Created by Phillip Romig on 4/3/12.
#  Modified by Josh Rands on 9/2/19
#  Copyright 2012 Colorado School of Mines. All rights reserved.
#
CXX = g++
LD = g++
LIB = ar
INC_DIRS = -I../ -I../data_structures

CXXFLAGS = -g -fPIC -pthread -std=c++17 -DBOOST_LOG_DYN_LINK ${INC_DIRS}
LDFLAGS = -g -std=c++17 -pthread ${INC_DIRS}
LIBFLAGS = -cvq

#
# You should be able to add object files here without changing anything else
#
TARGET = test
LIB_NAME = libsocket.a 
OBJ_LIB_FILES = endpoint.o socket.o udp_socket.o tcp_socket.o tcp_ap_socket.o tcp_station_socket.o message.o ../data_structures/*.o

OBJ_FILES = ${TARGET}.o ${OBJ_LIB_FILES} 
INC_FILES = endpoint.h socket.h udp_socket.h tcp_ap_socket.h tcp_station_socket.h message.h tcp_socket.h app_packet_handler.h ../data_structures/safe_circular_queue.h ../defines.h socket_defines.h

SUBDIRS = ../data_structures

# Any libraries we might need.
LIBRARYS = -lpthread

${TARGET}: ${OBJ_FILES}
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
	done
	${LD} ${LDFLAGS} ${OBJ_FILES} -o $@ ${LIBRARYS}
	rm test.o
	${LIB} ${LIBFLAGS} ${LIB_NAME} ${OBJ_LIB_FILES}
	rm -f "*.o"

%.o : %.cc ${INC_FILES}
	${CXX} -c ${CXXFLAGS} -o $@ $<

clean:
	rm -f core ${TARGET} ${OBJ_FILES} ${LIB_NAME}
	rm -f "*.o"
