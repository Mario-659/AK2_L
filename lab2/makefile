all: calculator

calculator: calculator.o
	ld -m elf_i386 -s -o calculator calculator.o

calculator.o: calculator.s
	as calculator.s --32 -o calculator.o

clean:
	rm calculator.o calculator calculatorGDB calculatorGDB.o

debug:
	as calculator.s -gstabs --32 -g -o calculatorGDB.o
	ld -m elf_i386 -g -o calculatorGDB calculatorGDB.o
	# rm calculator GDB.o subbGDB.o 
	gdb calculatorGDB