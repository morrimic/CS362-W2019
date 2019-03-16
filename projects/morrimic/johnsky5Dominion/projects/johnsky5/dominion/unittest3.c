#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

int main() {
	int seed = 1000;
	int numPlayer = 2;	
	int numFails = 0;
	int cards[10] = { adventurer, council_room, feast, gardens, mine
			   , remodel, smithy, village, baron, great_hall };
	struct gameState gameState;

	printf("TESTING getCost().\n");
	memset(&gameState, 23, sizeof(struct gameState));   // Clear Game State		
	initializeGame(numPlayer, cards, seed, &gameState); // Initialize New Game
	
	printf("Test 'curse' card cost equals 0.\n");
	if (getCost(curse) == 0) {
		printf("PASS - Correct cost returned.\n");
	}
	else {
		printf("FAIL - Incorrect cost returned.\n");
	}

	printf("Test 'estate' card cost equals 2.\n");
	if (getCost(estate) == 2) {
		printf("PASS - Correct cost returned.\n");
	}
	else {
		printf("FAIL - Incorrect cost returned.\n");
		numFails++;
	}

	printf("Test 'duchy' card cost equals 5.\n");
	if (getCost(duchy) == 5) {
		printf("PASS - Correct cost returned.\n");
	}
	else {
		printf("FAIL - Incorrect cost returned.\n");
		numFails++;
	}

	printf("Test 'province' card cost equals 8.\n");
	if (getCost(province) == 8) {
		printf("PASS - Correct cost returned.\n");
	}
	else {
		printf("FAIL - Incorrect cost returned.\n");
		numFails++;
	}

	printf("Test 'copper' card cost equals 0.\n");
	if (getCost(copper) == 0) {
		printf("PASS - Correct cost returned.\n");
	}
	else {
		printf("FAIL - Incorrect cost returned.\n");
		numFails++;
	}

	printf("Test 'silver' card cost equals 3.\n");
	if (getCost(silver) == 3) {
		printf("PASS - Correct cost returned.\n");
	}
	else {
		printf("FAIL - Incorrect cost returned.\n");
		numFails++;
	}

	printf("Test 'gold' card cost equals 6.\n");
	if (getCost(gold) == 6) {
		printf("PASS - Correct cost returned.\n");
	}
	else {
		printf("FAIL - Incorrect cost returned.\n");
		numFails++;
	}

	printf("Test 'adventurer' card cost equals 6.\n");
	if (getCost(adventurer) == 6) {
		printf("PASS - Correct cost returned.\n");
	}
	else {
		printf("FAIL - Incorrect cost returned.\n");
		numFails++;
	}

	printf("Test 'council_room' card cost equals 5.\n");
	if (getCost(council_room) == 5) {
		printf("PASS - Correct cost returned.\n");
	}
	else {
		printf("FAIL - Incorrect cost returned.\n");
		numFails++;
	}

	printf("Test 'feast' card cost equals 4.\n");
	if (getCost(feast) == 4) {
		printf("PASS - Correct cost returned.\n");
	}
	else {
		printf("FAIL - Incorrect cost returned.\n");
		numFails++;
	}

	printf("Test 'gardens' card cost equals 4.\n");
	if (getCost(gardens) == 4) {
		printf("PASS - Correct cost returned.\n");
	}
	else {
		printf("FAIL - Incorrect cost returned.\n");
		numFails++;
	}

	printf("Test 'mine' card cost equals 5.\n");
	if (getCost(mine) == 5) {
		printf("PASS - Correct cost returned.\n");
	}
	else {
		printf("FAIL - Incorrect cost returned.\n");
		numFails++;
	}

	printf("Test 'remodel' card cost equals 4.\n");
	if (getCost(remodel) == 4) {
		printf("PASS - Correct cost returned.\n");
	}
	else {
		printf("FAIL - Incorrect cost returned.\n");
		numFails++;
	}

	printf("Test 'smithy' card cost equals 4.\n");
	if (getCost(smithy) == 4) {
		printf("PASS - Correct cost returned.\n");
	}
	else {
		printf("FAIL - Incorrect cost returned.\n");
		numFails++;
	}

	printf("Test 'village' card cost equals 3.\n");
	if (getCost(village) == 3) {
		printf("PASS - Correct cost returned.\n");
	}
	else {
		printf("FAIL - Incorrect cost returned.\n");
		numFails++;
	}

	printf("Test 'baron' card cost equals 4.\n");
	if (getCost(baron) == 4) {
		printf("PASS - Correct cost returned.\n");
	}
	else {
		printf("FAIL - Incorrect cost returned.\n");
		numFails++;
	}

	printf("Test 'great_hall' card cost equals 3.\n");
	if (getCost(great_hall) == 3) {
		printf("PASS - Correct cost returned.\n");
	}
	else {
		printf("FAIL - Incorrect cost returned.\n");
		numFails++;
	}

	printf("Test 'minion' card cost equals 5.\n");
	if (getCost(minion) == 5) {
		printf("PASS - Correct cost returned.\n");
	}
	else {
		printf("FAIL - Incorrect cost returned.\n");
		numFails++;
	}

	printf("Test 'steward' card cost equals 3.\n");
	if (getCost(steward) == 3) {
		printf("PASS - Correct cost returned.\n");
	}
	else {
		printf("FAIL - Incorrect cost returned.\n");
		numFails++;
	}

	printf("Test 'tribute' card cost equals 5.\n");
	if (getCost(tribute) == 5) {
		printf("PASS - Correct cost returned.\n");
	}
	else {
		printf("FAIL - Incorrect cost returned.\n");
		numFails++;
	}

	printf("Test 'ambassador' card cost equals 3.\n");
	if (getCost(ambassador) == 3) {
		printf("PASS - Correct cost returned.\n");
	}
	else {
		printf("FAIL - Incorrect cost returned.\n");
		numFails++;
	}

	printf("Test 'cutpurse' card cost equals 4.\n");
	if (getCost(cutpurse) == 4) {
		printf("PASS - Correct cost returned.\n");
	}
	else {
		printf("FAIL - Incorrect cost returned.\n");
		numFails++;
	}

	printf("Test 'embargo' card cost equals 2.\n");
	if (getCost(embargo) == 2) {
		printf("PASS - Correct cost returned.\n");
	}
	else {
		printf("FAIL - Incorrect cost returned.\n");
		numFails++;
	}

	printf("Test 'outpost' card cost equals 5.\n");
	if (getCost(outpost) == 5) {
		printf("PASS - Correct cost returned.\n");
	}
	else {
		printf("FAIL - Incorrect cost returned.\n");
		numFails++;
	}

	printf("Test 'salvager' card cost equals 4.\n");
	if (getCost(salvager) == 4) {
		printf("PASS - Correct cost returned.\n");
	}
	else {
		printf("FAIL - Incorrect cost returned.\n");
		numFails++;
	}

	printf("Test 'sea_hag' card cost equals 4.\n");
	if (getCost(sea_hag) == 4) {
		printf("PASS - Correct cost returned.\n");
	}
	else {
		printf("FAIL - Incorrect cost returned.\n");
		numFails++;
	}

	printf("Test 'treasure_map' card cost equals 4.\n");
	if (getCost(treasure_map) == 4) {
		printf("PASS - Correct cost returned.\n");
	}
	else {
		printf("FAIL - Incorrect cost returned.\n");
		numFails++;
	}

	printf("Test invalid card number returns -1 value.\n");
	if (getCost(999) == -1) {
		printf("PASS - Invalid card returns -1 value.\n");
	}
	else {
		printf("FAIL - Invalid card returns other than -1 value.\n");
		numFails++;
	}

	if (numFails == 0) printf("All tests passed!\n");
	else printf("There were %d test fails.\n", numFails);
	
	return 0;
}