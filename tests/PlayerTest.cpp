/**
 * PlayerTest.cpp
 *
 * Unit tests for the Player class
 * These tests verify that Player works correctly in all scenarios
 *
 * To compile and run these tests:
 * g++ -std=c++17 -o player_test PlayerTest.cpp Player.cpp
 * ./player_test
 *
 * This is a simple testing approach without external libraries
 * Each test is a function that checks if something is true
 * If all assertions pass, the test passes
 */

#include "../src/Player.h"
#include <iostream>
#include <cassert>

/**
 * ASSERTION HELPER
 *
 * assert checks if a condition is true
 * If it's false, the program stops and tells you which line failed
 * This helps us catch bugs immediately
 */

/**
 * TEST 1: Player Creation with Default Rating
 *
 * When we create a Player without specifying a rating,
 * it should default to 1200.0
 */
void testPlayerCreationDefault()
{
    std::cout << "Test 1: Player creation with default rating..." << std::endl;

    Player alice{"Alice"};

    assert(alice.getName() == "Alice");
    assert(alice.getRating() == 1200.0);
    assert(alice.getGamesPlayed() == 0);
    assert(alice.getWins() == 0);
    assert(alice.getLosses() == 0);
    assert(alice.getDraws() == 0);

    std::cout << "  PASSED" << std::endl;
}

/**
 * TEST 2: Player Creation with Custom Rating
 *
 * When we create a Player with a specific rating,
 * it should use that rating
 */
void testPlayerCreationCustom() {
    std::cout << "Test 2: Player creation with custom rating..." << std::endl;

    Player bob{"Bob", 1500.0};

    assert(bob.getName() == "Bob");
    assert(bob.getRating() == 1500.0);
    assert(bob.getGamesPlayed() == 0);
    assert(bob.getWins() == 0);
    assert(bob.getLosses() == 0);
    assert(bob.getDraws() == 0);

    std::cout << "  PASSED" << std::endl;
}

/**
 * TEST 3: Negative Rating Validation
 *
 * IMPORTANT TEST
 * If someone tries to create a Player with a negative rating,
 * it should be clamped to 0.0
 *
 * This is the main test for our validation feature
 */
void testNegativeRatingClamped()
{
    std::cout << "Test 3: Negative rating gets clamped to 0..." << std::endl;

    Player charlie{"Charlie", -100.0};

    /**
     * Even though we passed -100.0, the constructor should catch this
     * and set it to 0.0 instead
     */
    assert(charlie.getRating() == 0.0);

    std::cout << "  PASSED" << std::endl;
}

/**
 * TEST 4: Record Win
 *
 * When a player wins a game:
 * - Wins counter should increase by 1
 * - Games played should increase by 1
 * - Losses and draws should stay the same
 */
void testRecordWin()
{
    std::cout << "Test 4: Recording a win..." << std::endl;

    Player david{"David"};

    david.recordWin();

    assert(david.getWins() == 1);
    assert(david.getGamesPlayed() == 1);
    assert(david.getLosses() == 0);
    assert(david.getDraws() == 0);

    /**
     * Record another win to make sure it keeps incrementing
     */
    david.recordWin();

    assert(david.getWins() == 2);
    assert(david.getGamesPlayed() == 2);

    std::cout << "  PASSED" << std::endl;
}

/**
 * TEST 5: Record Loss
 *
 * When a player loses a game:
 * - Losses counter should increase by 1
 * - Games played should increase by 1
 * - Wins and draws should stay the same
 */
void testRecordLoss()
{
    std::cout << "Test 5: Recording a loss..." << std::endl;

    Player eve{"Eve"};

    eve.recordLoss();

    assert(eve.getLosses() == 1);
    assert(eve.getGamesPlayed() == 1);
    assert(eve.getWins() == 0);
    assert(eve.getDraws() == 0);

    /**
     * Record another loss to make sure it keeps incrementing
     */
    eve.recordLoss();

    assert(eve.getLosses() == 2);
    assert(eve.getGamesPlayed() == 2);

    std::cout << "  PASSED" << std::endl;
}

/**
 * TEST 6: Record Draw
 *
 * When a player draws a game:
 * - Draws counter should increase by 1
 * - Games played should increase by 1
 * - Wins and losses should stay the same
 */
