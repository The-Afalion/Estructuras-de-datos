CC = gcc
AR = ar
CFLAGS = -g -Wall -Wextra -pedantic
IFLAGS = -I.
LDFLAGS = -L.
LDLIBS = -lstack
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s

COMMON = music.o radio.o queue.o
TARGETS = p3_e1 p3_e2 p3_e3
TEST_TARGETS = tb_p3_e1 tb_p3_e2 tb_p3_e3

.PHONY: all clean libs tests check run_e1 runv_e1 run_e2 runv_e2 run_e3 runv_e3

all: $(TARGETS)

tests: $(TEST_TARGETS)

check: all tests
	./tb_p3_e1
	./tb_p3_e2
	./tb_p3_e3

libs: libstack.a

libstack.a: stack.o
	$(AR) rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) $(IFLAGS) -c -o $@ $<

p3_e1: p3_e1.o $(COMMON) libstack.a
	$(CC) $(CFLAGS) -o $@ p3_e1.o $(COMMON) $(LDFLAGS) $(LDLIBS)

p3_e2: p3_e2.o $(COMMON) libstack.a
	$(CC) $(CFLAGS) -o $@ p3_e2.o $(COMMON) $(LDFLAGS) $(LDLIBS)

p3_e3: p3_e3.o music.o radio.o queue.o list.o libstack.a
	$(CC) $(CFLAGS) -o $@ p3_e3.o music.o radio.o queue.o list.o $(LDFLAGS) $(LDLIBS)

tb_p3_e1: tb_p3_e1.o music.o radio.o queue.o libstack.a
	$(CC) $(CFLAGS) -o $@ tb_p3_e1.o music.o radio.o queue.o $(LDFLAGS) $(LDLIBS)

tb_p3_e2: tb_p3_e2.o music.o radio.o queue.o libstack.a
	$(CC) $(CFLAGS) -o $@ tb_p3_e2.o music.o radio.o queue.o $(LDFLAGS) $(LDLIBS)

tb_p3_e3: tb_p3_e3.o music.o radio.o queue.o list.o libstack.a
	$(CC) $(CFLAGS) -o $@ tb_p3_e3.o music.o radio.o queue.o list.o $(LDFLAGS) $(LDLIBS)

run_e1: p3_e1
	./p3_e1 radio.txt

runv_e1: p3_e1
	$(VALGRIND) ./p3_e1 radio.txt

run_e2: p3_e2
	./p3_e2 radio_bfs.txt 1 10

runv_e2: p3_e2
	$(VALGRIND) ./p3_e2 radio_bfs.txt 1 10

run_e3: p3_e3
	./p3_e3 radio_bfs.txt

runv_e3: p3_e3
	$(VALGRIND) ./p3_e3 radio_bfs.txt

clean:
	rm -f *.o *.a $(TARGETS) $(TEST_TARGETS) tb_*.txt tb_*.out tb_*.err *.exe
