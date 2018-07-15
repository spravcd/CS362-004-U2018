#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "dominion.h"
#include "rngs.h"

#define VERBOSE_TEST 0


int testIsGameOver() {
	struct gameState state;
	int seed=10;
	int numPlayers = MAX_PLAYERS;
	//int handSize = 500;
	int globalFail=0;
    int k[10] = {adventurer, smithy, sea_hag, gardens, village,
				council_room, feast, mine, remodel, great_hall};
	
	initializeGame(numPlayers, k, seed, &state);

	// test initial state
	if (isGameOver(&state)) {
		printf("FAIL: initial state\n");
		globalFail=1;
	}
	// test 1 province cards left
	state.supplyCount[province] = 1;
	if (isGameOver(&state)) {
		printf("FAIL: single province\n");
		globalFail=1;
	}	
	// test 0 province cards left
	state.supplyCount[province] = 0;
	if (!isGameOver(&state)) {
		printf("FAIL: province condition\n");
		globalFail=1;
	}
	// test 1 supply pile empty (but test to make sure it's not depenedent on position)
	state.supplyCount[province]=1;
	int i=0;
	for (i=0; i< 25; i++) {
		int j;
		for (j=0; j < 25; j++) {
			if (i == j) {
				state.supplyCount[i] = 0;
			}
			// don't let a single empty province pile end the game
			else if (i == province) {
				state.supplyCount[province] =1;
			}
			else {
				state.supplyCount[i] = 1;
			}
		}
	}
	if (isGameOver(&state)) {
		printf("FAIL: single empty supply pile\n");
		globalFail=1;
	}
	// test two empty supply piles do not end game (last pile still empty from previous loop)
	state.supplyCount[10] = 0;
	if (isGameOver(&state)) {
		printf("FAIL: two empty supply piles\n");
		globalFail=1;
	}	
	// test three empty supplies piles do end game (two piles empty from previous loops)
	state.supplyCount[15] = 0;
	if (!isGameOver(&state)) {
		printf("FAIL: three empty supply piles\n");
		globalFail=1;
	}
	// test that 4 empty piles still ends game
	state.supplyCount[20] = 0;
	if (!isGameOver(&state)) {
		printf("FAIL: four empty supply piles\n");
		globalFail=1;
	}
	// test that all empty piles (except for province) still ends the game
	for (i=0; i< 25; i++) {
		// don't let a single empty province pile end the game
		if (i == province) {
			state.supplyCount[province] =1;
		}
		else {
			state.supplyCount[i] = 0;
		}
	}
	if (!isGameOver(&state)) {
		printf("FAIL: all empty supply piles\n");
		globalFail=1;
	}
	return globalFail;
}

int main() {
	printf("Testing isGameOver function...\n");
	
	if (testIsGameOver()) {
		printf("isGameOver function FAILED\n");
	}
	else {
		printf("isGameOver function PASSED\n");
	}
	printf("Testing isGameOver function complete\n");
}