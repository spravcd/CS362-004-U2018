#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "dominion.h"
#include "rngs.h"

#define VERBOSE_TEST 0

int testfullDeckCount(int card, int numCardsTest) {
	struct gameState state;
	int seed=10;
	int numPlayers = MAX_PLAYERS;
	int cP = 0; // currentPlayer
	int i;
	int dummyCard;
	//int handSize = 500;
	int globalFail=0;
    int k[10] = {adventurer, smithy, sea_hag, gardens, village,
				council_room, feast, mine, remodel, great_hall};
	
	initializeGame(numPlayers, k, seed, &state);
	
	// parameters
	// deck size, cPlayer, which card, numCards
	
	// test all in deck
	state.deckCount[cP] = numCardsTest;
	for (i=0; i< numCardsTest; i++) {
		state.deck[cP][i] = card;
	}
	// first test if tere are no other cards
	state.handCount[cP] = 0;
	state.discardCount[cP] =0;
	if (fullDeckCount(cP, card, &state)  != numCardsTest) {
		globalFail =1;
		printf("FAIL: all cards in deck, no other cards\n");
	}
	// now test if there are other cards, but none of interest
	state.handCount[cP] = numCardsTest;
	state.discardCount[cP] =numCardsTest;	
	if (card == gardens) {
		dummyCard = village;
	}
	else {
		dummyCard = gardens;
	}
	for (i=0; i < numCardsTest; i++ ) {
		state.hand[cP][i] = dummyCard;
		state.discard[cP][i] = dummyCard;
	}
	if (fullDeckCount(cP, card, &state) != numCardsTest) {
		globalFail =1;
		printf("FAIL: all cards in deck, hand and discard with dummies\n");
	}	
	
	
	// test all in hand
	state.handCount[cP] = numCardsTest;
	state.discardCount[cP] =0;
	state.deckCount[cP] = 0;
	for (i=0; i< numCardsTest; i++) {
		state.hand[cP][i] = card;
	}
	if (fullDeckCount(cP, card, &state)  != numCardsTest) {
		globalFail =1;
		printf("FAIL: all cards in hand, no other cards\n");
	}
	// now test if there are other cards, but none of interest
	state.deckCount[cP] = numCardsTest;
	state.discardCount[cP] =numCardsTest;	
	if (card == gardens) {
		dummyCard = village;
	}
	else {
		dummyCard = gardens;
	}
	for (i=0; i < numCardsTest; i++ ) {
		state.deck[cP][i] = dummyCard;
		state.discard[cP][i] = dummyCard;
	}
	if (fullDeckCount(cP, card, &state) != numCardsTest) {
		globalFail =1;
		printf("FAIL: all cards in hand, deck and discard with dummies\n");
	}		
	
	
	// test all in discard
	state.handCount[cP] = 0;
	state.discardCount[cP] =numCardsTest;
	state.deckCount[cP] = 0;
	for (i=0; i< numCardsTest; i++) {
		state.discard[cP][i] = card;
	}
	if (fullDeckCount(cP, card, &state)  != numCardsTest) {
		globalFail =1;
		printf("FAIL: all cards in discard, no other cards\n");
	}
	// now test if there are other cards, but none of interest
	state.handCount[cP] = numCardsTest;
	state.deckCount[cP] =numCardsTest;	
	if (card == gardens) {
		dummyCard = village;
	}
	else {
		dummyCard = gardens;
	}
	for (i=0; i < numCardsTest; i++ ) {
		state.deck[cP][i] = dummyCard;
		state.hand[cP][i] = dummyCard;
	}
	if (fullDeckCount(cP, card, &state) != numCardsTest) {
		globalFail =1;
		printf("FAIL: all cards in discard, deck and hand with dummies\n");
	}			
	// test a mix of card locations
	state.handCount[cP] = floor(numCardsTest/3);
	state.discardCount[cP] =floor(numCardsTest/3);
	state.deckCount[cP] = numCardsTest - state.handCount[cP] - state.discardCount[cP];
	for (i=0; i< floor(numCardsTest/3); i++) {
		state.discard[cP][i] = card;
		state.hand[cP][i] = card;
	}
	for (i=0; i< state.deckCount[cP]; i++) {
		state.deck[cP][i] = card;
	}
	if (fullDeckCount(cP, card, &state)  != numCardsTest) {
		globalFail =1;
		printf("FAIL: mix of cards in hand/discard/deck, no other cards\n");
	}
	// leave those cards where they are, and fill the rest with a dummyCard
	state.handCount[cP] = numCardsTest;
	state.discardCount[cP] =numCardsTest;
	for (i=floor(numCardsTest/3); i< numCardsTest; i++) {
		state.discard[cP][i] = dummyCard;
		state.hand[cP][i] = dummyCard;
	}
	for (i = state.deckCount[cP]; i < numCardsTest;i++) {
		state.deck[cP][i] = dummyCard;
	}
	state.deckCount[cP] = numCardsTest;
	if (fullDeckCount(cP, card, &state)  != numCardsTest) {
		globalFail =1;
		printf("FAIL: mix of cards in hand/discard/deck, with dummy cards\n");
		printf("count: %i, deck: %i, hand: %i, discard: %i\n", fullDeckCount(cP, card, &state), (int)floor(numCardsTest/3), (int)floor(numCardsTest/3), (int)(numCardsTest - (int)2*floor(numCardsTest/3)));
	}
	
	// test no cards at all
	state.handCount[cP] = 0;
	state.discardCount[cP] =0;
	state.deckCount[cP] = 0;
	if (fullDeckCount(cP, card, &state)  != 0) {
		globalFail =1;
		printf("FAIL: no cards at all\n");
	}
	// now test with all dummy cards
	state.handCount[cP] = numCardsTest;
	state.discardCount[cP] =numCardsTest;
	state.deckCount[cP] = numCardsTest;	
	for (i=0; i< numCardsTest; i++) {
		state.hand[cP][i] = dummyCard;
		state.deck[cP][i] = dummyCard;
		state.discard[cP][i] = dummyCard;
	}
	if (fullDeckCount(cP, card, &state)  != 0) {
		globalFail =1;
		printf("FAIL: all dummy cards, no cards of interest\n");
	}
	
	return globalFail;
}

int main() {
	
	printf("Testing fullDeckCount function...\n");
	// test the corners, some generic card with a normal number
	// a generic card with max hand and deck numbers
	// an estate card with a normal number
	// a coin card with a normal number
	if (testfullDeckCount(sea_hag, 10)) {
		printf("fullDeckCount function FAILED with 10 sea hags\n");
	}
	else if(testfullDeckCount(sea_hag, MAX_DECK)) {
		printf("fullDeckCount function FAILED with MAX_DECK sea hags\n");

	}
	else if(testfullDeckCount(sea_hag, MAX_HAND)) {
		printf("fullDeckCount function FAILED with MAX_HAND sea hags\n");

	}
	else if(testfullDeckCount(estate, 10)) {
		printf("fullDeckCount function FAILED with 10 estates\n");

	}
	else if(testfullDeckCount(copper, 10)) {
		printf("fullDeckCount function FAILED with 10 coppers\n");

	}
	else {
		printf("fullDeckCount function PASSED\n");
	}
	printf("Testing fullDeckCount function complete\n");
}