@echo off

echo Assembling asmfunc.asm...
nasm -f win64 asmfunc.asm -o asmfunc.o

echo Compiling and linking...
gcc main.c asmfunc.o -o main.exe

echo.
echo Running program...
echo.
main.exe

pause
