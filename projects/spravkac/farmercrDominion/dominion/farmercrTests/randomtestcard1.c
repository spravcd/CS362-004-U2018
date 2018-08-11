/******************************************************************************
* Program Name:	Assignment-4
* Author:			Craig Farmer
* Course:			CS 362 - 400, Summer 2018
* Date Created:	08/04/2018
* Last Modified:	08/05/2018
* Due Date:			08/05/2018
* File name:		randomtestcard2.c
* Description: A random test for Village card function.
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

#define TESTCARD "Village"
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
int failedActionCount = 0;

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
	case 8: //"Action Count":
		if (expected1 != actual1)
		{
			failedActionCount++;
		}
		break;
	default:
		break;
	}
}

// function that runs the test and baseline gameStates, comparing the results
void randomTestCard(int player, struct gameState *testGame)
{
	int drawCardResult;
	int bonus = 0;
	int cardEffectResult;

	struct gameState startGame;
	// copy the gameState to maintain a starting state for a baseline
	memcpy(&startGame, testGame, sizeof(struct gameState));

	// check that the card worked correctly
	cardEffectResult = cardEffect(village, 0, 0, 0, testGame, 0, &bonus);
	assertResult(0, cardEffectResult, 0, 0, 1);

	// manually copy the test card's functions
	// draw a card
	drawCardResult = drawCard(player, &startGame);
	assertResult(-1, drawCardResult, 0, startGame.deckCount[player], 3);

	// increase action count by 2
	startGame.numActions += 2;
	assertResult(startGame.numActions, testGame->numActions, 0, 0, 8);

	// discard the used Village card
	discardCard(0, player, &startGame, 0);
	assertResult(startGame.discardCount[player], testGame->discardCount[player], 0, 0, 7);

	// compare results of various gameState statuses
	assertResult(startGame.handCount[player], testGame->handCount[player], 0, 0, 5);
	assertResult(startGame.deckCount[player], testGame->deckCount[player], 0, 0, 6);
}

int main()
{
	struct gameState testGame;
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
		testGame.deckCount[player] = floor(Random() * MAX_DECK);
		testGame.playedCardCount = floor(Random() * (MAX_DECK - 1));
		testGame.discardCount[player] = floor(Random() * MAX_DECK);
		testGame.handCount[player] = floor(Random() * MAX_DECK);
		testGame.whoseTurn = player;

		randomTestCard(player, &testGame);

		iterationCounter++;
	}

	// log the results of the iterations
	fails = failedCardEffect + failedShuffle + failedCardDraw + failedHandCount + failedDeckCount + failedDiscardCount + failedTreasureCount + failedActionCount;

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
		printf("     action count: %d\n", failedActionCount);
	}
	printf("\n----- %s Card Random Testing Complete -----\n\n", TESTCARD);
}
