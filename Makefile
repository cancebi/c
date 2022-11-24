all: ecosys_exe

ecosys.o: ecosys.h ecosys.c
	gcc -c -o ecosys.o ecosys.c

main_test.o: ecosys.h main_test.c
	gcc -c -o main.o main_test.c

ecosys_exe: ecosys.o main.o
	gcc -o ecosys_exe main.o ecosys.o

clean:
	rm -f *.o ecosys_exe