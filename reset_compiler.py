#!/bin/python

import sys
import os
from subprocess import call

cxx = "g++"
if len(sys.argv) > 1:
    cxx = sys.argv[1]

os.environ["CXX"] = cxx
print("Resetting with " + os.environ["CXX"] +" compiler")
base = os.path.basename(os.getcwd())

if base == "Spring":
    os.chdir(".build")
    
call(["rm", "-rf"]+os.listdir(os.getcwd()))
call(["cmake", ".."])

    