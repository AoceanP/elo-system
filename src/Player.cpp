// Aleksandar Panich
// Version 1.0

#include "Player.h"

#include <iostream>
#include <iomanip>
#include <utility>

/**
 * This is the code that runs when you create a new Player object
 * Example: Player alice{"Alice", 1200};
 *
 * The initializer list is the part after the colon
 * It looks like: : member1(value1), member2(value2), ...
 *
 * Why use initializer list instead of just assigning in the body?
 * - More efficient, especially for complex types like std::string
 * - Some member variables can ONLY be initialized this way
 * - It's the C++ best practice and what professional code does
 */
Player::Player(std::string  name, double rating)
    : name(std::move(name)),
      rating(rating),
      gamesPlayed(0),
      wins(0),
      losses(0),
      draws(0)
{
    /**
     * I wanted to have a check if rating somehow became negative and set it to 0
     * No error message is printed - it just happens in the background
     *
     * Why silently?
     * - The user doesn't need to know about internal validation
     * - This is a safety net that prevents impossible states
     *
     * A player can stay at 0 rating until they win games and climb back up
     * This can be tested in the tests directory to verify it works
     */
    if (this->rating < 0)
    {
        this->rating = 0.0;
    }
}

/**
 * The method body is just one line that returns the name
 */
std::string Player::getName() const
{
    return name;
}

/**
 * Returns the player's current rating
 */
double Player::getRating() const
{
    return rating;
}

/**
 * Returns the total number of games this player has played
 */
int Player::getGamesPlayed() const
{
    return gamesPlayed;
}

/**
 * Returns how many games this player has won
 */
int Player::getWins() const
{
    return wins;
}

/**
 * Returns how many games this player has lost
 */
int Player::getLosses() const
{
    return losses;
}

/**
 * Returns how many games this player has drawn (tied)
 */
int Player::getDraws() const
{
    return draws;
}

/**
 * Changes the player's rating to a new value
 *
 * This is called by the Match class after it calculates what the new rating should be
 * The Match class uses the Elo formula and passes us the result
 *
 * Parameter: newRating - the new rating value to set
 *
 * This does NOT have const at the end because it CHANGES the player and the value will be changed.
 */
void Player::updateRating(double newRating)
{
    rating = newRating;
}

/**
 * Called when this player wins a game
 *
 * Two things happen:
 * 1. wins++ increments the wins counter by 1
 * 2. gamesPlayed++ increments the games counter by 1
 *
 * This keeps our statistics consistent
 * If a player wins 5 games and loses 3 games, gamesPlayed should be 8
 */
void Player::recordWin()
{
    wins++;
    gamesPlayed++;
}

/**
 * Called when this player loses a game
 *
 * Two things happen:
 * 1. losses++ increments the losses counter by 1
 * 2. gamesPlayed++ increments the games counter by 1
 */
void Player::recordLoss()
{
    losses++;
    gamesPlayed++;
}

/**
 * Called when this player's game ends in a tie
 *
 * Two things happen:
 * 1. draws++ increments the draws counter by 1
 * 2. gamesPlayed++ increments the games counter by 1
 */
void Player::recordDraw()
{
    draws++;
    gamesPlayed++;
}

/**
 * Prints out this player's information in a nicely formatted way
 * This is used by RankingSystem to display the leaderboard table
 *
 * Output looks like:
 * Alice                1245.5    15     10    3     2
 *
 * Breaking down the formatting:
 * - std::left: Left-align text instead of right-align
 * - std::setw(20): Make each field 20 characters wide
 * - std::fixed: Use fixed-point notation for decimals
 * - std::setprecision(1): Show 1 decimal place for doubles
 * - std::endl: End the line and flush the output
 *
 * Think of it like creating columns in a table:
 * Column 1: Name (20 chars wide)
 * Column 2: Rating (10 chars wide) with 1 decimal place
 * Column 3: Games (8 chars wide)
 * Column 4: Wins (6 chars wide)
 * Column 5: Losses (6 chars wide)
 * Column 6: Draws (6 chars wide)
 */
void Player::displayStats() const
{
    std::cout << std::left << std::setw(20) << name
              /** Left-align the name in a 20-character wide column */

              << std::fixed << std::setprecision(1)
              /** Use fixed-point notation with 1 decimal place for the next number */

              << std::setw(10) << rating
              /** Print rating in a 10-character wide column */

              << std::setw(8) << gamesPlayed
              /** Print games played in an 8-character wide column */

              << std::setw(6) << wins
              /** Print wins in a 6-character wide column */

              << std::setw(6) << losses
              /** Print losses in a 6-character wide column */

              << std::setw(6) << draws << std::endl;
              /** Print draws in a 6-character wide column, then end the line */
}