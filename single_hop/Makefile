#
# Makefile
# SmallSat Routing Protocol 
#
#  Created by Phillip Romig on 4/3/12.
#  Modified by Jonathan Diller on 05/14/21
#  Copyright 2012 Colorado School of Mines. All rights reserved.
#

CXX = g++
LD = g++
LIB = ar
INC_DIRS = -I../ -I../adhoc -I../data_structures -I../socket

CXXFLAGS = -g -fPIC -pthread -std=c++17 -DBOOST_LOG_DYN_LINK ${INC_DIRS}
LDFLAGS = -g -pthread ${INC_DIRS}
LIBFLAGS = -cvq

#
# You should be able to add object files here without changing anything else
#
TARGET = test
LIB_NAME = libsingle_hop.a
OBJ_FILES = single_hop.o
OBJ_LIB_FILES = ${OBJ_FILES} ../adhoc/*.o ../socket/*.o ../data_structures/*.o
OBJ_INCLUDE_FILES = ${TARGET}.o ${OBJ_LIB_FILES}
OBJ_BUILD_FILES = ${TARGET}.o ${OBJ_FILES}

INC_FILES = ${TARGET}.h defines.h network_monitor.h monitor_info.h

SUBDIRS = ../adhoc ../socket ../data_structures 

${TARGET} : ${OBJ_BUILD_FILES}
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
	done
	${LD} ${LDFLAGS} ${OBJ_INCLUDE_FILES} -o $@ ${LIBRARYS}
	rm test.o
	${LIB} ${LIBFLAGS} ${LIB_NAME} ${OBJ_LIB_FILES}
	rm -f "*.o"

%.o : %.cc ${INC_FILES} ${INC_DIRS}
	${CXX} -c ${CXXFLAGS} -o $@ $< 

run:
	make 
	./test

clean:
	rm -f core ${TARGET} ${LIB_NAME}
	rm -f *.o
	rm -f *.d
