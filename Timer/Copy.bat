@echo off
setlocal
set FILES=Clock.h Clock.cpp
set DES=E:\Programm\ArduinoSketch\Test\
set FLAGS=/Y
xcopy Clock.h %DES%Clock.h %FLAGS%
xcopy Clock.cpp %DES%Clock.cpp %FLAGS%
