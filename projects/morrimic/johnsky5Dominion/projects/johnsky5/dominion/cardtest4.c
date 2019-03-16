#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "salvager"

int main() {
	int seed = 1000;
	int numPlayers = 2;
	int currentPlayer = 0;
	int otherPlayer = 1;
	int numFails = 0;
	int choice1;
	int choice2 = 0;
	int choice3 = 0;
	int bonus = 0;
	int handpos = 0;

	struct gameState gameState, prevGameState;
	int k[10] = { adventurer, embargo, village, minion, mine, salvager,
			sea_hag, tribute, smithy, council_room };

	printf("----------------- Testing Card: %s ----------------\n", TESTCARD);

	for (int i = 0; i < 5; i++) {
		// Set choice of card to trash
		choice1 = i;
		// Clear Game State
		memset(&gameState, 23, sizeof(struct gameState));
		// initialize a game state and player cards
		initializeGame(numPlayers, k, seed, &gameState);
		// copy the game state to a prior state case
		memcpy(&prevGameState, &gameState, sizeof(struct gameState));
		// set hand position zero to salvager
		gameState.hand[currentPlayer][0] = salvager;

		// display initial player info
		printf("\nTesting salvager attempting to trash hand position %d.\n", i);
		printf("Player hand before:\n");
		for (int j = 0; j < gameState.handCount[currentPlayer]; j++) printf("Card %d has value %d.\n", j, gameState.hand[currentPlayer][j]);
		printf("Player deck count before: %d\n", gameState.deckCount[currentPlayer]);
		printf("Player discard count before: %d\n", gameState.discardCount[currentPlayer]);
		printf("Player buys count before: %d\n", gameState.numBuys);
		printf("Player coins before: %d\n", gameState.coins);

		// play card
		cardEffect(salvager, choice1, choice2, choice3, &gameState, handpos, &bonus);
		
		// display updated player info
		printf("Player hand after:\n");
		for (int j = 0; j < gameState.handCount[currentPlayer]; j++) printf("Card %d has value %d.\n", j, gameState.hand[currentPlayer][j]);
		printf("Player deck count after: %d\n", gameState.deckCount[currentPlayer]);
		printf("Player discard count after: %d\n", gameState.discardCount[currentPlayer]);
		printf("Player buys count after: %d\n", gameState.numBuys);
		printf("Player coins after: %d\n", gameState.coins);

		printf("\nTest that 'salvager' is no longer in hand position 0.\n");
		if (gameState.hand[currentPlayer][0] != salvager) {
			printf("PASS - Salvager is no longer in hand position 0.\n");
		}
		else {
			printf("FAIL - Salvager is still in hand position 0.\n");
			numFails++;
		}

		printf("\nTest that player is not attempting to trash the salvager card being played.)\n");
		if (choice1 != 0) {
			printf("PASS - Player is not trashing salvager card being played.\n");
		}
		else {
			printf("FAIL - Player trashed salvager card being played.\n");
			numFails++;
		}

		printf("\nTest that player hand count is two less than before (-1 from salvager trash card and -1 for discard.)\n");
		if (gameState.handCount[currentPlayer] == prevGameState.handCount[currentPlayer] - 2) {
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

		printf("\nTest that salvager is the top card in the discard.\n");
		if (gameState.discard[currentPlayer][gameState.discardCount[currentPlayer] - 1] == salvager) {
			printf("PASS - Salvager is top of discard pile.\n");
		}
		else {
			printf("FAIL - Salvager is not top of discard pile.\n");
			numFails++;
		}

		printf("\nTest the player's deck count is unchanged.\n");
		if (gameState.deckCount[currentPlayer] == prevGameState.deckCount[currentPlayer]) {
			printf("PASS - Current player deck count is unchanged.\n");
		}
		else {
			printf("FAIL - Current player deck count is not unchanged.\n");
			numFails++;
		}
		
		printf("\nTest that player buys increased by 1.\n");
		if (gameState.numBuys == prevGameState.numBuys + 1) {
			printf("PASS - Player gained 1 buy from salvager.\n");
		}
		else {
			printf("FAIL - Player did not gain 1 buy from salvager.\n");
			numFails++;
		}

		printf("\nTest that player coins properly increased.\n");
		if (gameState.coins == prevGameState.coins + getCost(prevGameState.hand[currentPlayer][choice1])) {
			printf("PASS - Player gained right amount of coins.\n");
		}
		else {
			printf("FAIL - Player did not gain proper amount of coins.\n");
			numFails++;
		}
	}

	/* Testing trashing a card in position 0 that is not the salvager currently being played. */
	// Set choice of card to trash
	choice1 = 0;
	// Clear Game State
	memset(&gameState, 23, sizeof(struct gameState));
	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &gameState);
	// copy the game state to a prior state case
	memcpy(&prevGameState, &gameState, sizeof(struct gameState));
	// set hand position zero to salvager
	gameState.hand[currentPlayer][3] = salvager;
	handpos = 3;

	// display initial player info
	printf("\nTesting salvager attempting to trash hand position %d with salvager in hand position 3.\n", 0);
	printf("Player hand before:\n");
	for (int j = 0; j < gameState.handCount[currentPlayer]; j++) printf("Card %d has value %d.\n", j, gameState.hand[currentPlayer][j]);
	printf("Player deck count before: %d\n", gameState.deckCount[currentPlayer]);
	printf("Player discard count before: %d\n", gameState.discardCount[currentPlayer]);
	printf("Player buys count before: %d\n", gameState.numBuys);
	printf("Player coins before: %d\n", gameState.coins);

	// play card
	cardEffect(salvager, choice1, choice2, choice3, &gameState, handpos, &bonus);

	// display updated player info
	printf("Player hand after:\n");
	for (int j = 0; j < gameState.handCount[currentPlayer]; j++) printf("Card %d has value %d.\n", j, gameState.hand[currentPlayer][j]);
	printf("Player deck count after: %d\n", gameState.deckCount[currentPlayer]);
	printf("Player discard count after: %d\n", gameState.discardCount[currentPlayer]);
	printf("Player buys count after: %d\n", gameState.numBuys);
	printf("Player coins after: %d\n", gameState.coins);

	printf("\nTest that 'salvager' is no longer in hand position 3.\n");
	if (gameState.hand[currentPlayer][3] != salvager) {
		printf("PASS - Salvager is no longer in hand position 3.\n");
	}
	else {
		printf("FAIL - Salvager is still in hand position 3.\n");
		numFails++;
	}

	printf("\nTest that player is not attempting to trash the salvager card being played.)\n");
	if (choice1 != 3) {
		printf("PASS - Player is not trashing salvager card being played.\n");
	}
	else {
		printf("FAIL - Player trashed salvager card being played.\n");
		numFails++;
	}

	printf("\nTest that player hand count is two less than before (-1 from salvager trash card and -1 for discard.)\n");
	if (gameState.handCount[currentPlayer] == prevGameState.handCount[currentPlayer] - 2) {
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

	printf("\nTest that salvager is the top card in the discard.\n");
	if (gameState.discard[currentPlayer][gameState.discardCount[currentPlayer] - 1] == salvager) {
		printf("PASS - Salvager is top of discard pile.\n");
	}
	else {
		printf("FAIL - Salvager is not top of discard pile.\n");
		numFails++;
	}

	printf("\nTest the player's deck count is unchanged.\n");
	if (gameState.deckCount[currentPlayer] == prevGameState.deckCount[currentPlayer]) {
		printf("PASS - Current player deck count is unchanged.\n");
	}
	else {
		printf("FAIL - Current player deck count is not unchanged.\n");
		numFails++;
	}

	printf("\nTest that player buys increased by 1.\n");
	if (gameState.numBuys == prevGameState.numBuys + 1) {
		printf("PASS - Player gained 1 buy from salvager.\n");
	}
	else {
		printf("FAIL - Player did not gain 1 buy from salvager.\n");
		numFails++;
	}

	printf("\nTest that player coins properly increased.\n");
	if (gameState.coins == prevGameState.coins + getCost(prevGameState.hand[currentPlayer][choice1])) {
		printf("PASS - Player gained right amount of coins.\n");
	}
	else {
		printf("FAIL - Player did not gain proper amount of coins.\n");
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

	printf("\n >>>>> Testing complete for %s <<<<<\n\n", TESTCARD);
	if (numFails == 0) printf("All tests passed!\n");
	else printf("There were %d test fails.\n", numFails);

	return 0;
}