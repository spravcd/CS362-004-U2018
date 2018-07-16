CFLAGS = -Wall -fpic -coverage -lm -std=c99

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS)
#To run playdom you need to entere: ./playdom <any integer number> like ./playdom 10*/
testDrawCard: testDrawCard.c dominion.o rngs.o
	gcc  -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

badTestDrawCard: badTestDrawCard.c dominion.o rngs.o
	gcc -o badTestDrawCard -g  badTestDrawCard.c dominion.o rngs.o $(CFLAGS)

testBuyCard: testDrawCard.c dominion.o rngs.o
	gcc -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

testAll: dominion.o testSuite.c
	gcc -o testSuite testSuite.c -g  dominion.o rngs.o $(CFLAGS)

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

runtests: testDrawCard 
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out
	
unittests.o: dominion.o rngs.o unittest1.c unittest2.c unittest3.c unittest4.c interface.o
	gcc -c unittest1.c unittest2.c unittest3.c unittest4.c -g $(CFLAGS)
	
cardtests.o:  interface.o dominion.o rngs.o cardtest1.c cardtest2.c cardtest3.c cardtest4.c 
	gcc -c cardtest1.c cardtest2.c cardtest3.c cardtest4.c -g $(CFLAGS)
	
unittestresults: unittests.o cardtests.o
	gcc -o unittest1 -g unittest1.o dominion.o rngs.o interface.o $(CFLAGS)
	gcc -o unittest2 -g unittest2.o dominion.o rngs.o interface.o $(CFLAGS)
	gcc -o unittest3 -g unittest3.o dominion.o rngs.o interface.o $(CFLAGS)
	gcc -o unittest4 -g unittest4.o dominion.o rngs.o interface.o $(CFLAGS)
	gcc -o cardtest1 -g cardtest1.o dominion.o rngs.o interface.o $(CFLAGS)
	gcc -o cardtest2 -g cardtest2.o dominion.o rngs.o interface.o $(CFLAGS)
	gcc -o cardtest3 -g cardtest3.o dominion.o rngs.o interface.o $(CFLAGS)
	gcc -o cardtest4 -g cardtest4.o dominion.o rngs.o interface.o $(CFLAGS)
	./unittest1 &> unittestresults.out
	./unittest2 &> unittestresults.out


player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

all: playdom player 

clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe
