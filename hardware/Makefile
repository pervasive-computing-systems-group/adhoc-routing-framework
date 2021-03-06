#
# Makefile
# SmallSat Routing Protocol 
#
#  Created by Phillip Romig on 4/3/12.
#  Modified by Josh Rands on 9/2/19
#  Modified by Zachary Smeton on 10/1/19
#  Copyright 2012 Colorado School of Mines. All rights reserved.
#

CXX = g++
LD = g++
LIB = ar
INC_DIRS = -I../aodv -I../socket -I../adhoc -I../hello_monitor -I../ -I../data_structures/ -I./rssi -I../single_hop

CXXFLAGS = -g -fPIC -pthread -std=c++17 -DBOOST_LOG_DYN_LINK -O2 -g -Wall -Wundef -Wno-trigraphs -fno-strict-aliasing -fno-common ${INC_DIRS}
LDFLAGS = -g -pthread -Wall -O2 -g -Wall -Wundef -Wno-trigraphs -fno-strict-aliasing -fno-common ${INC_DIRS}
LIBFLAGS = -cvq 

#
# You should be able to add object files here without changing anything else
#
TARGET = test
LIB_NAME = libhardware.a
OBJ_FILES = hardware_defines.o hardware_aodv.o hardware_hello.o hardware_hello_aodv.o hardware_rssi.o hardware_sh_ap.o hardware_sh_station.o
OBJ_LIB_FILES = ${OBJ_FILES} ../aodv/*.o ../socket/*.o ../data_structures/*.o ../adhoc/*.o ../hello_monitor/*.o ./rssi/radiotap_capture.o ./rssi/radiotap.o ../single_hop/*.o
OBJ_INCLUDE_FILES = ${TARGET}.o ${OBJ_LIB_FILES}
OBJ_BUILD_FILES = ${TARGET}.o ${OBJ_FILES}

INC_FILES = hardware_aodv.h hardware_sh_ap.h hardware_sh_station.h hardware_defines.h hardware_hello.h hardware_hello_aodv.h hardware_rssi.h

SUBDIRS = ../aodv ../socket ../data_structures ../adhoc ../hello_monitor ./rssi ../single_hop

#
# Any libraries we might need.
#
LIBRARYS = -lpthread -lpcap

${TARGET} : ${OBJ_BUILD_FILES}
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
	done
	${LD} ${LDFLAGS} ${OBJ_INCLUDE_FILES} -o $@ ${LIBRARYS}
	rm ${TARGET}.o
	${LIB} ${LIBFLAGS} ${LIB_NAME} ${OBJ_LIB_FILES}
	rm -f "*.o"

%.o : %.cc ${INC_FILES} ${INC_DIRS}
	${CXX} -c ${CXXFLAGS} -o $@ $< 

#
# Please remember not to submit objects or binarys.
#
clean:
	rm -f core ${TARGET} ${LIB_NAME}
	rm -f *.o
	rm -f *.d
