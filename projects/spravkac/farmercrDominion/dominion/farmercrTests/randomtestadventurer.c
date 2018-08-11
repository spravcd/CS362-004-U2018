/******************************************************************************
* Program Name:	Assignment-4
* Author:			Craig Farmer
* Course:			CS 362 - 400, Summer 2018
* Date Created:	08/04/2018
* Last Modified:	08/05/2018
* Due Date:			08/05/2018
* File name:		randomtestadventurer.c
* Description: A random test for Adventurer card function.
* Websites consulted:
*	https://www.gnu.org/software/make/manual/make.html
******************************************************************************/

#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define TESTCARD "Adventurer"
#define RED_TEXT "\x1b[31m"
#define GREEN_TEXT "\x1b[32m"
#define RESET_TEXT "\x1b[0m"
#define MIN 3
#define ITERATIONS 10000

// variables to keep counts of various failures
int failedCardEffect = 0;
int failedShuffle = 0;
int failedCardDraw = 0;
int failedHandCount = 0;
int failedDeckCount = 0;
int failedDiscardCount = 0;
int failedTreasureCount = 0;

// assert function that will test various comparisons and increment the failure count variables
void assertResult(int expected1, int actual1, int expected2, int actual2, int caseType)
{

	switch (caseType)
	{
	case 1: //"Card Effect":
		if (expected1 != actual1)
		{
			failedCardEffect++;
		}
		break;
	case 2: //"Shuffle":
		if (expected1 == actual1 && expected2 <= actual2)
		{
			failedShuffle++;
		}
		break;
	case 3: //"Draw Card":
		if (expected1 == actual1 && expected2 != actual2)
		{
			failedCardDraw++;
		}
		break;
	case 4: //"Treasure Count":
		if (expected1 != actual1)
		{
			failedTreasureCount++;
		}
		break;
	case 5: //"Hand Count":
		if (expected1 != actual1)
		{
			failedHandCount++;
		}
		break;
	case 6: //"Deck Count":
		if (expected1 != actual1)
		{
			failedDeckCount++;
		}
		break;
	case 7: //"Discard Count":
		if (expected1 != actual1)
		{
			failedDiscardCount++;
		}
		break;
	default:
		break;
	}
}

// function that runs the test and baseline gameStates, comparing the results
void randomTestCard(int player, struct gameState *testGame)
{
	// initial variables
	int startTreasureCount = 0;
	int testTreasureCount = 0;
	int tempHand[MAX_HAND];
	int tempHandCounter = 0;
	int drawnTreasure = 0;
	int card = -1;
	int bonus = 0;
	int cardEffectResult;
	int shuffleDeckResult;
	int drawCardResult;
	int counter;

	struct gameState startGame;
	// copy the gameState to maintain a starting state for a baseline
	memcpy(&startGame, testGame, sizeof(struct gameState));

	// check that the card worked correctly
	cardEffectResult = cardEffect(adventurer, 0, 0, 0, testGame, 0, &bonus);
	assertResult(0, cardEffectResult, 0, 0, 1);

	// manually copy the test card's functions
	// loop that draws cards until 2 treasure cards are drawn and added to hand
	while (drawnTreasure < 2)
	{
		if (startGame.deckCount[player] < 1)
		{
			shuffleDeckResult = shuffle(player, &startGame);
			assertResult(-1, shuffleDeckResult, 1, startGame.deckCount[player], 2);
		}

		drawCardResult = drawCard(player, &startGame);
		assertResult(-1, drawCardResult, 0, startGame.deckCount[player], 3);

		card = startGame.hand[player][startGame.handCount[player] - 1];
		if (card == copper || card == silver || card == gold)
		{
			drawnTreasure++;
		}
		else
		{
			tempHand[tempHandCounter] = card;
			startGame.handCount[player]--;
			tempHandCounter++;
		}
	}

	// cleanup after treasure cards are drawn
	while (tempHandCounter - 1 >= 0)
	{
		startGame.discard[player][startGame.discardCount[player]++] = tempHand[tempHandCounter - 1];
		tempHandCounter--;
	}

	// get the count of treasure cards in the test gameState
	counter = 0;
	card = -1;
	while (counter < testGame->handCount[player])
	{
		card = testGame->hand[player][counter];
		if (card == copper || card == silver || card == gold)
		{
			testTreasureCount++;
		}
		counter++;
	}

	// get the count of treasure cards in the baseline gameState
	counter = 0;
	card = -1;
	while (counter < startGame.handCount[player])
	{
		card = startGame.hand[player][counter];
		if (card == copper || card == silver || card == gold)
		{
			startTreasureCount++;
		}
		counter++;
	}
	// compare results of various gameState statuses
	assertResult(startTreasureCount, testTreasureCount, 0, 0, 4);
	assertResult(startGame.handCount[player], testGame->handCount[player], 0, 0, 5);
	assertResult(startGame.deckCount[player], testGame->deckCount[player], 0, 0, 6);
	assertResult(startGame.discardCount[player], testGame->discardCount[player], 0, 0, 7);
}

