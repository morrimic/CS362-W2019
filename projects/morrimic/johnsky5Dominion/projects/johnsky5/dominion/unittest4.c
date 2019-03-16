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

	printf("TESTING isGameOver().\n");
	memset(&gameState, 23, sizeof(struct gameState));   // Clear Game State		
	initializeGame(numPlayer, cards, seed, &gameState); // Initialize New Game
	
	printf("\nTest game over upon new initialized game.\n");	
	if (isGameOver(&gameState) == 0) {
		printf("PASS - Correctly identified that game is not over.\n");
	}
	else {
		printf("FAIL - Incorrectly identified game over condition.\n");
		numFails++;
	}
	
	printf("\nTest province stack is empty.\n");
	int provinceCountBefore = gameState.supplyCount[province];
	gameState.supplyCount[province] = 0;
	if (isGameOver(&gameState) == 1) {
		printf("PASS - Correctly identified game over condition from province count.\n");
	}
	else {
		printf("FAIL - Incorrectly identified game over condition from province count.\n");
		numFails++;
	}
	gameState.supplyCount[province] = provinceCountBefore;

	printf("\nTest two empty supply piles for game over.\n");
	int estateCountBefore = gameState.supplyCount[estate];
	int duchyCountBefore = gameState.supplyCount[duchy];
	gameState.supplyCount[estate] = 0;
	gameState.supplyCount[duchy] = 0;
	if (isGameOver(&gameState) == 0) {
		printf("PASS - Correctly identified that game is not over.\n");
	}
	else {
		printf("FAIL - Incorrectly identified game over condition.\n");
		numFails++;
	}
	gameState.supplyCount[estate] = estateCountBefore;
	gameState.supplyCount[duchy] = duchyCountBefore;
	
	printf("\nTest smallest 3 card numbers in supply are empty.\n");
	int curseCountBefore = gameState.supplyCount[curse];
	estateCountBefore = gameState.supplyCount[estate];
	duchyCountBefore = gameState.supplyCount[duchy];	
	gameState.supplyCount[curse] = 0;
	gameState.supplyCount[estate] = 0;
	gameState.supplyCount[duchy] = 0;
	if (isGameOver(&gameState) == 1) {
		printf("PASS - Correctly identified game over condition.\n");
	}
	else {
		printf("FAIL - Failed to identify game over condition.\n");
		numFails++;
	}
	gameState.supplyCount[curse] = curseCountBefore;
	gameState.supplyCount[estate] = estateCountBefore;
	gameState.supplyCount[duchy] = duchyCountBefore;

	printf("\nTest largest 3 card numbers in supply are empty.\n");	
	int treasureMapCountBefore = gameState.supplyCount[treasure_map];
	int seaHagCountBefore = gameState.supplyCount[sea_hag];
	int salvagerCountBefore = gameState.supplyCount[salvager];
	gameState.supplyCount[treasure_map] = 0;
	gameState.supplyCount[sea_hag] = 0;
	gameState.supplyCount[salvager] = 0;
	if (isGameOver(&gameState) == 1) {
		printf("PASS - Correctly identified game over condition.\n");
	}
	else {
		printf("FAIL - Failed to identify game over condition.\n");
		numFails++;
	}
	gameState.supplyCount[treasure_map] = treasureMapCountBefore;
	gameState.supplyCount[sea_hag] = seaHagCountBefore;
	gameState.supplyCount[salvager] = salvagerCountBefore;
		
	if (numFails == 0) printf("All tests passed!\n");
	else printf("There were %d test fails.\n", numFails);

	return 0;
}
