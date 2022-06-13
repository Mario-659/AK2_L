all: runnable

runnable: main.cpp
	g++ -Wall -m32 -O2 -o Runnable main.cpp

debug:
	rm RunnableDebug
	g++ -Wall -g -m32 -o RunnableDebug main.cpp
	./RunnableDebug
	