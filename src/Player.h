// Aleksandar Panich
// Version 1.0

#ifndef PLAYER_H
#define PLAYER_H

#include <string>

/**
 * Player Class
 *
 * The Player class represents a single competitor in the Elo rating system.
 * Each player has:
 * - A name and current rating
 * - Statistics including games played, wins, losses, draws
 * - Methods to update their rating and record game results
 *
 * Design Philosophy:
 * - Encapsulation: Private data with public methods to access it
 * - Responsibility: A Player is responsible for managing its own data
 * - Const-correctness: Methods that don't change data are marked const
 */
class Player
{

private:

    /**
     * Player's name as text
     * Example: "Alice"
     */
    std::string name;

    /**
     * Player's current Elo rating
     * Example: 1200.0
     */
    double rating;

    /**
     * How many total games has this player played
     * This should equal: wins + losses + draws
     */
    int gamesPlayed;

    /**
     * How many games did this player win?
     * Used to calculate win/loss ratio and statistics
     */
    int wins;

    /**
     * How many games did this player lose?
     * Used to calculate win/loss ratio and statistics
     */
    int losses;

    /**
     * How many games ended in a tie?
     * Some games might not have a clear winner
     */
    int draws;

public:

    /**
     * Parameters:
     *   name - The player's name as text
     *   rating - The starting rating, defaults to 1200.0 if not given
     *
     * The = 1200.0 is a DEFAULT VALUE
     *
     * Usage examples:
     *   Player alice{"Alice"};           Uses default rating 1200.0
     *   Player bob{"Bob", 1500.0};       Uses custom rating 1500.0
     *
     * The initializer list in Player.cpp will:
     * - Set this.name to the parameter name
     * - Set this.rating to the parameter rating
     * - Initialize gamesPlayed, wins, losses, draws to 0
     */
    Player(std::string  name, double rating = 1200.0);

    /**
     * Get the player's name
     *
     * Returns the player's name as text
     *
     * Example return values:
     *   "Alice"
     *   "Bob"
     */
    std::string getName() const;

    /**
     * Get the player's current Elo rating
     *
     * Returns the current rating as a decimal number
     *
     * Example return values:
     *   1245.5
     *   1200.0
     */
    double getRating() const;

    /**
     * Get the total number of games this player has played
     *
     * Returns total games as a whole number
     * This should equal wins plus losses plus draws
     *
     * Example return values:
     *   15
     *   0
     */
    int getGamesPlayed() const;

    /**
     * Get the number of games this player has won
     *
     * Returns win count as a whole number
     *
     * Example return values:
     *   10
     *   0
     */
    int getWins() const;

    /**
     * Get the number of games this player has lost
     *
     * Returns loss count as a whole number
     *
     * Example return values:
     *   3
     *   0
     */
    int getLosses() const;

    /**
     * Get the number of games this player has drawn
     *
     * Returns draw count as a whole number
     *
     * Example return values:
     *   2
     *   0
     */
    int getDraws() const;

    /**
     * Update the player's rating to a new value
     *
     * This is called by the Match class after processing a game
     * The Match class calculates the new rating using the Elo formula,
     * then calls this method to update it
     *
     * Parameter:
     *   newRating - The new rating value to set
     *
     * Example values for newRating:
     *   1250.5
     *   1190.0
     *
     * Called from Match.cpp like this:
     * player.updateRating(1250.5);
     */
    void updateRating(double newRating);

    /**
     * Record that this player won a game
     *
     * This method should:
     * - Increase the wins counter by 1
     * - Increase the gamesPlayed counter by 1
     *
     * Called from Match.cpp when player1 wins
     *
     * Usage example:
     *   player.recordWin();
     */
    void recordWin();

    /**
     * Record that this player lost a game
     *
     * This method should:
     * - Increase the losses counter by 1
     * - Increase the gamesPlayed counter by 1
     *
     * Called from Match.cpp when player2 wins (which means player1 loses)
     *
     * Usage example:
     *   player.recordLoss();
     */
    void recordLoss();

    /**
     * Record that this player drew a game
     *
     * This method should:
     * - Increase the draws counter by 1
     * - Increase the gamesPlayed counter by 1
     *
     * Called from Match.cpp when result equals 0 (tie)
     *
     * Usage example:
     *   player.recordDraw();
     */
    void recordDraw();

    /**
     * DISPLAY METHOD
     *
     * Display this player's statistics in a formatted table row
     *
     * Output format will look like this:
     * Name                 Rating    Games  Wins  Loses Draws
     * Alice                1245.5    15     10    3     2
     *
     * Called by RankingSystem to print the leaderboard table
     *
     * Uses std::cout to print to the console
     * Uses formatting tools to make columns line up nicely
     */
    void displayStats() const;

};

#endif