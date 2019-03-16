#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "adventurer"

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
	memset(&gameState, 23, sizeof(struct gameState));   // Clear Game State		
	initializeGame(numPlayers, k, seed, &gameState);

	printf("----------------- Testing Card: %s ----------------\n", TESTCARD);

	// adding specific cards to top of deck for known test conditions
	gainCard(gold, &gameState, 1, currentPlayer);
	gainCard(estate, &gameState, 1, currentPlayer);
	gainCard(gold, &gameState, 1, currentPlayer);
	gainCard(estate, &gameState, 1, currentPlayer);
	gainCard(estate, &gameState, 1, currentPlayer);
	gainCard(estate, &gameState, 1, currentPlayer);
	gainCard(estate, &gameState, 1, currentPlayer);
	// when playing adventurer it is expected to cycle through 5 estates in the time it takes to reveal 2 golds
	// satisfactory end conditions are 2 golds added to hand and 5 estates added to discard for 7 total cards moved
	// the adventurer should then also be moved to the discard

	// copy the game state to a prior state case
	memcpy(&prevGameState, &gameState, sizeof(struct gameState));
	// set hand position zero to adventurer
	gameState.hand[currentPlayer][0] = adventurer;
	
	
	// display initial player info
	printf("Testing adventurer with estates and golds placed on top of deck.\n");
	printf("Player hand before:\n");
	for (int i = 0; i < gameState.handCount[currentPlayer]; i++) printf("Card %d has value %d.\n", i, gameState.hand[currentPlayer][i]);
	printf("Player deck count before: %d\n", gameState.deckCount[currentPlayer]);
	printf("Player discard count before: %d\n", gameState.discardCount[currentPlayer]);
	
	// play card
	cardEffect(adventurer, choice1, choice2, choice3, &gameState, handpos, &bonus);
	
	// display updated player info
	printf("Player hand after:\n");	
	for (int i = 0; i < gameState.handCount[currentPlayer]; i++) printf("Card %d has value %d.\n", i, gameState.hand[currentPlayer][i]);
	printf("Player deck count after: %d\n", gameState.deckCount[currentPlayer]);
	printf("Player discard count after: %d\n", gameState.discardCount[currentPlayer]);

	printf("\nTest that player discard count has increased by 6 (5 estate + 1 adventurer).\n");
	if (gameState.discardCount[currentPlayer] == prevGameState.discardCount[currentPlayer] + 6) {
		printf("PASS - Player discard count is correct.\n");
	}
	else {
		printf("FAIL - Player discard count is incorrect.\n");
		numFails++;
	}

	printf("\nTest that player discard has 1 adventurer and then 5 estates on top.\n");
	int correctDiscard = 1;
	for (int i = 0; i < 6; i++) {
		if (i == 0) {
			if (gameState.discard[currentPlayer][gameState.discardCount[currentPlayer] - i - 1] != adventurer) correctDiscard = 0;
		}
		else if (gameState.discard[currentPlayer][gameState.discardCount[currentPlayer] - i - 1] != estate) correctDiscard = 0;
	}
	if (correctDiscard == 1) {
		printf("PASS - Player discard contains 5 estates on top.\n");
	}
	else {
		printf("FAIL - Player discard is incorrect.\n");
		numFails++;
	}

	printf("\nTest that player hand count is now 1 higher than before (+2 golds from 'adventurer' and -1 for discard.)\n");
	if (gameState.handCount[currentPlayer] == prevGameState.handCount[currentPlayer] + 1) {
		printf("PASS - Player hand count is correct.\n");
	}
	else {
		printf("FAIL - Player hand count is incorrect.\n");
		numFails++;
	}

	printf("\nTest that first and last cards in player hand are golds (first because of discardCard() mechanic.\n");
	int twoGolds = 1;	
	if (gameState.hand[currentPlayer][gameState.handCount[currentPlayer] - 1] != gold) twoGolds = 0;
	if (gameState.hand[currentPlayer][0] != gold) twoGolds = 0;	
	if (twoGolds == 1) {
		printf("PASS - Player hand contains two new golds.\n");
	}
	else {
		printf("FAIL - Player hand does not contain two new golds.\n");
		numFails++;
	}

	printf("\nTest that 'adventurer' is no longer in hand position 0.\n");
	if (gameState.hand[currentPlayer][0] != adventurer) {
		printf("PASS - Adventurer is no longer in hand position 0.\n");
	}
	else {
		printf("FAIL - Adventurer is still in hand position 0.\n");
		numFails++;
	}

	/* Other Player Tests */

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
		
	/* Re-initialize game due to prior changes. */
	memset(&gameState, 23, sizeof(struct gameState));   // Clear Game State		
	initializeGame(numPlayers, k, seed, &gameState);
	// set hand position zero to adventurer
	gameState.hand[currentPlayer][0] = adventurer;
	// set rest of hand to estates
	for (int i = 1; i < 5; i++) gameState.hand[currentPlayer][i] = estate;
	/* Set player's entire remaining deck to estate cards so that two treasures cannot be revealed. */
	for (int i = 0; i < gameState.deckCount[currentPlayer]; i++) {
		gameState.deck[currentPlayer][i] = estate;
	}

	// display initial player info
	printf("\nTesting adventurer only estates remaining in player deck and hand.\n");
	printf("Player hand before:\n");
	for (int i = 0; i < gameState.handCount[currentPlayer]; i++) printf("Card %d has value %d.\n", i, gameState.hand[currentPlayer][i]);
	printf("Player deck count before: %d\n", gameState.deckCount[currentPlayer]);
	printf("Player discard count before: %d\n", gameState.discardCount[currentPlayer]);

	// play card
	cardEffect(adventurer, choice1, choice2, choice3, &gameState, handpos, &bonus);

	// display updated player info
	printf("Player hand after:\n");
	for (int i = 0; i < gameState.handCount[currentPlayer]; i++) printf("Card %d has value %d.\n", i, gameState.hand[currentPlayer][i]);
	printf("Player deck count after: %d\n", gameState.deckCount[currentPlayer]);
	printf("Player discard count after: %d\n", gameState.discardCount[currentPlayer]);

	printf("\n >>>>> Testing complete for %s <<<<<\n\n", TESTCARD);
	if (numFails == 0) printf("All tests passed!\n");
	else printf("There were %d test fails.\n", numFails);

	return 0;
}