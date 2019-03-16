#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>
#include <time.h> 

#define TESTCARD "smithy"
#define NUMTESTS 10000

/* CAUTION: Enabling noisy test (1) will generate up to 10,000 failed test notifications. */
#define NOISYTEST 0

int main() {
	printf("----------------- Testing Card: %s ----------------\n", TESTCARD);
	// Random seeds
	int seed = 1000;
	srand(time(NULL));
	// Game Variables
	int numPlayers = 2;
	int currentPlayer = 0;
	int choice1 = 0;
	int choice2 = 0;
	int choice3 = 0;
	int handpos = 0;
	struct gameState gameState, prevGameState;
	int k[10] = { adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room };

	// Test Variables
	int numPass = 0;
	int numFails = 0;

	/*
		NOTE: There is a bug in adventurer that can break fully random tests. If there are not enough treasure cards
		in a players deck to draw 2, the card effect will run infinitely. To alleviate this, I am adding two copper
		to the deck at the beginning. This ensures that there will always be a minimum of two treasures. In a fully random
		test, there would be occasional infinite loops that flag this issue as existing.
	*/

	for (int i = 0; i < NUMTESTS; i++) {
		// Initialize a game state and player cards
		memset(&gameState, 23, sizeof(struct gameState));   // Clear Game State		
		initializeGame(numPlayers, k, seed, &gameState);
		currentPlayer = rand() % 2;

		// Randomly set hand.		
		for (int i = 0; i < 5; i++) {
			int randomGainCardNumber = rand() % 13;
			// Gain random card to deck
			if (randomGainCardNumber == 12) { gameState.hand[currentPlayer][i] = gold; }
			else if (randomGainCardNumber == 11) { gameState.hand[currentPlayer][i] = silver; }
			else if (randomGainCardNumber == 10) { gameState.hand[currentPlayer][i] = copper; }
			else { gameState.hand[currentPlayer][i] = k[randomGainCardNumber]; }
		}
		// Randomly choose hand position for adventurer.
		handpos = rand() % 5;
		gameState.hand[currentPlayer][handpos] = smithy;

		// Randomly gain up to 30 cards to deck.
		int numberRandomCardGain = rand() % 30;
		for (int i = 0; i < numberRandomCardGain; i++) {
			int randomGainCardNumber = rand() % 13;
			// Gain random card to deck
			if (randomGainCardNumber == 12) { gainCard(gold, &gameState, 1, currentPlayer); }
			else if (randomGainCardNumber == 11) { gainCard(silver, &gameState, 1, currentPlayer); }
			else if (randomGainCardNumber == 10) { gainCard(copper, &gameState, 1, currentPlayer); }
			else { gainCard(k[randomGainCardNumber], &gameState, 1, currentPlayer); }
		}

		// Shuffle Deck After Adding Cards
		shuffle(currentPlayer, &gameState);
		memcpy(&prevGameState, &gameState, sizeof(struct gameState));

		// Perform the Card Effect
		//cardEffect(adventurer, choice1, choice2, choice3, &gameState, handpos, &bonus);
		playCard(handpos, choice1, choice2, choice3, &gameState);

		// Compare Pre/Post Game States
		// Verify hand count increased by 2 (+3 card, -1 discard)
		if (gameState.handCount[currentPlayer] - 2 != prevGameState.handCount[currentPlayer]) {
			if (NOISYTEST) printf("Incorrect hand count.\n");
			numFails++;
		}
		// Verify discard count increased by 1.
		else if (gameState.discardCount[currentPlayer] != prevGameState.discardCount[currentPlayer]) {
			if (NOISYTEST) printf("Incorrect discard count.\n");
			numFails++;
		}
		// Verify smithy was discarded.
		else if (gameState.discard[currentPlayer][gameState.discardCount[currentPlayer] - 1] != smithy) {
			if (NOISYTEST) printf("Incorrect discard top card.\n");
			numFails++;
		}
		// Verify actions unchanged.
		else if (gameState.numActions != prevGameState.numActions) {
			if (NOISYTEST) printf("Incorrect player actions.\n");
			numFails++;
		}
		// Verify buys are unchanged.
		else if (gameState.numBuys != prevGameState.numBuys) {
			if (NOISYTEST) printf("Incorrect player buys.\n");
			numFails++;
		}
		else { numPass++; }
	}

	printf("\n >>>>> Testing complete for %s <<<<<\n\n", TESTCARD);
	if (numFails == 0) printf("All tests passed!\n");
	else {
		printf("There were %d test fails.\n", numFails);
		printf("There were %d test passes.\n", numPass);
	}

	return 0;
}