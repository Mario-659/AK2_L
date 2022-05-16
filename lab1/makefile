all: add subb

add subb: add.o subb.o
	ld -m elf_i386 -s -o add add.o
	ld -m elf_i386 -s -o subb subb.o

add.o subb.o: add.s subb.s
	as add.s --32 -o add.o
	as subb.s --32 -o subb.o

clean:
	rm add.o add addGDB addGDB.o subb subb.o subbGDB

debug:
	as add.s -gstabs --32 -g -o addGDB.o
	ld -m elf_i386 -g -o addGDB addGDB.o
	as subb.s -gstabs --32 -g -o subbGDB.o
	ld -m elf_i386 -g -o subbGDB subbGDB.o
	# rm addGDB.o subbGDB.o 
	# gdb subbGDB