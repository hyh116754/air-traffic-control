
#
#	Makefile for micha project
#

DEBUG = -g
CC = qcc
LD = qcc


TARGET = -Vgcc_ntox86_64
#TARGET = -Vgcc_ntox86
#TARGET = -Vgcc_ntoarmv7le
#TARGET = -Vgcc_ntoaarch64le


CFLAGS += $(DEBUG) $(TARGET) -Wall
LDFLAGS+= $(DEBUG) $(TARGET)
BINS = airplaneClient controlTowerServer gateClient
all: $(BINS)

clean:
	rm -f *.o $(BINS);
#	cd solutions; make clean

util.o: util.c util.h
airplaneClinet.o: airplaneClient.c airplaneClient.h util.c util.h msg_struct.h
controlTowerServer.o: controlTowerServer.c controlTowerServer.h util.c util.h msg_struct.h
gateClient.o: gateClient.c gateClient.h util.c util.h msg_struct.h

