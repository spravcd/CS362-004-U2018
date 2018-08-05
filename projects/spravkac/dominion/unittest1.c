/* 
unit test for handCard function to determine value of card at handPos 
22 JUL 2018
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "dominion.h"
#include "rngs.h"

#define VERBOSE_TEST 0

void randHand(struct gameState *state, int handSize) {
	int cP = state->whoseTurn;
	int numKingdomCards = 10;
	state->handCount[cP]=0;
	while (state->handCount[cP] < handSize) {
		state->hand[cP][state->handCount[cP]] = floor(Random() * numKingdomCards);
		state->handCount[cP]++;
	}
}
// test kingdom cards, estate cards, and coin cards
int testHandCard(int numPlayers, int handSize) {
	struct gameState state;
	int seed=10;
	//int numPlayers = MAX_PLAYERS;
	int currPlayer = 0;
	//int handSize = 500;
	int setFail=0;
	int globalFail=0;
    int k[10] = {adventurer, smithy, sea_hag, gardens, village,
				council_room, feast, mine, remodel, great_hall};
	
	// game init limits starting cards to copper and estate (not a full test)
	initializeGame(numPlayers, k, seed, &state);	
	// for each player in the game, cycle through each card in their hand
	// and ensure that it matches what you'd get by manually checking
	for (currPlayer = 0; currPlayer < numPlayers; currPlayer++) {
		int i=0;
		// first test the starting cards
		state.whoseTurn = currPlayer;
		state.handCount[currPlayer] = 0;
		for (i=0; i < handSize; i++) {
			drawCard(currPlayer, &state);
		}
		
		//printHand(currPlayer, &state);
		for (i=0; i < state.handCount[currPlayer]; i++) {
			if (handCard(i, &state) == state.hand[currPlayer][i]) {
				//printf("PASS: Player %i, cardPos %i\n", currPlayer, i);
			}
			else {
				printf("FAIL: Player %i, cardPos %i\n", currPlayer, i);
				setFail=1;
				globalFail=1;
			}
		}
		if (!setFail && VERBOSE_TEST) {
			printf("PASS: Player %i Starting Cards\n", currPlayer);
		}
		setFail=0;
		// now test kindom cards
		state.handCount[currPlayer] = 0;
		randHand(&state, handSize);
		for (i=0; i < state.handCount[currPlayer]; i++) {
			if (handCard(i, &state) == state.hand[currPlayer][i]) {
				//printf("PASS: Player %i, cardPos %i\n", currPlayer, i);
			}
			else {
				printf("FAIL: Player %i, cardPos %i\n", currPlayer, i);
				setFail=1;
				globalFail=1;
			}
		}
		if (!setFail && VERBOSE_TEST) {
			printf("PASS: Player %i KindomCards\n", currPlayer);
		}
	}
	return globalFail;
}


//void testHandCard() {
	/*
int main() {
	printf("Testing handCard function ...\n");
	// test corners
	if (testHandCard(MAX_PLAYERS, MAX_HAND)) {
		printf("FAIL");
	}
	else {
		printf("PASS");
	}
	printf(": MAX_PLAYERS with MAX_HAND\n");
	if (testHandCard(2, 5)) {
		printf("FAIL");
	}
	else {
		printf("PASS");
	}
	printf(": 2 players with 5 card hand\n");
	if (testHandCard(2, 0)) {
		printf("FAIL");
	}
	else {
		printf("PASS");
	}
	printf(": 2 players with 0 card hand\n");
	
	printf("Testing handCard function complete\n");

}
*/