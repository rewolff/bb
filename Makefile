

CC=gcc 
CFLAGS=-Wall -O2 -ggdb
#  --std=c99 -pedantic

EXE=bb_create bb_print bb_set bb_mon bb_nom bb_parport bb_ftdi \
	bb_xor bb_mon_ping bb_list bb_kitt bb_alarm bb_mmon \
	bb_mnom bb_log bb_printlog bb_remote bb_procvar \
	bb_createlog bb_filter bb_dio rv
SCRIPTS=bb_df bb_sysload set_var_to_exitstatus

all:$(EXE)

LDLIBS=bb_lib.o


bb_ftdi: bb_ftdi.o
	$(CC) $< -lftdi -lusb -o $@


clean: 
	rm -f *.o *~ $(EXE)

