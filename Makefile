

CC=gcc 
CFLAGS=-Wall -O2 -ggdb
#  --std=c99 -pedantic

EXE=bb_create bb_print bb_set bb_mon bb_nom bb_parport bb_ftdi \
	bb_xor bb_mon_ping bb_list bb_kitt bb_alarm

all:$(EXE)



bb_create: bb_create.o bb_lib.o

bb_print: bb_print.o bb_lib.o

bb_set: bb_set.o bb_lib.o

bb_mon: bb_mon.o bb_lib.o

bb_nom: bb_nom.o bb_lib.o

bb_mon_ping: bb_mon_ping.o bb_lib.o

bb_list: bb_list.o bb_lib.o

bb_alarm: bb_alarm.o bb_lib.o

bb_ftdi: bb_ftdi.o
	$(CC) $< -lftdi -lusb -o $@


clean: 
	rm -f *.o *~ $(EXE)

