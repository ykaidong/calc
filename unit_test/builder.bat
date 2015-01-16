
gcc -Wall -c calc_test.c
gcc -Wall -c .\cmockery\cmockery.c
gcc -Wall -D UNIT_TESTING -c ..\src\calc.c
gcc -o calc_test.exe calc.o cmockery.o calc_test.o

calc_test.exe

pause
