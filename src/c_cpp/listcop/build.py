#!/usr/bin/python3
import os
print("编译listcop")
os.system("gcc listcop.c color.c -o listcop -I. -Os")
