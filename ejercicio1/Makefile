########################################################
CC=gcc
# CFLAGS= -g -Wall -pedantic
CFLAGS= -g -Wall
# EJS = p1_e1
EJS = p1_e1 p1_e2 p1_e3
########################################################
OBJECTSP1E1 = p1_e1.o music.o
OBJECTSP1E2 = p1_e2.o radio.o music.o
OBJECTSP1E3 = p1_e3.o radio.o music.o
########################################################

all: $(EJS)

p1_e1: $(OBJECTSP1E1)
	$(CC) $(CFLAGS) -o $@ $(OBJECTSP1E1)

p1_e2: $(OBJECTSP1E2)
	$(CC) $(CFLAGS) -o $@ $(OBJECTSP1E2)

p1_e3: $(OBJECTSP1E3)
	$(CC) $(CFLAGS) -o $@ $(OBJECTSP1E3)


p1_e1.o: p1_e1.c music.h
	$(CC) $(CFLAGS) -c $<

p1_e2.o: p1_e2.c music.h radio.h
	$(CC) $(CFLAGS) -c $<

p1_e3.o: p1_e3.c music.h radio.h
	$(CC) $(CFLAGS) -c $<

music.o: music.c music.h
	$(CC) $(CFLAGS) -c $<

radio.o: radio.c radio.h music.h
	$(CC) $(CFLAGS) -c $<

clear:
	rm -f *.o

clean:
	rm -f *.o $(EJS)

run: all
	@echo ">>>>>> Running p1_e1"
	./p1_e1
	@echo "\n>>>>>> Running p1_e2"
	./p1_e2
	@echo "\n>>>>>> Running p1_e3 (needs a file, e.g., radio1.txt)"
	@echo "Usage: ./p1_e3 radio1.txt"


runv: all
	@echo ">>>>>> Running p1_e1 with valgrind"
	valgrind --leak-check=full ./p1_e1
	@echo "\n>>>>>> Running p1_e2 with valgrind"
	valgrind --leak-check=full --track-origins=yes ./p1_e2
	@echo "\n>>>>>> Running p1_e3 with valgrind (needs a file, e.g., radio1.txt)"
	@echo "Usage: valgrind --leak-check=full ./p1_e3 radio1.txt"
