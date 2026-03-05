########################################################
CC=gcc
CFLAGS= -g -Wall -pedantic
##################################################
EJS = p2_e1
EJS_2a = p2_e2a
EJS_2b = p2_e2b
EJS_3 = p2_e3
EJS_ALL = p2_e1 p2_e2a p2_e2b p2_e3
################################################
DEBUG = valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes
FTXT_1 = playlist1.txt
FTXT_2 = playlist2.txt
FTXT_2 = radio.txt
########################################################
OBJECTSP2E1 = p2_e1.o music.o
OBJECTSP2E2a = p2_e2a.o radio.o music.o
OBJECTSP2E2b = p2_e2a.o radio.o music.o
OBJECTSP2E3 = p2_e3.o radio.o music.o
########################################################

all: $(EJS_ALL)
	make $(EJS_ALL)



## DEBUGS OF *.c

e1_DEBUG:  $(EJS)
	$(DEBUG) ./$(EJS)

e2a_DEBUG: $(EJS_2a)
	$(DEBUG) ./$(EJS_2a)

e2b_DEBUG: $(EJS_2b)
	$(DEBUG) ./$(EJS_2b)




e3_DEBUG:  $(EJS_3)
	$(DEBUG) ./$(EJS_3) $(FTXT)




## TADs OF MUSIC AND RADIO
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
	$(DEBUG) ./p2_e1
	@echo "\n>>>>>> Running p1_e2 with valgrind"
	$(DEBUG) ./p2_e2
	@echo "\n>>>>>> Running p1_e3"
	$(DEBUG) ./p2_e3 radio1.txt


