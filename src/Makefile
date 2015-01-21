# Makefile for Tomasulo algorithm simulator
# Copyright (C) 2014-2015 Wen Wen (wew55@pitt.edu),
#                         Joydeep Rakshit (jor73@pitt.edu),
#                         Lu Zhang (luz14@pitt.edu)

PROG = tomasulo
OUTPUT_PREFIX = ./bin/
PROG_OUTPUT = $(OUTPUT_PREFIX)tomasulo

CC= g++ #Clang++ is better for OSX
OFLAGS= -o
CFLAGS= -c


#make target

all:$(PROG)

tomasulo: main.o \
	 parser.o\
	 ARF.o\
	 CDB.o\
	 FU.o\
	 LSQ.o\
	 ROB.o\
 	 RS.o\
 	 RAT.o\
	 print.o\
	 BRN.o
	$(CC) $(OFLAGS) $(PROG_OUTPUT)  main.o \
	 parser.o\
	 ARF.o\
	 CDB.o\
	 FU.o\
	 LSQ.o\
	 ROB.o\
	 RS.o\
	 RAT.o\
	 print.o\
	 BRN.o


main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp
parser.o: parser.cpp
	$(CC) $(CFLAGS) parser.cpp
ARF.o: ARF.cpp
	$(CC) $(CFLAGS) ARF.cpp
CDB.o: CDB.cpp
	$(CC) $(CFLAGS) CDB.cpp
FU.o: FU.cpp
	$(CC) $(CFLAGS) FU.cpp
LSQ.o: LSQ.cpp
	$(CC) $(CFLAGS) LSQ.cpp
ROB.o: ROB.cpp
	$(CC) $(CFLAGS) ROB.cpp
RS.o: RS.cpp
	$(CC) $(CFLAGS) RS.cpp
RAT.o: RAT.cpp
	$(CC) $(CFLAGS) RAT.cpp
print.o: print.cpp
	$(CC) $(CFLAGS) print.cpp
BRN.o: BRN.cpp
	$(CC) $(CFLAGS) BRN.cpp

clean:
	rm ./*.o
