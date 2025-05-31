:: Windows Desktop Application
@echo off
echo "PE: CodeRain.exe (EasyX-mingw64)"
g++ CodeRain.cpp -o CodeRain -Os -I. -L. -leasyx -mwindows
