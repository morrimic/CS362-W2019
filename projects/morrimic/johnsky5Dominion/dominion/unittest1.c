#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

int main() {	
	int seed = 1000;
	int numPlayer = 2;		
	int currentPlayer;
	int numFails = 0;	
	int cards[10] = { adventurer, council_room, feast, gardens, mine
			   , remodel, smithy, village, baron, great_hall };
	struct gameState gameState;
		
	printf("TESTING buyCard().\n");	
	memset(&gameState, 23, sizeof(struct gameState));   // Clear Game State		
	initializeGame(numPlayer, cards, seed, &gameState); // Re-initialize Game
		
	for (currentPlayer = 0; currentPlayer < 2; currentPlayer++) {
		printf("\n--- Running tests for player %d. ---\n", currentPlayer);

		/* Unsuccessful Buy Tests */
		printf("\nTest that gameState->whoseTurn returns appropriate value.\n");
		if (gameState.whoseTurn == currentPlayer) printf("PASS - Correctly returned current player.\n");
		else {
			printf("FAIL - Incorrect player returned from gameState->whoseTurn.\n");
			numFails++;
		}

		printf("\nTest buying a card with a negative value.\n");
		if (buyCard(-1, &gameState) == -1) {
			printf("PASS - Correctly rejected attempt to buy card with negative value.\n");
		}
		else {
			printf("FAIL - Failed to reject attempt to buy card with negative value.\n");
			numFails++;
		}

		/* Need to re-initialize game due to failure caused by above test. */
		memset(&gameState, 23, sizeof(struct gameState));   // Clear Game State		
		initializeGame(numPlayer, cards, seed, &gameState); // Re-initialize Game
		if (currentPlayer == 1) gameState.whoseTurn = 1; // Adjust game state for current player turn if necessary.

		printf("\nTest buying a card with a value too high to exist.\n");
		if (buyCard(999, &gameState) == -1) {
			printf("PASS - Correctly rejected attempt to buy card with value too high to exist.\n");
		}
		else {
			printf("FAIL - Failed to reject attempt to buy card with value too high to exist.\n");
			numFails++;
		}

		/* Need to re-initialize game due to failure caused by above test. */
		memset(&gameState, 23, sizeof(struct gameState));   // Clear Game State		
		initializeGame(numPlayer, cards, seed, &gameState); // Re-initialize Game
		if (currentPlayer == 1) gameState.whoseTurn = 1; // Adjust game state for current player turn if necessary.

		printf("\nTest attempting to buy with no 'buys' left.\n");
		gameState.numBuys = 0; // Set number of 'buys' for current player to zero.
		if (buyCard(1, &gameState) == -1) printf("PASS - Correctly rejected buy attempt.\n");
		else {
			printf("FAIL - Incorrectly allowed player to buy without 'buys' remaining.\n");
			numFails++;
		}
		gameState.numBuys = 1; // Return number of 'buys' for current player to one.

		printf("\nTest attempting to buy a card with no 'supply' left of that card.\n");
		int previousSupplyCount = gameState.supplyCount[1];
		gameState.supplyCount[1] = 0; // Set number of 'supply' for estate to zero. Note: Player will always be able to afford estate on turn one.
		if (buyCard(1, &gameState) == -1) printf("PASS - Correctly rejected buy attempt.\n");
		else {
			printf("FAIL - Incorrectly allowed player to buy without 'supply' remaining.\n");
			numFails++;
		}
		gameState.supplyCount[1] = previousSupplyCount; // Return number of 'supply' for estate to prior value.

		printf("\nTest attempting to buy with insufficient 'coins'.\n");
		// Note - card [3] is province, which can never be purchased on turn one due to cost.
		if (buyCard(3, &gameState) == -1) printf("PASS - Correctly rejected buy attempt.\n");
		else {
			printf("FAIL - Incorrectly allowed player to buy 'province' on turn one.\n");
			numFails++;
		}

		/* Successful Buy Tests */

		printf("\nThe following tests are for buying estate card as valid buy.\n"); // Note - There is always sufficient gold to buy an estate on turn 1.
		// Obtain Supply Counts Before Buy
		int supplyCountBefore[treasure_map + 1];
		for (int i = 0; i < (treasure_map + 1); i++) supplyCountBefore[i] = gameState.supplyCount[i];
		// Store Player Info Before Buy		
		int coinsBeforeBuy = gameState.coins;
		int numBuysBeforeBuy = gameState.numBuys;
		int discardCountBeforeBuy = gameState.discardCount[currentPlayer];
		// Buy estate card - card number is 1.
		buyCard(1, &gameState);

		printf("\nTest supply counts are unchanged except for purchased card.\n");
		int supplyFail = 0;
		for (int i = 0; i < (treasure_map + 1); i++) {
			if (i == 1) {
				if (gameState.supplyCount[i] != supplyCountBefore[i] - 1) supplyFail = -1;
			}
			else {
				if (gameState.supplyCount[i] != supplyCountBefore[i]) supplyFail = -1;
			}
		}
		if (supplyFail != -1) printf("PASS - Supply is correct.\n");
		else {
			printf("FAIL - Supply counts are incorrect.\n");
			numFails++;
		}
		
		printf("\nTest player coins reduced by buy amount.\n");
		if (gameState.coins == coinsBeforeBuy - getCost(1)) {
			printf("PASS - Coins reduced by cost of estate.\n");
		}
		else {
			printf("FAIL - Coins not reduced by cost of estate.\n");
			numFails++;
		}

		printf("\nTest player buys reduced by one.\n");
		if (gameState.numBuys == numBuysBeforeBuy - 1) {
			printf("PASS - Player buys reduced by one.\n");
		}
		else {
			printf("FAIL - Player buys not reduced by one.\n");
			numFails++;
		}

		printf("\nTest player discard count was increaed by one.\n");
		if (gameState.discardCount[currentPlayer] == (discardCountBeforeBuy + 1)) {
			printf("PASS - Player discard count increased by one.\n");
		}
		else {
			printf("FAIL - Player discard count not increased by one.\n");
			numFails++;
		}

		printf("\nTest new estate card was placed in discard.\n");
		if (gameState.discard[currentPlayer][gameState.discardCount[currentPlayer] - 1] == 1) {
			printf("PASS - Estate is top card in player discard.\n");
		}
		else {
			printf("FAIL - Estate is not top card in player discard.\n");
			numFails++;
		}
			
		endTurn(&gameState);		
	}
	
	if (numFails == 0) printf("All tests passed!\n");
	else printf("There were %d test fails.\n", numFails);

	return 0;
}