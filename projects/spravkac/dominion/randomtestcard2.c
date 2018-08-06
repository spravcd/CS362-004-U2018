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
#define PRINT_CARDS 0

// helper function to get the correct card easily
int getCard(int cardRef, int* kindomCards) {
	if (cardRef < 10) {
		return kindomCards[cardRef];
	}
	
	else {
		return cardRef - 10; // curse through gold
	}	
}

// helper rand function
int dispRand(int max) {
	return (int)floor(Random()*max);
}

// helper function similar to fullDeckCount but queries playedCards
int playedCardCount(struct gameState* state, int card) {
	int i;
	int count=0;
	for (i=0; i<state->playedCardCount; i++) {
		if (state->playedCards[i] == card) {
			count++;
		}
	}
	return count;
}

int testVillage(int numHand, int numDeck) {
	struct gameState state;
	struct gameState origState;
	int seed=10;
	int numPlayers = MAX_PLAYERS;
	int cP = 0; // currentPlayer
	int i,j;
	
	int globalFail=0;
	
    int k[10] = {adventurer, smithy, sea_hag, gardens, village,
				council_room, feast, mine, remodel, great_hall};
	
	// random number of players
	numPlayers = floor(Random()*MAX_PLAYERS);
	if (numPlayers<2) {
		numPlayers=2;
	}
	initializeGame(numPlayers, k, seed, &state);	

	// random card types
	state.handCount[cP] = numHand;
	state.deckCount[cP] = numDeck;
	for (i=0;i<state.handCount[cP]; i++) {
		state.hand[cP][i] = getCard((int)floor(Random()*17), k); // there are 10 kingom cards + 3 estate + curse + treasure
	}
	for (i=0;i<state.deckCount[cP]; i++) {
		state.deck[cP][i] = getCard((int)floor(Random()*17), k); // there are 10 kingom cards + 3 estate + curse + treasure
	}	
	state.hand[cP][0]=village;
	if (PRINT_CARDS) {
		printHand(cP, &state);
		printDeck(cP, &state);
		printDiscard(cP, &state);
		printPlayed(cP, &state);
	}
	memcpy(&origState, &state, sizeof(struct gameState));
	cardEffect(village, 0,0,0, &state, 0, 0);
	
	if (PRINT_CARDS) {
		printHand(cP, &state);
		printDeck(cP, &state);
		printDiscard(cP, &state);
		printPlayed(cP, &state);
	}
	
	// oracle code
	// check +1 cards (draw 1 and discard 1, should give equal ammounts)
	if (state.handCount[cP] != origState.handCount[cP]) {
		if(NOISY_TEST)
			printf("FAIL: did not receive exactly three cards into hand\n");
		globalFail=1;
	}
	// check addition of 1 card to hand from deck
	if (state.hand[cP][0] != origState.deck[cP][origState.deckCount[cP]-1]) {
		if(NOISY_TEST)
			printf("FAIL: did not draw first card from deck\n");
		globalFail=1;
	}
	// check additional +2 actions (although 1 was required to play the card only if using playCard func)
	if (state.numActions != origState.numActions+2) {
		if(NOISY_TEST)
			printf("FAIL: did not provide exactly 2 additional actions\n");
		globalFail=1;
	}
	// check that valid cards from deck were drawn
	for (i=0; i< 17; i++) {
		if (fullDeckCount(cP, i, &origState) != fullDeckCount(cP, i, &state) + playedCardCount(&state, i)) {
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
			globalFail=1;
		}
	}

	return globalFail;
}


int main() {
	printf("Testing Village implementation...\n");
	
	//set up random number geSnerator
	SelectStream(1);
	PutSeed((long)time(NULL));	
	int i;
	int numRuns=10000;
	int failed=0;
	int numHand = dispRand(MAX_HAND);
	int numDeck = dispRand(MAX_DECK);
	//dispRand();
	//dispRand();
	
	for (i=0; i< numRuns; i++) {
		numHand = dispRand(MAX_HAND);
		numDeck = dispRand(MAX_DECK);
		//numHand = 8;
		//numDeck = 8;
		if (testVillage(numHand, numDeck)) {
			//printf("Village implementation FAILED\n");
			//i=numRuns;
			failed=1;
		}
	}

	if (failed) {
		printf("Village implementation FAILED\n");
	}
	else {
		printf("Village implementation PASSED\n");
	}
	printf("Testing Village implementation complete\n");
	
}

	