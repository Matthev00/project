CC = g++
CFLAGS = -m64 -Wall
ASMBIN = nasm
LIBS = -L/usr/lib -lGL -lglut

all: program

program: main.o alpha_blending.o
	$(CC) $(CFLAGS) main.o alpha_blending.o -o program $(LIBS) -no-pie

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp -o main.o

alpha_blending.o: alpha_blending.s
	$(ASMBIN) -g -F dwarf -f elf64 alpha_blending.s -o alpha_blending.o

clean:
	rm -f ./*.o