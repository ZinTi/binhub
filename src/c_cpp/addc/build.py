#!/usr/bin/python3
import os
print("编译 AddC")
os.system("g++ addc.cpp -o addc -O3 -static")
