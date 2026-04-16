CC = gcc
CFLAGS = -g -Wall -Wextra -pedantic -std=c99
IFLAGS = -I.

TARGET = p4_e1
OBJS = p4_e1.o bstree.o radio.o music.o stack.o queue.o list.o

.PHONY: all clean run_e1_normal run_e1_sorted

all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) $(IFLAGS) -c -o $@ $<

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

run_e1_normal: $(TARGET)
	./$(TARGET) data_music_10.txt 8 normal

run_e1_sorted: $(TARGET)
	./$(TARGET) data_music_10.txt 8 sorted

clean:
	rm -f *.o $(TARGET)
