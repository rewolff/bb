#!/usr/bin/python


from bb import *

bb = bb ()

bbv = bb.var (sys.argv[1])

bbv.set (int (sys.argv[2]))

sys.exit ()


