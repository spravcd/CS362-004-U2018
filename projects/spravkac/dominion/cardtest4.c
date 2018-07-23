/* 
unit test for great hall card 
22 JUL 2018
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "dominion.h"
#include "rngs.h"
#include "interface.h"


int testGreatHall() {
	struct gameState state;
	struct gameState origState;
	int seed=10;
	int numPlayers = MAX_PLAYERS;
	int cP = 0; // currentPlayer
	int i,j;
	int dummyHandCard = sea_hag;
	int dummyDeckCard = gardens;
	//int handSize = 500;
	int numTestCards=5;
	int globalFail=0;
	//int numDrawn;
    int k[10] = {adventurer, smithy, sea_hag, gardens, village,
				council_room, feast, mine, remodel, great_hall};
	
	initializeGame(numPlayers, k, seed, &state);
	
	for (i=0; i< numTestCards; i++) {
		state.hand[cP][i] = dummyHandCard;
		state.deck[cP][i] = dummyDeckCard;
	}
	state.hand[cP][0]=great_hall;
	memcpy(&origState, &state, sizeof(struct gameState));
	/*printHand(cP, &state);
	printDeck(cP, &state);
	printDiscard(cP, &state);
	printf("actions:%i\n", state.numActions);
	*/
	//playCard(0, 0,0,0, &state);
	cardEffect(great_hall, 0,0,0, &state, 0, 0);
	/*printHand(cP, &state);
	printDeck(cP, &state);
	printDiscard(cP, &state);
	printf("actions:%i\n", state.numActions);
	*/

	// check card was drawn from hand
	if (state.hand[cP][0] != origState.deck[cP][0]) {
		printf("FAIL: did not draw first card from deck");
		globalFail=1;
	}
	
	// check +1 action (used 1 action to play card, so actions should be equal only if using 'playCard func')
	if (state.numActions != origState.numActions+1) {
		printf("FAIL: did not icrement actions by exactly 1");
		globalFail=1;
	}
	// check that valid cards from deck were drawn
	if (fullDeckCount(cP, dummyDeckCard, &origState) != fullDeckCount(cP, dummyDeckCard, &state)) {
		printf("FAIL: cards not conserved during draw\n");
		globalFail=1;
	}
	if (fullDeckCount(cP, dummyHandCard, &origState) != fullDeckCount(cP, dummyHandCard, &state)) {
		printf("FAIL: cards not conserved during draw\n");
		globalFail=1;
	}
	// check for no state change for other players
	for (i=0; i < numPlayers; i++) {
		if (i != cP) {
			// test hand
			for (j=0; j<state.handCount[i]; j++) {
				if (state.hand[i][j] != origState.hand[i][j]) {
					printf("FAIL: state change for wrong player\n");
					globalFail=1;
				}
			}
			// test deck
			for (j=0; j<state.deckCount[i]; j++) {
				if (state.deck[i][j] != origState.deck[i][j]) {
					printf("FAIL: state change for wrong player\n");
					globalFail=1;
				}
			}
			// test discard
			for (j=0; j<state.discardCount[i]; j++) {
				if (state.discard[i][j] != origState.discard[i][j]) {
					printf("FAIL: state change for wrong player\n");
					globalFail=1;
				}
			}
		}
	}
	// check no state change for victory and kingdom cards
	for (i=0; i < card_count; i++) {
		if (state.supplyCount[i] != origState.supplyCount[i]) {
			printf("FAIL: state change for supply cards (victory, kingdom)\n");
		}
	}
	
	return globalFail;
}
/*
int main() {
	printf("Testing Great Hall implementation...\n");
	
	if (testGreatHall()) {
		printf("Great Hall implementation FAILED\n");
	}
	else {
		printf("Great Hall implementation PASSED\n");
	}
	printf("Testing Great Hall implementation complete\n");
}
*/