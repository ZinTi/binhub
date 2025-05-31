::  Windows Console
@echo off
echo PE: tellme.exe
gcc tellme.c -o tellme -Os -lws2_32
