#include <stdio.h>
#include <stdlib.h>
#include "dominion.h"
#include "rngs.h"


void randHand(struct gameState *state, int handSize) {
	int i=0;
	int cP = state->whoseTurn;
	int numKingdomCards = 10;
	state->handCount[cP]=0;
	while (state->handCount[cP] < handSize) {
		state->hand[cP][state->handCount[cP]] = floor(Random() * numKingdomCards);
		state->handCount[cP]++;
	}
}

//void testHandCard() {
int main() {
	struct gameState state;
	int handPos=0;
	int seed=10;
	int numPlayers = MAX_PLAYERS;
	int currPlayer = MAX_PLAYERS;
	int handSize = 500;
	int setFail=0;
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
			}
		}
		if (!setFail) {
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
			}
		}
		if (!setFail) {
			printf("PASS: Player %i KindomCards\n", currPlayer);
		}
	}
}