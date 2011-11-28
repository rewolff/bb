
#define BB_LOGFILE_MAGIC (*((int *)"BBLF"))

#define BB_LOG_HDRVERSION 1

struct logfile_header {
  int magic;
  int datastart;
  int hdrversion;
  int dt;
  int numsamples;
  int curpos;
  char pad[0x200 - 0x18];
};


