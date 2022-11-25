all: test_exe ecosys_exe

ecosys.o: ecosys.h ecosys.c
	gcc -c -o ecosys.o ecosys.c

main_test.o: ecosys.h main_tests.c
	gcc -c -o main_test.o main_tests.c

test_exe: ecosys.o main_test.o
	gcc -o test_exe main_test.o ecosys.o

main_ecosys.o: ecosys.h main_ecosys.c
	gcc -c -o main_ecosys.o main_ecosys.c

ecosys_exe: ecosys.o main_ecosys.o
	gcc -o ecosys_exe main_ecosys.o ecosys.o 

clean:
	rm -f *.o test_exe ecosys_exe