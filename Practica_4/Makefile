CC = gcc
CFLAGS = -g -Wall -Wextra -pedantic -std=c99
IFLAGS = -I.
OBJDIR = build

TARGET = p4_e1
TARGET_E3 = p4_e3
COMMON_OBJS = $(OBJDIR)/bstree.o $(OBJDIR)/radio.o $(OBJDIR)/music.o \
              $(OBJDIR)/stack.o $(OBJDIR)/queue.o $(OBJDIR)/list.o
OBJS = $(OBJDIR)/p4_e1.o $(COMMON_OBJS)
OBJS_E3 = $(OBJDIR)/p4_e3.o $(COMMON_OBJS)

.PHONY: all clean rebuild_e1 rebuild_e3 run_e1_normal run_e1_sorted run_e1_100_normal run_e1_100_sorted run_e1_1k_normal run_e1_1k_sorted run_e1_all run_e3

all: $(TARGET) $(TARGET_E3)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(IFLAGS) -c -o $@ $<

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

$(TARGET_E3): $(OBJS_E3)
	$(CC) $(CFLAGS) -o $@ $(OBJS_E3)

rebuild_e1: clean
	$(MAKE) $(TARGET)

rebuild_e3: clean
	$(MAKE) $(TARGET_E3)

run_e1_normal: rebuild_e1
	./$(TARGET) data_music_10.txt 8 normal

run_e1_sorted: rebuild_e1
	./$(TARGET) data_music_10.txt 8 sorted

run_e1_100_normal: rebuild_e1
	./$(TARGET) data_music_100.txt 91 normal

run_e1_100_sorted: rebuild_e1
	./$(TARGET) data_music_100.txt 91 sorted

run_e1_1k_normal: rebuild_e1
	./$(TARGET) data_music_1K.txt 937 normal

run_e1_1k_sorted: rebuild_e1
	./$(TARGET) data_music_1K.txt 937 sorted

run_e1_all: rebuild_e1
	./$(TARGET) data_music_10.txt 8 normal
	./$(TARGET) data_music_10.txt 8 sorted
	./$(TARGET) data_music_100.txt 91 normal
	./$(TARGET) data_music_100.txt 91 sorted
	./$(TARGET) data_music_1K.txt 937 normal
	./$(TARGET) data_music_1K.txt 937 sorted

run_e3: rebuild_e3
	./$(TARGET_E3) data_music_10.txt 240

clean:
	rm -rf $(OBJDIR) *.o $(TARGET) $(TARGET_E3)
