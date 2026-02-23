########################################################
CC=gcc
CFLAGS= -g -Wall -pedantic
##################################################
EJS = p1_e1
EJS_2 = p2_e2
EJS_3 = p3_e3
EJS_ALL = p1_e1 p1_e2 p1_e3
################################################
DEBUG = valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes
FTXT = radio1.txt

########################################################
OBJECTSP1E1 = p1_e1.o music.o
OBJECTSP1E2 = p1_e2.o radio.o music.o
OBJECTSP1E3 = p1_e3.o radio.o music.o
########################################################

all: $(EJS_ALL)
	make $(EJS_ALL)

e1_DEBUG:  p1_e1
	$(DEBUG) ./p1_e1

e2_DEBUG:  p1_e2
	$(DEBUG) ./p1_e2

e3_DEBUG:  p1_e3
	$(DEBUG) ./p1_e1 $(FTXT)





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
	rm -f *.o $(EJS_ALL)

run: all
	@echo ">>>>>> Running p1_e1"
	./p1_e1
	@echo "\n>>>>>> Running p1_e2"
	./p1_e2
	@echo "\n>>>>>> Running p1_e3" 
	./p1_e3 radio1.txt


runv: all
	@echo ">>>>>> Running p1_e1 with valgrind"
	valgrind --leak-check=full ./p1_e1
	@echo "\n>>>>>> Running p1_e2 with valgrind"
	valgrind --leak-check=full --track-origins=yes ./p1_e2
	@echo "\n>>>>>> Running p1_e3"
	valgrind --leak-check=full ./p1_e3 radio1.txt

runv_2:	$(EJS_ALL)
	@echo ">>>>>> Running p1_e1 with valgrind"
	$(DEBUG) ./p1_e1
	@echo "\n>>>>>> Running p1_e2 with valgrind"
	$(DEBUG) ./p1_e2
	@echo "\n>>>>>> Running p1_e3 with valgrind (needs a file, e.g., radio1.txt)"
	$(DEBUG) ./p1_e1 $(FTXT)
