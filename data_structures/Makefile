#
# Makefile
# SmallSat Routing Protocol 
#
#  Created by Phillip Romig on 4/3/12.
#  Modified by Josh Rands on 10/4/19
#  Copyright 2012 Colorado School of Mines. All rights reserved.
#

CXX = g++
LD = g++
LIB = ar
INC_DIRS = -I../

CXXFLAGS = -g -fPIC -pthread -std=c++17 -DBOOST_LOG_DYN_LINK ${INC_DIRS}
LIBFLAGS = -cvq
LDFLAGS = -g -pthread ${INC_DIRS}

# You should be able to add object files here without changing anything else
TARGET = test
LIB_NAME = libdatastructures.a
OBJ_LIB_FILES = buffered_packet.o packet_buffer.o

STATIC_LIBRARIES = 

OBJ_FILES = ${TARGET}.o ${OBJ_LIB_FILES} 
INC_FILES = buffered_packet.h packet_buffer.h defines.h

# Any libraries we might need.
LIBRARYS = -lpthread

${TARGET}: ${OBJ_FILES}
	${LD} ${LDFLAGS} ${OBJ_FILES} -o $@ ${LIBRARYS}
	rm test.o
	${LIB} ${LIBFLAGS} ${LIB_NAME} ${OBJ_LIB_FILES}
	rm -f "*.o"

%.o : %.cc ${INC_FILES}
	${CXX} -c ${CXXFLAGS} -o $@ $<

clean:
	rm -f core ${TARGET} ${OBJ_FILES} ${LIB_NAME}
	rm -f "*.o"
