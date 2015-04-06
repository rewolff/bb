#!/usr/bin/python

import struct
import sys
import mmap

class bb_var:
    def __init__ (self, shm, offset):
        self.shm = shm
        self.offset = offset

    def val(self):
        #print "offset = ", self.offset
        vall = struct.unpack('I', self.shm[self.offset:self.offset+4])
        return vall[0]



class bb:
    def __init__ (self):
        shmf = open ('/home/wolff/.bb_shm', 'r+')
        self.shm = mmap.mmap (shmf.fileno(), 0)
        f = open ('/home/wolff/.bb_names', 'r')
        self.types = {}
        self.offsets = {}
        for line in f:
            line = line.rstrip ()
            spl = line.split (" ")
            self.types[spl[0]] = spl[2]
            self.offsets[spl[0]] = int (spl[1], 16)

    def var (self, name):
        return bb_var (self.shm, self.offsets[name])
        


