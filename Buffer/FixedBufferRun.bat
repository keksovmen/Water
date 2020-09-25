@echo off
setlocal
set SOURCE_FILES=FixedBufferTest.cpp
set OUTPUT_NAME=test.exe

echo Start compiling
g++ -o %OUTPUT_NAME% %SOURCE_FILES%
echo Finished
echo.
echo Start testing
echo.
%OUTPUT_NAME%
echo.
echo End testing
endlocal
