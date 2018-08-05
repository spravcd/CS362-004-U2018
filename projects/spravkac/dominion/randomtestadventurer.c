#include <stdio.h>
#include <stdlib.h>
/* 
random unit test for adventurer card 
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

int testAdventurer(int numHand, int numDeck) {
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
	for (i=0; i< numTestCards; i++) {
		state.hand[cP][i] = dummyHandCard;
		state.deck[cP][i] = dummyDeckCard;
	}
	state.hand[cP][0]=adventurer;
	state.deck[cP][numTestCards-2] = copper;
	state.deck[cP][numTestCards-1] = gold;

	}
	state.hand[cP][0]=adventurer;
	//printHand(cP, &state);
	//printDeck(cP, &state);
	//printDiscard(cP, &state);
	
	memcpy(&origState, &state, sizeof(struct gameState));
	cardEffect(adventurer, 0,0,0, &state, 0, 0);
	
	//printHand(cP, &state);
	//printDeck(cP, &state);
	//printDiscard(cP, &state);
	
	// oracle code
	// 2 treasure cards should be brought up from deck to hand
	i=0;
	while (treasureCount < 2 && i < origState.deckCount[cP]) {
		if (origState.deck[cP][i] == copper) {
			numCopper++;
		}
		else if (origState.deck[cP][i] == silver) {
			numSilver++;
			}
		else if (origState.deck[cP][i] == gold) {
			numGold++;
		}
		else {
			numDiscarded++;
		}
		treasureCount = numCopper + numSilver + numGold;
		i++;
	}
	// now you know how many cards should have been discarded and which treasures
	// should now be the hand!!
	if (numDiscarded + treasureCount != origState.deckCount[cP] - state.deckCount[cP]) {
		if (NOISY_TEST)
			printf("FAIL: wrong number of cards in deck, %i vs %i\n", numDiscarded + treasureCount, origState.deckCount[cP] - state.deckCount[cP]);
		globalFail=1;		
	}
	// make sure nothing else changed in the deck
	j=0;
	for (i=numDiscarded+ treasureCount; i < origState.deckCount[cP]; i++) {
		if (origState.deck[cP][i] != state.deck[cP][j]) {
			if (NOISY_TEST)
				printf("FAIL: incorrect state change to deck\n");
			globalFail=1;
			i = origState.deckCount[cP];
		}
		j++;
	}
		
	// adventurer brings 2 cards to the hand
	if (state.handCount[cP] != origState.handCount[cP]+2) {
		if (NOISY_TEST)
			printf("FAIL: wrong number of cards brought to hand\n");
		globalFail=1;			
	}
	// check to make sure correct cards are brought to hand
	int tempGold=0;
	int tempCopper=0;
	int tempSilver=0;
	for (i=state.handCount[cP]-treasureCount; i< state.handCount[cP]; i++) {
		if (state.hand[cP][i] == copper) {
			tempCopper++;
		}
		else if (state.hand[cP][i] == silver) {
			tempSilver++;
			}
		else if (state.hand[cP][i] == gold) {
			tempGold++;
		}		
	}
	if (tempCopper != numCopper) {
		if (NOISY_TEST)
			printf("FAIL: incorrect number of coppers in hand\n");
		globalFail=1;
	}
	if (tempSilver != numSilver) {
		if (NOISY_TEST)
			printf("FAIL: incorrect number of silvers in hand\n");
		globalFail=1;
	}
	if (tempGold != numGold) {
		if (NOISY_TEST)
			printf("FAIL: incorrect number of gold in hand\n");
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
						printf("FAIL: state change (hand) for wrong player\n");
					globalFail=1;
				}
			}
			// test deck
			for (j=0; j<state.deckCount[i]; j++) {
				if (state.deck[i][j] != origState.deck[i][j]) {
					if (NOISY_TEST)
						printf("FAIL: state change (deck) for wrong player\n");
					globalFail=1;
				}
			}
			// test discard
			for (j=0; j<state.discardCount[i]; j++) {
				if (state.discard[i][j] != origState.discard[i][j]) {
					if (NOISY_TEST)
						printf("FAIL: state change (discard) for wrong player\n");
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
	printf("Testing Adventurer implementation...\n");
	
	//set up random number generator
	SelectStream(1);
	PutSeed((long)time(NULL));	
	int i;
	int numRuns=2;
	int failed=0;
	int numHand, numDeck;
	
	for (i=0; i< numRuns; i++) {
		numHand = dispRand(MAX_HAND);
		numDeck = dispRand(MAX_DECK);
		//numHand=6;
		//numDeck=6;
		if (testAdventurer(numHand, numDeck)) {
			//printf("Adventurer implementation FAILED\n");
			//i=numRuns;
			failed=1;
		}
	}

	if (failed) {
		printf("Adventurer implementation FAILED\n");
	}
	else {
		printf("Adventurer implementation PASSED\n");
	}
	printf("Testing Adventurer implementation complete\n");
	
}

	