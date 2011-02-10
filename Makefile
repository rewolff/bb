

CC=gcc 
CFLAGS=-Wall -O2 -g

EXE=bb_create bb_print bb_set bb_mon bb_nom bb_parport bb_ftdi bb_xor bb_alarm
all:$(EXE)



bb_create: bb_create.o bb_lib.o

bb_print: bb_print.o bb_lib.o

bb_set: bb_set.o bb_lib.o

bb_mon: bb_mon.o bb_lib.o

bb_nom: bb_nom.o bb_lib.o

bb_ftdi: bb_ftdi.o
	$(CC) $< -lftdi -lusb -o $@


clean: 
	rm -f *.o *~ $(EXE)

