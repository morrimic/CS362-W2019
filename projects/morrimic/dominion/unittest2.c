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

	printf("TESTING gainCard().\n");
	memset(&gameState, 23, sizeof(struct gameState));   // Clear Game State		
	initializeGame(numPlayer, cards, seed, &gameState); // Initialize New Game

	int supplyCountBefore[treasure_map + 1];
	int deckCountBeforeGain;
	int discardCountBeforeGain;
	int handCountBeforeGain;
	
	for (currentPlayer = 0; currentPlayer < 2; currentPlayer++) {
		printf("\n--- Running tests for player %d. ---\n", currentPlayer);

		/* Unsuccessful card gain tests. */

		printf("\nTest gaining a card that is not in play.\n");
		if (gainCard(sea_hag, &gameState, 0, currentPlayer) == -1) {
			printf("PASS - Correctly rejected attempt to gain card not in play.\n");
		}
		else {
			printf("FAIL - Failed to reject attempt to gain card not in play.\n");
			numFails++;
		}

		printf("\nTest gaining a card with a negative value.\n");
		if (gainCard(-1, &gameState, 0, currentPlayer) == -1) {
			printf("PASS - Correctly rejected attempt to gain card with negative value.\n");
		}
		else {
			printf("FAIL - Failed to reject attempt to gain card with negative value.\n");
			numFails++;
		}

		printf("\nTest gaining a card with a value too high to exist.\n");
		if (gainCard(999, &gameState, 0, currentPlayer) == -1) {
			printf("PASS - Correctly rejected attempt to gain card with value too high to exist.\n");
		}
		else {
			printf("FAIL - Failed to reject attempt to gain card with value too high to exist.\n");
			numFails++;
		}

		printf("\nTest gaining a card with zero supply remaining.\n");
		int backupEstateCount = gameState.supplyCount[estate];
		gameState.supplyCount[estate] = 0; // Set supply count for 'estate' to zero.
		if (gainCard(estate, &gameState, 0, currentPlayer) == -1) {
			printf("PASS - Correctly rejected attempt to gain estate card with zero supply remaining.\n");
		}
		else {
			printf("FAIL - Failed to reject attempt to gain card estate card with zero supply remaining.\n");
			numFails++;
		}
		gameState.supplyCount[estate] = backupEstateCount; // Restore supply count for 'estate'.
				
		/* Successful card gain tests. */

		/* TESTS FOR ADDING CARD TO DISCARD */
		printf("\nThe following tests are for a card being gained to the discard (flag == 0).\n");
		// Store Supply Counts Before Card Gain		
		for (int i = 0; i < (treasure_map + 1); i++) supplyCountBefore[i] = gameState.supplyCount[i];
		// Store Player Info Before Card Gain
		deckCountBeforeGain = gameState.deckCount[currentPlayer];
		discardCountBeforeGain = gameState.discardCount[currentPlayer];
		handCountBeforeGain = gameState.handCount[currentPlayer];
		// Gain estate card - card number is 1.
		gainCard(estate, &gameState, 0, currentPlayer);
		
		printf("\nTest supply counts are unchanged except for gained card.\n");
		int supplyFail = 0;
		for (int i = 0; i < (treasure_map + 1); i++) {
			if (i == estate) {
				if (gameState.supplyCount[i] != supplyCountBefore[i] - 1) supplyFail = -1;
			}
			else {
				if (gameState.supplyCount[i] != supplyCountBefore[i]) supplyFail = -1;
			}
		}
		if (supplyFail != -1) {
			printf("PASS - Supply is correct.\n");
		}
		else {
			printf("FAIL - Supply counts are incorrect.\n");
			numFails++;
		}
				
		printf("\nTest player discard count was increaed by one.\n");
		if (gameState.discardCount[currentPlayer] == (discardCountBeforeGain + 1)) {
			printf("PASS - Player discard count increased by one.\n");
		}
		else {
			printf("FAIL - Player discard count not increased by one.\n");
			numFails++;
		}

		printf("\nTest player deck count unchanged from card gain.\n");
		if (gameState.deckCount[currentPlayer] == deckCountBeforeGain) {
			printf("PASS - Current player deck count unchanged.\n");
		}
		else {
			printf("FAIL - Current player deck count not unchanged.\n");
			numFails++;
		}

		printf("\nTest player hand count unchanged from card gain.\n");
		if (gameState.handCount[currentPlayer] == handCountBeforeGain) {
			printf("PASS - Current player hand count unchanged.\n");
		}
		else {
			printf("FAIL - Current player hand count not unchanged.\n");
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

		/* TESTS FOR ADDING CARD TO DECK */
		printf("\nThe following tests are for a card being gained to the deck (flag == 1).\n");
		// Store Supply Counts Before Card Gain		
		for (int i = 0; i < (treasure_map + 1); i++) supplyCountBefore[i] = gameState.supplyCount[i];
		// Store Player Info Before Card Gain
		deckCountBeforeGain = gameState.deckCount[currentPlayer];
		discardCountBeforeGain = gameState.discardCount[currentPlayer];
		handCountBeforeGain = gameState.handCount[currentPlayer];
		// Gain estate card - card number is 1.
		gainCard(estate, &gameState, 1, currentPlayer);

		printf("\nTest supply counts are unchanged except for gained card.\n");
		supplyFail = 0;
		for (int i = 0; i < (treasure_map + 1); i++) {
			if (i == estate) {
				if (gameState.supplyCount[i] != supplyCountBefore[i] - 1) supplyFail = -1;
			}
			else {
				if (gameState.supplyCount[i] != supplyCountBefore[i]) supplyFail = -1;
			}
		}
		if (supplyFail != -1) {
			printf("PASS - Supply is correct.\n");
		}
		else {
			printf("FAIL - Supply counts are incorrect.\n");
			numFails++;
		}
		
		printf("\nTest player deck count increased by one from card buy.\n");
		if (gameState.deckCount[currentPlayer] == deckCountBeforeGain + 1) {
			printf("PASS - Current player deck count increased by one.\n");
		}
		else {
			printf("FAIL - Current player deck count not increased by one.\n");
			numFails++;
		}

		printf("\nTest player hand count unchanged from card gain.\n");
		if (gameState.handCount[currentPlayer] == handCountBeforeGain) {
			printf("PASS - Current player hand count unchanged.\n");
		}
		else {
			printf("FAIL - Current player hand count not unchanged.\n");
			numFails++;
		}

		printf("\nTest player discard count unchanged from card gain.\n");
		if (gameState.discardCount[currentPlayer] == discardCountBeforeGain) {
			printf("PASS - Current player discard count unchanged.\n");
		}
		else {
			printf("FAIL - Current player discard count not unchanged.\n");
			numFails++;
		}

		printf("\nTest new estate card was placed in deck.\n");
		if (gameState.deck[currentPlayer][gameState.deckCount[currentPlayer] - 1] == estate) {
			printf("PASS - Estate is added to deck in final deck position.\n");
		}
		else {
			printf("FAIL - Estate is not added to deck in final deck position.\n");
			numFails++;
		}

		/* TESTS FOR ADDING CARD TO HAND */
		printf("\nThe following tests are for a card being gained to the hand (flag == 2).\n");
		// Store Supply Counts Before Card Gain		
		for (int i = 0; i < (treasure_map + 1); i++) supplyCountBefore[i] = gameState.supplyCount[i];
		// Store Player Info Before Card Gain
		deckCountBeforeGain = gameState.deckCount[currentPlayer];
		discardCountBeforeGain = gameState.discardCount[currentPlayer];
		handCountBeforeGain = gameState.handCount[currentPlayer];
		// Gain estate card - card number is 1.
		gainCard(estate, &gameState, 2, currentPlayer);

		printf("\nTest supply counts are unchanged except for gained card.\n");
		supplyFail = 0;
		for (int i = 0; i < (treasure_map + 1); i++) {
			if (i == estate) {
				if (gameState.supplyCount[i] != supplyCountBefore[i] - 1) supplyFail = -1;
			}
			else {
				if (gameState.supplyCount[i] != supplyCountBefore[i]) supplyFail = -1;
			}
		}
		if (supplyFail != -1) {
			printf("PASS - Supply is correct.\n");
		}
		else {
			printf("FAIL - Supply counts are incorrect.\n");
			numFails++;
		}
		
		printf("\nTest player discard count unchanged from card gain.\n");
		if (gameState.discardCount[currentPlayer] == discardCountBeforeGain) {
			printf("PASS - Current player discard count unchanged.\n");
		}
		else {
			printf("FAIL - Current player discard count not unchanged.\n");
			numFails++;
		}

		printf("\nTest player deck count unchanged from card gain.\n");
		if (gameState.deckCount[currentPlayer] == deckCountBeforeGain) {
			printf("PASS - Current player deck count unchanged.\n");
		}
		else {
			printf("FAIL - Current player deck count not unchanged.\n");
			numFails++;
		}

		printf("\nTest player hand count increased by one from card gain.\n");
		if (gameState.handCount[currentPlayer] == handCountBeforeGain + 1) {
			printf("PASS - Current player hand count increased by one.\n");
		}
		else {
			printf("FAIL - Current player hand count not increased by one.\n");
			numFails++;
		}
				
		printf("\nTest new estate card was placed in hand.\n");
		if (gameState.hand[currentPlayer][gameState.handCount[currentPlayer] - 1] == estate) {
			printf("PASS - Estate is added to hand in final hand position.\n");
		}
		else {
			printf("FAIL - Estate is not added to hand in final hand position.\n");
			numFails++;
		}		

		endTurn(&gameState);
	}

	if (numFails == 0) printf("All tests passed!\n");
	else printf("There were %d test fails.\n", numFails);
	 
	return 0;
}
