// Aleksandar Panich
// Version 1.0

#include "../src/Match.h"
#include "../src/Player.h"
#include <iostream>
#include <cassert>
#include <cmath>

/**
 * TEST 1: Match Creation
 *
 * Verify that a Match object can be created with two players
 */
void testMatchCreation()
{
    std::cout << "Test 1: Match creation..." << std::endl;

    Player alice{"Alice", 1200.0};
    Player bob{"Bob", 1200.0};

    /**
     * Create a match where alice wins
     * K-factor = 32 (default)
     */
    Match match{alice, bob, 1};

    /**
     * Match should be created without errors
     * If we get here, test passes
     */
    std::cout << "  PASSED" << std::endl;
}

/**
 * TEST 2: Equal Rating Match - Winner Gains Points
 *
 * When two players have equal rating:
 * - Expected score for each = 0.5 (50% chance)
 * - Winner (actual = 1.0) gains: K * (1.0 - 0.5) = K * 0.5
 * - Loser (actual = 0.0) loses: K * (0.0 - 0.5) = K * -0.5
 *
 * With K=32:
 * - Winner gains 16 points
 * - Loser loses 16 points
 */
void testEqualRatingMatch()
{
    std::cout << "Test 2: Equal rating match..." << std::endl;

    Player alice{"Alice", 1200.0};
    Player bob{"Bob", 1200.0};

    double initialAlice = alice.getRating();
    double initialBob = bob.getRating();

    /**
     * Alice wins
     */
    Match match{alice, bob, 1, 32.0};
    match.processMatch();

    double finalAlice = alice.getRating();
    double finalBob = bob.getRating();

    /**
     * With equal ratings and K=32:
     * Winner should gain 16 points
     * Loser should lose 16 points
     */
    double expectedGain = 16.0;

    assert(std::abs(finalAlice - (initialAlice + expectedGain)) < 0.1);
    assert(std::abs(finalBob - (initialBob - expectedGain)) < 0.1);

    /**
     * Verify win/loss were recorded
     */
    assert(alice.getWins() == 1);
    assert(bob.getLosses() == 1);
    assert(alice.getGamesPlayed() == 1);
    assert(bob.getGamesPlayed() == 1);

    std::cout << "  PASSED" << std::endl;
}

/**
 * TEST 3: Favorite Wins - Small Gain
 *
 * When a higher-rated player beats a lower-rated player:
 * - The higher-rated player gains less
 * - This is the expected outcome
 */
void testFavoriteWins()
{
    std::cout << "Test 3: Favorite wins (small gain)..." << std::endl;

    /**
     * Alice is 400 points stronger than Bob
     * Expected: Alice should win ~91% of the time
     */
    Player alice{"Alice", 1600.0};
    Player bob{"Bob", 1200.0};

    double initialAlice = alice.getRating();

    /**
     * Alice wins (as expected)
     */
    Match match{alice, bob, 1, 32.0};
    match.processMatch();

    double finalAlice = alice.getRating();
    double gainAlice = finalAlice - initialAlice;

    /**
     * Alice should gain very little (around 2-3 points)
     * because winning was expected
     */
    assert(gainAlice > 0 && gainAlice < 10);

    std::cout << "  PASSED" << std::endl;
}

/**
 * TEST 4: Upset Victory - Large Gain
 *
 * When a lower-rated player beats a higher-rated player:
 * - The lower-rated player gains a lot
 * - This is an upset, a surprise result
 */
void testUpsetVictory()
{
    std::cout << "Test 4: Upset victory (large gain)..." << std::endl;

    /**
     * Bob is 400 points weaker than Alice
     * But Bob wins (upset!)
     */
    Player alice{"Alice", 1600.0};
    Player bob{"Bob", 1200.0};

    double initialBob = bob.getRating();

    /**
     * Bob wins (upset)
     */
    Match match{alice, bob, -1, 32.0};
    match.processMatch();

    double finalBob = bob.getRating();
    double gainBob = finalBob - initialBob;

    /**
     * Bob should gain a lot (around 24-30 points)
     * because beating Alice was unexpected
     */
    assert(gainBob > 20 && gainBob < 32);

    std::cout << "  PASSED" << std::endl;
}

/**
 * TEST 5: Draw Match
 *
 * When players draw (tie):
 * - Both get 0.5 points (half a win)
 * - Higher-rated loses points (didn't win as expected)
 * - Lower-rated gains points (better than expected)
 */
