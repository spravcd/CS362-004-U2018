#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "dominion.h"
#include "rngs.h"
#include "interface.h"

#define VERBOSE_TEST 0

int testGetCost() {
	int globalFail=0;
	int count = 0;
	
	// test to ensure that all the current cards return the correct cost value
	if (getCost(curse) != CURSE_COST) {
		printf("FAIL: curse\n");
		globalFail=1;
	}	
	count++;
	if (getCost(estate) != ESTATE_COST) {
		printf("FAIL: estate\n");
		globalFail=1;
	}
	count++;
	if (getCost(duchy) != DUCHY_COST) {
		printf("FAIL: duchy\n");
		globalFail=1;
	}
	count++;
	if (getCost(province) != PROVINCE_COST) {
		printf("FAIL: province\n");
		globalFail=1;
	}
	count++;
	if (getCost(copper) != COPPER_COST) {
		printf("FAIL: copper\n");
		globalFail=1;
	}
	count++;
	if (getCost(silver) != SILVER_COST) {
		printf("FAIL: silver\n");
		globalFail=1;
	}
	count++;
	if (getCost(gold) != GOLD_COST) {
		printf("FAIL: gold\n");
		globalFail=1;
	}
	count++;
	if (getCost(adventurer) != ADVENTURER_COST) {
		printf("FAIL: adventurer\n");
		globalFail=1;
	}
	count++;
	if (getCost(council_room) != COUNCIL_ROOM_COST) {
		printf("FAIL: council room\n");
		globalFail=1;
	}
	count++;
	if (getCost(feast) != FEAST_COST) {
		printf("FAIL: feast\n");
		globalFail=1;
	}
	count++;
	if (getCost(gardens) != GARDEN_COST) {
		printf("FAIL: gardens\n");
		globalFail=1;
	}
	count++;
	if (getCost(mine) != MINE_COST) {
		printf("FAIL: mine\n");
		globalFail=1;
	}
	count++;
	if (getCost(remodel) != REMODEL_COST) {
		printf("FAIL: remodel\n");
		globalFail=1;
	}
	count++;
	if (getCost(smithy) != SMITHY_COST) {
		printf("FAIL: smithy\n");
		globalFail=1;
	}
	count++;
	if (getCost(village) != VILLAGE_COST) {
		printf("FAIL: village\n");
		globalFail=1;
	}
	count++;
	if (getCost(baron) != BARON_COST) {
		printf("FAIL: baron\n");
		globalFail=1;
	}
	count++;
	if (getCost(great_hall) != GREAT_HALL_COST) {
		printf("FAIL: great_hall\n");
		globalFail=1;
	}
	count++;
	if (getCost(minion) != MINION_COST) {
		printf("FAIL: minion\n");
		globalFail=1;
	}
	count++;
	if (getCost(steward) != STEWARD_COST) {
		printf("FAIL: steward\n");
		globalFail=1;
	}
	count++;
	if (getCost(tribute) != TRIBUTE_COST) {
		printf("FAIL: tribute\n");
		globalFail=1;
	}
	count++;
	if (getCost(ambassador) != AMBASSADOR_COST) {
		printf("FAIL: ambassador\n");
		globalFail=1;
	}
	count++;
	if (getCost(cutpurse) != CUTPURSE_COST) {
		printf("FAIL: cutpurse\n");
		globalFail=1;
	}
	count++;
	if (getCost(embargo) != EMBARGO_COST) {
		printf("FAIL: embargo\n");
		globalFail=1;
	}
	count++;
	if (getCost(outpost) != OUTPOST_COST) {
		printf("FAIL: outpost\n");
		globalFail=1;
	}
	count++;
	if (getCost(salvager) != SALVAGER_COST) {
		printf("FAIL: salvager\n");
		globalFail=1;
	}
	count++;
	if (getCost(sea_hag) != SEA_HAG_COST) {
		printf("FAIL: sea_hag\n");
		globalFail=1;
	}
	count++;
	if (getCost(treasure_map) != TREASURE_MAP_COST) {
		printf("FAIL: treasure_map\n");
		globalFail=1;
	}
	count++;
	// test to ensure that an unknown card returns a -1
	if (getCost(card_count+1) != -1) {
		printf("FAIL: unknown card should return -1\n");
		globalFail=1;
	}
	// ensure that you've tested all the cards
	if (count != card_count) {
		printf("FAIL: unkown number of cards, potential failure\n");
		globalFail=1;
	}
	return globalFail;
}

/*
int main() {
	printf("Testing getCost function...\n");
	if (testGetCost()) {
		printf("getCost function FAILED\n");
	}
	else {
		printf("getCost function PASSED\n");
	}
	printf("Testing getCost function complete\n");
}
*/