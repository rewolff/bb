#!/usr/bin/python


from bb import *

bb = bb ()

bbv = bb.var (sys.argv[1])

print bbv.val ()

sys.exit ()


