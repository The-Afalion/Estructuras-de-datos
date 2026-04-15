CC = gcc
AR = ar
CFLAGS = -g -Wall -Wextra -pedantic
IFLAGS = -I.
LDFLAGS = -L.
LDLIBS = -lstack
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s

COMMON = music.o radio.o queue.o
TARGETS = p3_e1 p3_e2 p3_e3
QLIST_TARGETS = p3_e1s p3_e2s

.PHONY: all clean libs run_e1 runv_e1 run_e2 runv_e2 run_e3 runv_e3 \
        run_e1s runv_e1s run_e2s runv_e2s

all: $(TARGETS) $(QLIST_TARGETS)

libs: libstack.a

libstack.a: stack.o
	$(AR) rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) $(IFLAGS) -c -o $@ $<

# --- Array-based queue targets ---
p3_e1: p3_e1.o $(COMMON) libstack.a
	$(CC) $(CFLAGS) -o $@ p3_e1.o $(COMMON) $(LDFLAGS) $(LDLIBS)

p3_e2: p3_e2.o $(COMMON) libstack.a
	$(CC) $(CFLAGS) -o $@ p3_e2.o $(COMMON) $(LDFLAGS) $(LDLIBS)

p3_e3: p3_e3.o music.o radio.o queue.o list.o libstack.a
	$(CC) $(CFLAGS) -o $@ p3_e3.o music.o radio.o queue.o list.o $(LDFLAGS) $(LDLIBS)

# --- List-based queue targets (Exercise 4) ---
p3_e1s: p3_e1.o music.o radio.o queueList.o list.o libstack.a
	$(CC) $(CFLAGS) -o $@ p3_e1.o music.o radio.o queueList.o list.o $(LDFLAGS) $(LDLIBS)

p3_e2s: p3_e2.o music.o radio.o queueList.o list.o libstack.a
	$(CC) $(CFLAGS) -o $@ p3_e2.o music.o radio.o queueList.o list.o $(LDFLAGS) $(LDLIBS)

# --- Run targets ---
run_e1: p3_e1
	./p3_e1 radio.txt

runv_e1: p3_e1
	$(VALGRIND) ./p3_e1 radio.txt

run_e2: p3_e2
	./p3_e2 radio_bfs.txt 1 2

runv_e2: p3_e2
	$(VALGRIND) ./p3_e2 radio_bfs.txt 1 2

run_e3: p3_e3
	./p3_e3 radio_bfs.txt

runv_e3: p3_e3
	$(VALGRIND) ./p3_e3 radio_bfs.txt

run_e1s: p3_e1s
	./p3_e1s radio.txt

runv_e1s: p3_e1s
	$(VALGRIND) ./p3_e1s radio.txt

run_e2s: p3_e2s
	./p3_e2s radio_bfs.txt 1 2

runv_e2s: p3_e2s
	$(VALGRIND) ./p3_e2s radio_bfs.txt 1 2

test_queue: test_queue.o queue.o
    $(CC) $(CFLAGS) -o $@ test_queue.o queue.o

run_test_queue: test_queue
    ./test_queue

runv_test_queue: test_queue
    $(VALGRIND) ./test_queue

clean:
	rm -f *.o *.a $(TARGETS) $(QLIST_TARGETS)