int main()
{
	struct gameState testGame;
	int treasures[] = {copper, silver, gold};
	int numTreasures;
	int iterationCounter = 0;
	int counter;
	int player;
	int fails;
	int failCount;
	int passCount;
	srand(time(NULL));

	printf("\n\n----- Starting Random Testing of %s Card -----\n\n", TESTCARD);
	printf("   Running test for %d iterations\n", ITERATIONS);

	// loop that runs for number of declared iterations (10,000)
	while (iterationCounter < ITERATIONS)
	{
		for (counter = 0; counter < sizeof(struct gameState); counter++)
		{
			((char*)&testGame)[counter] = floor(Random() * 256);
		}
		
		// set random values for game
		player = floor(Random() * MAX_PLAYERS);
		testGame.deckCount[player] = floor(Random() * ((MAX_DECK - MIN) + 1) + MIN);
		numTreasures = floor(Random() *((testGame.deckCount[player] - MIN) + 1) + MIN);
		
		// ensure that there are at least 3 treasure cards in the deck
		counter = 0;
		while (counter < numTreasures)
		{
			testGame.deck[player][counter] = treasures[rand() % 3];
			counter++;
		}
		testGame.discardCount[player] = 0;
		testGame.handCount[player] = floor(Random() * ((MAX_HAND - MIN) + 1) + MIN);
		testGame.whoseTurn = player;

		randomTestCard(player, &testGame);

		iterationCounter++;
	}
	
	// log the results of the iterations
	fails = failedCardEffect + failedShuffle + failedCardDraw + failedHandCount + failedDeckCount + failedDiscardCount + failedTreasureCount;

	if (ITERATIONS - fails <= 0)
	{
		passCount = 0;
		failCount = ITERATIONS;
	}
	else
	{
		passCount = ITERATIONS - fails;
		failCount = fails;
	}

	printf("   Results of random tests\n");
	printf("   -----------------------\n");
	printf(GREEN_TEXT "      PASSED: %d\n" RESET_TEXT, passCount);
	printf(RED_TEXT "      FAILED: %d\n" RESET_TEXT, failCount);
	printf("   =======================\n");

	if (fails == 0)
	{
		printf(GREEN_TEXT "      ALL TESTS PASSED\n" RESET_TEXT);
	}
	else
	{
		printf(RED_TEXT "      FAILURES\n" RESET_TEXT);
		printf("     cardEffect(): %d\n", failedCardEffect);
		printf("        shuffle(): %d\n", failedShuffle);
		printf("       drawCard(): %d\n", failedCardDraw);
		printf("       hand count: %d\n", failedHandCount);
		printf("       deck count: %d\n", failedDeckCount);
		printf("    discard count: %d\n", failedDiscardCount);
		printf("   treasure count: %d\n", failedTreasureCount);
	}
	printf("\n----- %s Card Random Testing Complete -----\n\n", TESTCARD);
}
