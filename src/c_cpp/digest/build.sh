#!/bin/bash
g++ -std=c++17 -Wall -Os -I/usr/local/openssl-3.4.0/include main.cpp HashUtils.cpp -o digest -L/usr/local/openssl-3.4.0/lib64 -L/usr/lib/x86_64-linux-gnu -lssl -lcrypto -ldl -lpthread -static