void testRecordDraw()
{
    std::cout << "Test 6: Recording a draw..." << std::endl;

    Player frank{"Frank"};

    frank.recordDraw();

    assert(frank.getDraws() == 1);
    assert(frank.getGamesPlayed() == 1);
    assert(frank.getWins() == 0);
    assert(frank.getLosses() == 0);

    /**
     * Record another draw to make sure it keeps incrementing
     */
    frank.recordDraw();

    assert(frank.getDraws() == 2);
    assert(frank.getGamesPlayed() == 2);

    std::cout << "  PASSED" << std::endl;
}

/**
 * TEST 7: Mixed Game Results
 *
 * A player plays multiple games with different outcomes
 * Test that all counters work together correctly
 *
 * Example: 3 wins, 2 losses, 1 draw = 6 games total
 */
void testMixedResults()
{
    std::cout << "Test 7: Mixed game results..." << std::endl;

    Player grace{"Grace"};

    grace.recordWin();
    grace.recordWin();
    grace.recordWin();
    grace.recordLoss();
    grace.recordLoss();
    grace.recordDraw();

    assert(grace.getWins() == 3);
    assert(grace.getLosses() == 2);
    assert(grace.getDraws() == 1);
    assert(grace.getGamesPlayed() == 6);

    std::cout << "  PASSED" << std::endl;
}

/**
 * TEST 8: Update Rating
 *
 * The Match class will update a player's rating after each game
 * Test that updateRating correctly changes the rating
 */
void testUpdateRating()
{
    std::cout << "Test 8: Updating rating..." << std::endl;

    Player henry{"Henry", 1200.0};

    assert(henry.getRating() == 1200.0);

    /**
     * Simulate Match class updating the rating
     * After a win, rating might go up
     */
    henry.updateRating(1220.0);

    assert(henry.getRating() == 1220.0);

    /**
     * After a loss, rating might go down
     */
    henry.updateRating(1210.0);

    assert(henry.getRating() == 1210.0);

    std::cout << "  PASSED" << std::endl;
}

/**
 * TEST 9: Rating at Zero
 *
 * Test that a player can have a rating of exactly 0
 * and can climb back up with wins
 */
void testRatingAtZero()
{
    std::cout << "Test 9: Player at zero rating can win and climb..." << std::endl;

    /**
     * Create a player at zero rating
     */
    Player irene{"Irene", 0.0};

    assert(irene.getRating() == 0.0);

    /**
     * Even at zero, they can still record games
     */
    irene.recordWin();
    assert(irene.getWins() == 1);
    assert(irene.getGamesPlayed() == 1);

    /**
     * They can update their rating back up
     */
    irene.updateRating(50.0);
    assert(irene.getRating() == 50.0);

    std::cout << "  PASSED" << std::endl;
}

/**
 * TEST 10: Decimal Rating Precision
 *
 * Ratings can have decimal places (e.g., 1245.5)
 * Test that we preserve decimal precision
 */
void testDecimalRatingPrecision()
{
    std::cout << "Test 10: Decimal rating precision..." << std::endl;

    Player jack{"Jack", 1234.5};

    assert(jack.getRating() == 1234.5);

    jack.updateRating(1267.8);

    assert(jack.getRating() == 1267.8);

    std::cout << "  PASSED" << std::endl;
}

/**
 * MAIN TEST RUNNER
 *
 * This function runs all the tests
 * If any test fails, the program stops and tells you which one
 * If all pass, you'll see "All tests passed!"
 */
int main()
{
    std::cout << std::endl;
    std::cout << "Running Player Class Tests..." << std::endl;
    std::cout << std::endl;

    try
    {
        testPlayerCreationDefault();
        testPlayerCreationCustom();
        testNegativeRatingClamped();
        testRecordWin();
        testRecordLoss();
        testRecordDraw();
        testMixedResults();
        testUpdateRating();
        testRatingAtZero();
        testDecimalRatingPrecision();

        std::cout << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "All tests passed!" << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << std::endl;

        return 0;
    }
    catch (...)
    {
        std::cout << std::endl;
        std::cout << "TEST FAILED!" << std::endl;
        return 1;
    }
}