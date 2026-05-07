#-----------------------------------------------------------------------------
# Makefile
#
# Targets:
#   all          - build every executable (p4_e1, p4_e2, p4_e3)
#   p4_e1        - Exercise 1 (find_min, find_max, contains, insert) + 2 (remove)
#   p4_e2        - Exercise 3 (tree_rangeSearch test program)
#   p4_e3        - Exercise 4 OPTIONAL (tree_countLongSongs test program)
#   run_eX       - run the X exercise with the canonical input
#   runv_eX      - same as run_eX but under valgrind
#   clean        - remove every intermediate file and binary
#-----------------------------------------------------------------------------

CC       = gcc
CFLAGS   = -g -Wall -Wextra -pedantic -std=c90
IFLAGS   = -I.
LDLIBS   =
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s

# Object files shared by every executable. We link with stack.o + queue.o
# (built from their .c files) instead of libstack.a so the build is
# self-contained and Valgrind can show full call stacks.
COMMON_OBJS = bstree.o radio.o music.o list.o queue.o stack.o

TARGETS = p4_e1 p4_e2 p4_e3

.PHONY: all clean \
        run_e1 runv_e1 run_e1_normal run_e1_sorted \
        run_e1_100_normal run_e1_100_sorted run_e1_1k_normal run_e1_1k_sorted \
        run_e1_all \
        run_e2 runv_e2 \
        run_e3 runv_e3

#-----------------------------------------------------------------------------
# Build rules
#-----------------------------------------------------------------------------

all: $(TARGETS)

%.o: %.c
	$(CC) $(CFLAGS) $(IFLAGS) -c -o $@ $<

p4_e1: p4_e1.o $(COMMON_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

p4_e2: p4_e2.o $(COMMON_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

p4_e3: p4_e3.o $(COMMON_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

#-----------------------------------------------------------------------------
# Run targets
#-----------------------------------------------------------------------------

# Default e1 run uses the example shown in the practice statement.
run_e1: p4_e1
	./p4_e1 data_music_10.txt 8 normal

runv_e1: p4_e1
	$(VALGRIND) ./p4_e1 data_music_10.txt 8 normal

# Variants used to compare normal vs sorted insertion.
run_e1_normal: p4_e1
	./p4_e1 data_music_10.txt 8 normal

run_e1_sorted: p4_e1
	./p4_e1 data_music_10.txt 8 sorted

run_e1_100_normal: p4_e1
	./p4_e1 data_music_100.txt 91 normal

run_e1_100_sorted: p4_e1
	./p4_e1 data_music_100.txt 91 sorted

run_e1_1k_normal: p4_e1
	./p4_e1 data_music_1K.txt 937 normal

run_e1_1k_sorted: p4_e1
	./p4_e1 data_music_1K.txt 937 sorted

run_e1_all: p4_e1
	./p4_e1 data_music_10.txt 8 normal
	./p4_e1 data_music_10.txt 8 sorted
	./p4_e1 data_music_100.txt 91 normal
	./p4_e1 data_music_100.txt 91 sorted
	./p4_e1 data_music_1K.txt 937 normal
	./p4_e1 data_music_1K.txt 937 sorted

# Exercise 3 (program p4_e2) - canonical example: ids in [3, 7]
run_e2: p4_e2
	./p4_e2 data_music_10.txt 3 7

runv_e2: p4_e2
	$(VALGRIND) ./p4_e2 data_music_10.txt 3 7

# Exercise 4 OPTIONAL (program p4_e3) - canonical example: duration > 300
run_e3: p4_e3
	./p4_e3 data_music_10.txt 300

runv_e3: p4_e3
	$(VALGRIND) ./p4_e3 data_music_10.txt 300

#-----------------------------------------------------------------------------
# Clean rule
#-----------------------------------------------------------------------------

clean:
	rm -f *.o $(TARGETS) $(TARGETS:%=%.exe)
