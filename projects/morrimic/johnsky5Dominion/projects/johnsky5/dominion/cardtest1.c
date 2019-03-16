#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "smithy"

int main() {
	int seed = 1000;
	int numPlayers = 2;
	int currentPlayer = 0;
	int otherPlayer = 1;
	int numFails = 0;
	int choice1 = 0;
	int choice2 = 0;
	int choice3 = 0;
	int bonus = 0;
	int handpos = 0;
		
	struct gameState gameState, prevGameState;
	int k[10] = { adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room };

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &gameState);

	printf("----------------- Testing Card: %s ----------------\n", TESTCARD);
	
	// copy the game state to a prior state case
	memcpy(&prevGameState, &gameState, sizeof(struct gameState));
	// set hand position zero to smithy
	gameState.hand[currentPlayer][0] = smithy;
	// display initial player hand
	printf("Player hand before:\n");
	for (int i = 0; i < gameState.handCount[currentPlayer]; i++) printf("Card %d has value %d.\n", i, gameState.hand[currentPlayer][i]);
	// play card
	cardEffect(smithy, choice1, choice2, choice3, &gameState, handpos, &bonus);
	// display updated player hand
	printf("Player hand after:\n");
	for (int i = 0; i < gameState.handCount[currentPlayer]; i++) printf("Card %d has value %d.\n", i, gameState.hand[currentPlayer][i]);
	
	printf("\nTest that 'smithy' is no longer in hand position 0.\n");
	if (gameState.hand[currentPlayer][0] != smithy) {
		printf("PASS - Smithy is no longer in hand position 0.\n");
	}
	else {
		printf("FAIL - Smithy is still in hand position 0.\n");
		numFails++;
	}

	printf("\nTest that player hand count is now 2 higher than before (+3 from 'smithy' and -1 for discard.)\n");
	if (gameState.handCount[currentPlayer] == prevGameState.handCount[currentPlayer] + 2) {
		printf("PASS - Player hand count is correct.\n");
	}
	else {
		printf("FAIL - Player hand count is incorrect.\n");
		numFails++;
	}

	printf("\nTest that player discard count has increased by 1.\n");
	if (gameState.discardCount[currentPlayer] == prevGameState.discardCount[currentPlayer] + 1) {
		printf("PASS - Player discard count is correct.\n");
	}
	else {
		printf("FAIL - Player discard count is incorrect.\n");
		numFails++;
	}

	printf("\nTest that smithy is the top card in the discard.\n");
	if (gameState.discard[currentPlayer][gameState.discardCount[currentPlayer] - 1] == smithy) {
		printf("PASS - Smithy is top of discard pile.\n");
	}
	else {
		printf("FAIL - Smithy is not top of discard pile.\n");
		numFails++;
	}
	
	/*	
		Note - This is not a unit test of the shuffle/card draw methods and as such we are calling smithy 
		knowing >= 3 cards remain in the deck and assuming that cardDraw() is functional or tested separately.
	*/
	printf("\nTest the player's deck count has decreased by 3 after playing smithy.\n");
	if (gameState.deckCount[currentPlayer] == prevGameState.deckCount[currentPlayer] - 3) {
		printf("PASS - Current player deck count decreased by three.\n");
	}
	else {
		printf("FAIL - Current player deck count not decreased by three.\n");
		numFails++;
	}
	
	printf("\nTest that the other player's hand is unchanged.\n");
	int otherHandUnchanged = 1;
	if (gameState.handCount[otherPlayer] != prevGameState.handCount[otherPlayer]) otherHandUnchanged = 0;
	for (int i = 0; i < gameState.handCount[otherPlayer]; i++) {
		if (gameState.hand[otherPlayer][i] != prevGameState.hand[otherPlayer][i]) otherHandUnchanged = 0;
	}
	if (otherHandUnchanged == 1) {
		printf("PASS - Other player hand unchanged.\n");
	}
	else {
		printf("FAIL - Other player hand has changed.\n");
		numFails++;
	}

	printf("\nTest the other players's deck count is unchanged.\n");
	if (gameState.deckCount[otherPlayer] == prevGameState.deckCount[otherPlayer]) {
		printf("PASS - Other player's deck count unchanged.\n");
	}
	else {
		printf("FAIL - Other player's deck count has changed.\n");
		numFails++;
	}

	printf("\nTest the other players's discard count is unchanged.\n");
	if (gameState.discardCount[otherPlayer] == prevGameState.discardCount[otherPlayer]) {
		printf("PASS - Other player's discard count unchanged.\n");
	}
	else {
		printf("FAIL - Other player's discard count has changed.\n");
		numFails++;
	}

	printf("\n >>>>> Testing complete for %s <<<<<\n\n", TESTCARD);
	if (numFails == 0) printf("All tests passed!\n");
	else printf("There were %d test fails.\n", numFails);

	return 0;
}