void testDrawMatch()
{
    std::cout << "Test 5: Draw match..." << std::endl;

    Player alice{"Alice", 1200.0};
    Player bob{"Bob", 1200.0};

    double initialAlice = alice.getRating();
    double initialBob = bob.getRating();

    /**
     * Draw (result = 0)
     */
    Match match{alice, bob, 0, 32.0};
    match.processMatch();

    double finalAlice = alice.getRating();
    double finalBob = bob.getRating();

    /**
     * Both should stay at same rating (no change for equal match)
     */
    assert(std::abs(finalAlice - initialAlice) < 0.1);
    assert(std::abs(finalBob - initialBob) < 0.1);

    /**
     * Both should record a draw
     */
    assert(alice.getDraws() == 1);
    assert(bob.getDraws() == 1);

    std::cout << "  PASSED" << std::endl;
}

/**
 * TEST 6: Higher K-Factor (More Volatile)
 *
 * Higher K means bigger rating swings
 */
void testHigherKFactor()
{
    std::cout << "Test 6: Higher K-Factor (more volatile)..." << std::endl;

    Player alice1{"Alice1", 1200.0};
    Player bob1{"Bob1", 1200.0};

    Player alice2{"Alice2", 1200.0};
    Player bob2{"Bob2", 1200.0};

    /**
     * Match 1 with K=32
     */
    Match match1{alice1, bob1, 1, 32.0};
    match1.processMatch();
    double gain1 = alice1.getRating() - 1200.0;

    /**
     * Match 2 with K=64 (double)
     */
    Match match2{alice2, bob2, 1, 64.0};
    match2.processMatch();
    double gain2 = alice2.getRating() - 1200.0;

    /**
     * With K=64, gains should be roughly double
     */
    assert(std::abs(gain2 - 2 * gain1) < 1.0);

    std::cout << "  PASSED" << std::endl;
}

/**
 * TEST 7: Rating Clamped at Zero
 *
 * Even with negative results, rating doesn't go below 0
 */
void testRatingMinimum()
{
    std::cout << "Test 7: Rating clamped at minimum..." << std::endl;

    /**
     * Alice starts at a very low rating
     */
    Player alice{"Alice", 10.0};
    Player bob{"Bob", 2000.0};

    /**
     * Alice loses (expected)
     */
    Match match{alice, bob, -1, 32.0};
    match.processMatch();

    /**
     * Alice should not go below 0
     */
    assert(alice.getRating() >= 0.0);

    std::cout << "  PASSED" << std::endl;
}

/**
 * TEST 8: Multiple Matches Update Correctly
 *
 * Test that ratings accumulate across multiple matches
 */
void testMultipleMatches()
{
    std::cout << "Test 8: Multiple matches..." << std::endl;

    Player alice{"Alice", 1200.0};
    Player bob{"Bob", 1200.0};

    /**
     * Alice wins first match
     */
    Match match1{alice, bob, 1, 32.0};
    match1.processMatch();

    double afterMatch1 = alice.getRating();

    /**
     * Alice wins second match (against same opponent)
     * Opponent's rating dropped, so less gain
     */
    Match match2{alice, bob, 1, 32.0};
    match2.processMatch();

    double afterMatch2 = alice.getRating();

    /**
     * Both matches should update ratings
     */
    assert(afterMatch1 > 1200.0);
    assert(afterMatch2 > afterMatch1);

    /**
     * Games played should be 2
     */
    assert(alice.getGamesPlayed() == 2);
    assert(alice.getWins() == 2);

    std::cout << "  PASSED" << std::endl;
}

/**
 * TEST 9: Ratings are Zero-Sum
 *
 * Total rating gained = total rating lost
 * (Rating points are conserved, except for draws)
 */
void testZeroSumWithoutDraw()
{
    std::cout << "Test 9: Zero-sum ratings..." << std::endl;

    Player alice{"Alice", 1500.0};
    Player bob{"Bob", 1100.0};

    double totalBefore = alice.getRating() + bob.getRating();

    /**
     * Alice wins
     */
    Match match{alice, bob, 1, 32.0};
    match.processMatch();

    double totalAfter = alice.getRating() + bob.getRating();

    /**
     * Without draw, total points should be conserved
     */
    assert(std::abs(totalBefore - totalAfter) < 0.1);

    std::cout << "  PASSED" << std::endl;
}

/**
 * MAIN TEST RUNNER
 */
int main()
{
    std::cout << std::endl;
    std::cout << "Running Match Class Tests..." << std::endl;
    std::cout << std::endl;

    try
    {
        testMatchCreation();
        testEqualRatingMatch();
        testFavoriteWins();
        testUpsetVictory();
        testDrawMatch();
        testHigherKFactor();
        testRatingMinimum();
        testMultipleMatches();
        testZeroSumWithoutDraw();

        std::cout << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "All Match tests passed!" << std::endl;
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