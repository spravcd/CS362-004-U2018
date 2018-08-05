#include <stdio.h>
#include <stdlib.h>
/* 
random random unit test for village card 
05 AUG 2018
*/
#include <math.h>
#include <string.h>
#include "dominion.h"
#include "rngs.h"
#include "interface.h"
#include<time.h>

#define NOISY_TEST 0

int getCard(int cardRef, int* kindomCards) {
	if (cardRef < 10) {
		return kindomCards[cardRef];
	}
	
	else {
		return cardRef - 10; // curse through gold
	}	
}

int dispRand(int max) {
	return (int)floor(Random()*max);
}

int testVillage(int numHand, int numDeck) {
	struct gameState state;
	struct gameState origState;
	int seed=10;
	int numPlayers = MAX_PLAYERS;
	int cP = 0; // currentPlayer
	int i,j;
	int numCopper = 0;
	int numSilver = 0;
	int numGold= 0 ;
	int numDiscarded=0;
	int treasureCount=0;
	int firstTreasure=-1;
	
	int dummyHandCard = sea_hag;
	int dummyDeckCard = gardens;
	int numTestCards=5;
	
	int numDrawn;
	
	int globalFail=0;
	//int numDrawn;
	
    int k[10] = {adventurer, smithy, sea_hag, gardens, village,
				council_room, feast, mine, remodel, great_hall};
	
	// random number of players
	numPlayers = floor(Random()*MAX_PLAYERS);
	if (numPlayers<2) {
		numPlayers=2;
	}
	initializeGame(numPlayers, k, seed, &state);	
	//printf("hand %i, deck %i\n", numTestHand, numTestDeck);
	// random card types
	int random=1;
	if (random==1) {
	// random number of cards
	//state.handCount[cP] = (int)floor(Random()*6);
	//state.deckCount[cP] = (int)floor(Random()*6);
	state.handCount[cP] = numHand;
	state.deckCount[cP] = numDeck;
	for (i=0;i<state.handCount[cP]; i++) {
		state.hand[cP][i] = getCard((int)floor(Random()*17), k); // there are 10 kingom cards + 3 estate + curse + treasure
	}
	for (i=0;i<state.deckCount[cP]; i++) {
		state.deck[cP][i] = getCard((int)floor(Random()*17), k); // there are 10 kingom cards + 3 estate + curse + treasure
	}	
	}
	else {

	}
	state.hand[cP][0]=village;
	printHand(cP, &state);
	printDeck(cP, &state);
	printDiscard(cP, &state);
	memcpy(&origState, &state, sizeof(struct gameState));
	cardEffect(smithy, 0,0,0, &state, 0, 0);
	
	//playCard(0, 0,0,0, &state);
	
	printHand(cP, &state);
	printDeck(cP, &state);
	printDiscard(cP, &state);
	
	// oracle code
	// check +3 cards (draw 3 and discard 1, should give 2 more cards)
	numDrawn = state.handCount[cP] +1 - origState.handCount[cP];
	if (numDrawn != 3) {
		if(NOISY_TEST)
			printf("FAIL: did not receive exactly three cards into hand\n");
		globalFail=1;
	}


	// check that valid cards from deck were drawn
	for (i=0; i< 17; i++) {
		if (fullDeckCount(cP, i, &origState) != fullDeckCount(cP, i, &state)) {
			if (NOISY_TEST)
				printf("FAIL: cards not conserved during draw\n");
			globalFail=1;
		}
	}
	// check for no state change for other players
	for (i=0; i < numPlayers; i++) {
		if (i != cP) {
			// test hand
			for (j=0; j<state.handCount[i]; j++) {
				if (state.hand[i][j] != origState.hand[i][j]) {
					if (NOISY_TEST)
						printf("FAIL: state change for wrong player\n");
					globalFail=1;
				}
			}
			// test deck
			for (j=0; j<state.deckCount[i]; j++) {
				if (state.deck[i][j] != origState.deck[i][j]) {
					if (NOISY_TEST)
						printf("FAIL: state change for wrong player\n");
					globalFail=1;
				}
			}
			// test discard
			for (j=0; j<state.discardCount[i]; j++) {
				if (state.discard[i][j] != origState.discard[i][j]) {
					if (NOISY_TEST)
						printf("FAIL: state change for wrong player\n");
					globalFail=1;
				}
			}
		}
	}
	// check no state change for victory and kingdom cards
	for (i=0; i < card_count; i++) {
		if (state.supplyCount[i] != origState.supplyCount[i]) {
			if (NOISY_TEST)
				printf("FAIL: state change for supply cards (victory, kingdom)\n");
		}
	}
	

	return globalFail;
}


int main() {
	printf("Testing Smithy implementation...\n");
	
	//set up random number geSnerator
	SelectStream(1);
	PutSeed((long)time(NULL));	
	int i;
	int numRuns=1;
	int failed=0;
	int numHand = dispRand(MAX_HAND);
	int numDeck = dispRand(MAX_DECK);
	//dispRand();
	//dispRand();
	
	for (i=0; i< numRuns; i++) {
		numHand = dispRand(MAX_HAND);
		numDeck = dispRand(MAX_DECK);
		numHand = 5;
		numDeck = 5;
		if (testSmithy(numHand, numDeck)) {
			//printf("Smithy implementation FAILED\n");
			//i=numRuns;
			failed=1;
		}
	}

	if (failed) {
		printf("Smithy implementation FAILED\n");
	}
	else {
		printf("Smithy implementation PASSED\n");
	}
	printf("Testing Smithy implementation complete\n");
	
}

	