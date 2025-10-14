// Aleksandar Panich
// Version 1.0

#ifndef RANKINGSYSTEM_H
#define RANKINGSYSTEM_H

#include "Player.h"
#include <vector>
#include <string>
#include <memory>

/**
 * This class manages:
 * - A collection of all players in the system
 * - Creating and finding players
 * - Recording matches between players
 * - Displaying leaderboards
 * - Saving and loading data from files
 *
 * Design pattern: This is a "Manager" or "Controller" class
 * It provides a simple public interface for complex operations
 */
class RankingSystem
{

private:

    /**
     * - Have to manually delete (easy to forget = memory leak)
     * - Modern C++ best practice: avoid raw 'new' and 'delete'
     * - unique_ptr handles cleanup automatically
     */
    std::vector<std::unique_ptr<Player>> players;

public:

    /**
     * Add a new player to the system
     *
     * Parameters:
     *   name - The player's name (must be unique)
     *   initialRating - Starting Elo rating (default 1200)
     *
     * If a player with that name already exists, print error and do nothing
     */
    void addPlayer(const std::string& name, double initialRating = 1200.0);

    /**
     * Find a player by name
     *
     * Parameters:
     *   name - The name to search for
     *
     * Returns: Pointer to Player if found, nullptr if not found
     *
     * Important: Caller MUST check if return value is nullptr!
     * Example:
     *   Player* p = system.findPlayer("Alice");
     *   if (p != nullptr) {
     *       cout << p->getRating();
     *   }
     */
    Player* findPlayer(const std::string& name);

    /**
     * Record a match between two players
     *
     * Parameters:
     *   name1 - First player's name
     *   name2 - Second player's name
     *   result - Match outcome: 1 (name1 wins), 0 (draw), -1 (name2 wins)
     *
     * This method:
     * 1. Finds both players by name
     * 2. Creates a Match object
     * 3. Calls processMatch() to update ratings
     *
     * If either player is not found, print error and do nothing
     */
    void recordMatch(const std::string& name1, const std::string& name2, const int result);

    /**
     * Display all players sorted by rating highest first
     *
     * Shows: Name | Rating | Games | Wins | Losses | Draws
     *
     * Example output:
     * Name                 Rating    Games  Wins  Loses Draws
     * Alice                1245.5    15     10    3     2
     * Bob                  1210.0    12     7     4     1
     */
    void displayLeaderboard() const;

    /**
     * Save all player data to a file
     *
     * Format: CSV (Comma-Separated Values)
     * One player per line:
     * Name,Rating,GamesPlayed,Wins,Losses,Draws
     *
     * Example:
     * Alice,1245.5,15,10,3,2
     * Bob,1210.0,12,7,4,1
     *
     * Parameters:
     *   filename - Path to file to save
     *
     * This allows data to persist between program runs
     */
    void saveToFile(const std::string& filename) const;

    /**
     * Load all player data from a file
     *
     * Clears current players and loads from the CSV file
     * If file doesn't exist, starts with empty system
     *
     * Parameters:
     *   filename - Path to file to load
     *
     * This allows data to be restored from previous runs
     */
    void loadFromFile(const std::string& filename);

    /**
     * Get the number of players in the system
     *
     * Returns: Number of players as size_t
     *
     * Why size_t instead of int?
     * - size_t is unsigned (can't be negative)
     * - It's the return type of vector::size()
     * - Best practice for sizes and counts
     */
    size_t getPlayerCount() const;

    /**
     * Get all player names in the system
     *
     * Returns: A vector of all player names
     *
     * This is useful for:
     * - Finding available opponents
     * - Displaying player lists
     * - Random opponent selection
     */
    std::vector<std::string> getAllPlayerNames() const;

};

#endif