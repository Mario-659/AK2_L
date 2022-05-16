all: Runnable

Runnable: main.cpp
	g++ -Wall -m32 -o Runnable main.cpp

clean:
	rm Runnable Runnable_GDB

debug:
	rm Runnable_GDB
	g++ -g -Wall -m32 -o Runnable_GDB main.cpp
	gdb Runnable_GDB