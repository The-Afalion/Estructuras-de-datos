CC=gcc
CFLAGS=-g -Wall -Wextra -pedantic
IFLAGS=-I./
LDFLAGS=-L./
LDLIBS=-lstack
VALGRIND=valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s

COMMON=music.o radio.o file_utils.o p2_utils.o

all: p2_e1 p2_e2a p2_e2b p2_e3

p2_e1: p2_e1.o $(COMMON) libstack.a
	$(CC) $(CFLAGS) -o $@ p2_e1.o $(COMMON) $(LDFLAGS) $(LDLIBS)

p2_e2a: p2_e2a.o $(COMMON) libstack.a
	$(CC) $(CFLAGS) -o $@ p2_e2a.o $(COMMON) $(LDFLAGS) $(LDLIBS)

p2_e2b: p2_e2b.o $(COMMON) libstack.a
	$(CC) $(CFLAGS) -o $@ p2_e2b.o $(COMMON) $(LDFLAGS) $(LDLIBS)

p2_e3: p2_e3.o $(COMMON) libstack.a
	$(CC) $(CFLAGS) -o $@ p2_e3.o $(COMMON) $(LDFLAGS) $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) $(IFLAGS) -c $<

run_e1: p2_e1
	./p2_e1 radio.txt

runv_e1: p2_e1
	$(VALGRIND) ./p2_e1 radio.txt

run_e2a: p2_e2a
	./p2_e2a playlist1.txt playlist2.txt

runv_e2a: p2_e2a
	$(VALGRIND) ./p2_e2a playlist1.txt playlist2.txt

run_e2b: p2_e2b
	./p2_e2b playlistA.txt playlistB.txt

runv_e2b: p2_e2b
	$(VALGRIND) ./p2_e2b playlistA.txt playlistB.txt

run_e3: p2_e3
	./p2_e3 radio.txt 482 317

runv_e3: p2_e3
	$(VALGRIND) ./p2_e3 radio.txt 482 317

p2_testbench: p2_testbench.o $(COMMON) libstack.a
	$(CC) $(CFLAGS) -o $@ p2_testbench.o $(COMMON) $(LDFLAGS) $(LDLIBS)

run_testbench: p2_testbench
	./p2_testbench

runv_testbench: p2_testbench
	$(VALGRIND) ./p2_testbench

clean:
	rm -f *.o p2_e1 p2_e2a p2_e2b p2_e3 p2_testbench
