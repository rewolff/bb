#!/usr/bin/python

from bb import *

from Tkinter import *
import tkMessageBox
import Tkinter

class bbIntVar (IntVar):
    def __init__ (self, parent, bb, bbname):
        IntVar.__init__(self)
        self.bbv = bb.var (bbname)
        self.trace("w", self.callback)
        self.parent = parent
        self.timer ()

    def callback(self, *args):
        #print "args=" , args
        #print "changed, newval = ", self.get ()
        self.bbv.set (self.get())

    def timer (self):
        vv = self.bbv.val ()
        if (vv != self.get()):
            self.set(vv)
        self.parent.after (100, self.timer)

class bbVarButton:
    def __init__ (self, parent, bb, bbname):
        self.var = bbIntVar (parent, bb, bbname)
        self.but = Checkbutton(parent, text = bbname, variable = self.var)
        self.but.pack ()


class myapp(Frame):
  
    def __init__(self, parent):
        Frame.__init__(self, parent)   
        self.parent = parent
        self.bb = bb()
        self.initUI()
    
    def initUI(self):
        self.buttons = []
        for s in sys.argv[1:]:
            self.buttons.append (bbVarButton (self.parent, self.bb, s))
        self.quitb = Button(self.parent, text = "quit", command = self.quit )
        self.quitb.pack ()
        self.parent.title("Buttons")
        self.pack(fill=BOTH, expand=1)
    

#sanity check the arguments. 
if len(sys.argv) <= 1:
    print "Usage: ", sys.argv[0], " <bb_varnames>..."
    sys.exit ()


root = Tk()
app = myapp(root)
root.mainloop()  
