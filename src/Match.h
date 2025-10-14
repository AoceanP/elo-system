// Aleksandar Panich
// Version 1.0

#ifndef MATCH_H
#define MATCH_H

#include "Player.h"

/**
 * Match Class
 *
 * Represents a single game/match between two players
 *
 * When a match finishes, we need to:
 * 1. Calculate what rating each player SHOULD have gotten
 * 2. Update both players with their new ratings
 * 3. Record the result for both players (win/loss/draw)
 *
 * The Match class handles all of this in one place
 * This follows the Single Responsibility Principle:
 * Match is responsible for match logic, nothing else
 */
class Match
{

private:

    /**
     * Reference to the first player in this match
     *
     * Why a reference instead of a pointer?
     * - References can't be null
     * - References can't be changed after creation
     * - Cleaner syntax
     */
    Player& player1;

    /**
     * Reference to the second player in this match
     */
    Player& player2;

    /**
     * The result of the match from player1's perspective
     *
     * Possible values:
     *   1 = player1 won
     *   0 = draw
     *  -1 = player2 won (player1 lost)
     *
     * We only store one perspective because if player1 won,
     * then player2 automatically lost
     */
    int result;

    /**
     * K-Factor: How much ratings change per game
     *
     * The Elo formula is:
     * New Rating = Old Rating + K * (Actual Score - Expected Score)
     *
     * K determines how volatile ratings are:
     * - K = 32 (standard for most players)
     * - K = 16 (more experienced players, more stable)
     * - K = 10 (top of their game, very conservative change)
     *
     * Higher K means bigger swings in rating per game
     * Lower K means smaller, more gradual changes
     */
    double kFactor;

    /**
     * It uses the Elo formula:
     * Expected = 1 / (1 + 10^((opponent_rating - player_rating) / 400))
     *
     * Parameters:
     *   ratingA - The rating of player A
     *   ratingB - The rating of player B
     *
     * Returns: Expected score between 0 and 1
     *
     * Why private?
     * - This is an implementation detail
     * - Only the Match class needs to call this
     * - External code doesn't need to know about it
     *
     * Example calculations:
     * If A = 1200, B = 1200: Expected = 0.5 (even match)
     * If A = 1600, B = 1200: Expected = 0.91 (strong favorite)
     * If A = 1200, B = 1600: Expected = 0.09 (huge underdog)
     */
    static double calculateExpectedScore(double ratingA, double ratingB) ;

public:

    /**
     * Parameters:
     *   p1 - Reference to the first player
     *   p2 - Reference to the second player
     *   result - The outcome of the match
     *            1 means p1 won
     *            0 means draw
     *           -1 means p2 won
     *   kFactor - How much ratings should change (default 32.0)
     *
     * The constructor stores these values but doesn't calculate anything yet
     * It just sets up the match data
     *
     * The actual calculation happens when processMatch() is called
     *
     * Usage example:
     *   Match m{alice, bob, 1, 32.0};  alice won, K=32
     *   Match m2{charlie, david, -1};   david won, K=default 32
     */
    Match(Player& p1, Player& p2, int result, double kFactor = 32.0);

    /**
     * It does these steps:
     * 1. Get current ratings of both players
     * 2. Calculate expected scores using the Elo formula
     * 3. Determine actual scores based on match result
     * 4. Calculate new ratings using the formula
     * 5. Update both players with their new ratings
     * 6. Record the result in each player's statistics
     *
     * After calling this method:
     * - Both players have updated ratings
     * - Both players have updated win/loss/draw counts
     * - The match is complete
     *
     * Usage example:
     *   Match m{alice, bob, 1};
     *   m.processMatch();  Now alice and bob are updated
     */
    void processMatch() const;
};

#endif