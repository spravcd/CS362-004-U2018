#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "dominion.h"
#include "rngs.h"
#include "interface.h"

int main() {
	// unit test 1
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
	printf("\n\n");
	
	// unit test 2
	
	printf("Testing isGameOver function...\n");
	
	if (testIsGameOver()) {
		printf("isGameOver function FAILED\n");
	}
	else {
		printf("isGameOver function PASSED\n");
	}
	printf("Testing isGameOver function complete\n");
	
	printf("\n\n");

	// unit test 3
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
	printf("\n\n");
	
	// unit test 4
	printf("Testing getCost function...\n");
	if (testGetCost()) {
		printf("getCost function FAILED\n");
	}
	else {
		printf("getCost function PASSED\n");
	}
	printf("Testing getCost function complete\n");
	printf("\n\n");

	// card test 1
	printf("Testing Smithy implementation...\n");
	
	if (testSmithy()) {
		printf("Smithy implementation FAILED\n");
	}
	else {
		printf("Smithy implementation PASSED\n");
	}
	printf("Testing Smithy implementation complete\n");
	printf("\n\n");
	
	// card test 2
	printf("Testing Adventurer implementation...\n");
	
	if (testAdventurer()) {
		printf("Adventurer implementation FAILED\n");
	}
	else {
		printf("Adventurer implementation PASSED\n");
	}
	printf("Testing Adventurer implementation complete\n");
	printf("\n\n");
	
	// card test 3
	printf("Testing village implementation...\n");
	
	if (testVillage()) {
		printf("village implementation FAILED\n");
	}
	else {
		printf("village implementation PASSED\n");
	}
	printf("Testing village implementation complete\n");
	printf("\n\n");
	
	// card test 4
	printf("Testing Great Hall implementation...\n");
	
	if (testGreatHall()) {
		printf("Great Hall implementation FAILED\n");
	}
	else {
		printf("Great Hall implementation PASSED\n");
	}
	printf("Testing Great Hall implementation complete\n");
}