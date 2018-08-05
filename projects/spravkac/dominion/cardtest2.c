#include <stdio.h>
#include <stdlib.h>
/* 
unit test for adventurer card 
22 JUL 2018
*/
#include <math.h>
#include <string.h>
#include "dominion.h"
#include "rngs.h"
#include "interface.h"


int testAdventurer() {
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
	state.hand[cP][0]=adventurer;
	state.deck[cP][numTestCards-2] = copper;
	state.deck[cP][numTestCards-1] = gold;
	memcpy(&origState, &state, sizeof(struct gameState));
	printHand(cP, &state);
	printDeck(cP, &state);
	//printDiscard(cP, &state);
	//playCard(0, 0,0,0, &state);
	cardEffect(adventurer, 0,0,0, &state, 0, 0);
	printHand(cP, &state);
	printDeck(cP, &state);
	//printDiscard(cP, &state);
	
	// check that the two (and only two!) treasure cards now appear in the hand
	int copperCount=0;
	int goldCount=0;
	for (i=0; i < state.handCount[cP]; i++) {
		if (state.hand[cP][i] == copper) {
			copperCount++;
		}
		else if (state.hand[cP][i] == gold) {
			goldCount++;
		}
	}
	if (copperCount != 1 || goldCount != 1) {
		printf("FAIL: wrong treasure count in hand");
		globalFail = 1;
	}
	// check that bypassed cards were discarded
	// this works because you put the treasure cards at the back of the deck, so everything but those
	// guys should be discarded
	if (state.discardCount[cP] != origState.discardCount[cP] + origState.deckCount[cP] -2) {
		printf("FAIL: wrong number of cards discarded\n");
		globalFail=1;
	}
	// check to make sure only the correct stuff was discarded
	for (i=0; i < state.discardCount[cP]; i++) {
		if (state.discard[cP][i] != dummyDeckCard) {
		printf("FAIL: wrong card discarded\n");
		globalFail=1;			
		}
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
	if (fullDeckCount(cP, copper, &origState) != fullDeckCount(cP, copper, &state)) {
		printf("FAIL: cards not conserved during draw\n");
		globalFail=1;
	}
	if (fullDeckCount(cP, gold, &origState) != fullDeckCount(cP, gold, &state)) {
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
	printf("Testing Adventurer implementation...\n");
	
	if (testAdventurer()) {
		printf("Adventurer implementation FAILED\n");
	}
	else {
		printf("Adventurer implementation PASSED\n");
	}
	printf("Testing Adventurer implementation complete\n");
}
*/