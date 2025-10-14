// Aleksandar Panich
// Version 1.0

#include "Match.h"
#include <cmath>

/**
 * Creates a new Match with references to two players and the result
 *
 * Uses initializer list to bind the references
 * We bind references to the actual Player objects passed in
 *
 * The constructor just sets up the data
 * It doesn't do any calculations
 */
Match::Match(Player& p1, Player& p2, const int result, const double kFactor)
    : player1(p1),
      player2(p2),
      result(result),
      kFactor(kFactor)
{
     // Constructor body is empty as the initializer list did all the work
}

/**
 * This implements the core Elo formula for expected score:
 * E_A = 1 / (1 + 10^((R_B - R_A) / 400))
 *
 * Where:
 * - E_A is the expected score for player A
 * - R_A is player A's rating
 * - R_B is player B's rating
 * - 400 is the scaling constant (standard in chess)
 *
 * What does this mean?
 *
 * If you have a 400-point rating advantage over someone:
 * - 10^(400/400) = 10^1 = 10
 * - 1 / (1 + 10) = 1/11 ≈ 0.09
 * - So the weaker player has only 9% expected win rate
 *
 * If ratings are equal:
 * - 10^(0/400) = 10^0 = 1
 * - 1 / (1 + 1) = 1/2 = 0.5
 * - So both players have 50% expected win rate
 *
 * Parameters:
 *   ratingA - The rating of player A
 *   ratingB - The rating of player B (the opponent)
 *
 * Returns: Expected score between 0 and 1
 */
double Match::calculateExpectedScore(double ratingA, double ratingB)
{
    /**
     * Step 1: Calculate the exponent
     * (ratingB - ratingA) / 400.0
     *
     * We subtract A from B, so:
     * - If B is stronger (higher rating), exponent is positive
     * - If A is stronger, exponent is negative
     */
    const double exponent = (ratingB - ratingA) / 400.0;

    /**
     * Step 2: Calculate 10 to that power
     *
     * The 10.0 must be a double (not int) for proper floating-point math
     */
    const double powerOfTen = pow(10.0, exponent);

    /**
     * Step 3: Apply the formula
     * E_A = 1 / (1 + 10^((R_B - R_A) / 400))
     */
    const double expectedScore = 1.0 / (1.0 + powerOfTen);

    return expectedScore;
}

/**
 * This is the main method that runs the entire Elo calculation
 * and updates both players
 *
 * Steps:
 * 1. Get current ratings
 * 2. Calculate expected scores
 * 3. Determine actual scores
 * 4. Calculate new ratings
 * 5. Update players
 */
void Match::processMatch() const
{
    /**
     * STEP 1: Get current ratings
     *
     * We need both players' current ratings to calculate expected scores
     */
    const double rating1 = player1.getRating();
    const double rating2 = player2.getRating();

    /**
     * STEP 2: Calculate expected scores
     *
     * What were the odds before the game started?
     *
     * Expected scores are inverses of each other
     * They add up to 1.0 because one player wins, one loses
     *
     * If player1 has 0.75 expected (75% chance to win),
     * then player2 has 0.25 expected (25% chance to win)
     */
    const double expected1 = calculateExpectedScore(rating1, rating2);
    const double expected2 = calculateExpectedScore(rating2, rating1);

    /**
     * STEP 3: Determine actual scores
     *
     * What actually happened in the game?
     * Convert the result into scores (0 to 1)
     *
     * Also record the result in each player's statistics
     */
    double actual1, actual2;

    if (result == 1)
        {
        /**
         * Player1 won
         * actual1 = 1.0 means player1 gets full credit
         * actual2 = 0.0 means player2 gets no credit
         */
        actual1 = 1.0;
        actual2 = 0.0;
        player1.recordWin();
        player2.recordLoss();
    }
    else if (result == -1)
        {
        /**
         * Player2 won (player1 lost)
         * actual1 = 0.0 means player1 gets no credit
         * actual2 = 1.0 means player2 gets full credit
         */
        actual1 = 0.0;
        actual2 = 1.0;
        player1.recordLoss();
        player2.recordWin();
    }
    else
        {
        /**
         * result == 0, it's a draw
         * Both players get half credit
         * 0.5 means "half a win"
         */
        actual1 = 0.5;
        actual2 = 0.5;
        player1.recordDraw();
        player2.recordDraw();
    }

    /**
     * STEP 4: Calculate new ratings
     *
     * Apply the Elo formula to both players:
     * New Rating = Old Rating + K * (Actual - Expected)
     *
     * The key insight is (Actual - Expected):
     * - If you WIN against someone you're expected to beat: small change
     * - If you WIN against someone stronger: big change
     * - If you LOSE to someone weaker: big rating drop
     * - If you LOSE to someone stronger: small drop
     *
     * This rewards beating favorites and punishes losses to underdogs
     */
    const double newRating1 = rating1 + kFactor * (actual1 - expected1);
    const double newRating2 = rating2 + kFactor * (actual2 - expected2);

    /**
     * STEP 5: Update both players
     *
     * Call updateRating to set the new ratings
     * This is the only way ratings change in the system
     */
    player1.updateRating(newRating1);
    player2.updateRating(newRating2);
}