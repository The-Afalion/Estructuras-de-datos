CC = gcc
CFLAGS = -g -Wall -Wextra -pedantic -std=c99
IFLAGS = -I.

TARGET = p4_e1
TARGET_E3 = p4_e3
OBJS = p4_e1.o bstree.o radio.o music.o stack.o queue.o list.o
OBJS_E3 = p4_e3.o bstree.o radio.o music.o stack.o queue.o list.o

.PHONY: all clean run_e1_normal run_e1_sorted run_e1_100_normal run_e1_100_sorted run_e1_1k_normal run_e1_1k_sorted run_e1_all run_e3

all: $(TARGET) $(TARGET_E3)

%.o: %.c
	$(CC) $(CFLAGS) $(IFLAGS) -c -o $@ $<

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

$(TARGET_E3): $(OBJS_E3)
	$(CC) $(CFLAGS) -o $@ $(OBJS_E3)

run_e1_normal: $(TARGET)
	./$(TARGET) data_music_10.txt 8 normal

run_e1_sorted: $(TARGET)
	./$(TARGET) data_music_10.txt 8 sorted

run_e1_100_normal: $(TARGET)
	./$(TARGET) data_music_100.txt 91 normal

run_e1_100_sorted: $(TARGET)
	./$(TARGET) data_music_100.txt 91 sorted

run_e1_1k_normal: $(TARGET)
	./$(TARGET) data_music_1K.txt 937 normal

run_e1_1k_sorted: $(TARGET)
	./$(TARGET) data_music_1K.txt 937 sorted

run_e1_all: run_e1_normal run_e1_sorted run_e1_100_normal run_e1_100_sorted run_e1_1k_normal run_e1_1k_sorted

run_e3: $(TARGET_E3)
	./$(TARGET_E3) data_music_10.txt 240

clean:
	rm -f *.o $(TARGET) $(TARGET_E3)
