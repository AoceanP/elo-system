// Aleksandar Panich
// Version 1.0

#include "RankingSystem.h"
#include "Match.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

/**
 * ADD PLAYER
 *
 * Adds a new player to the system
 * Checks if player already exists to prevent duplicates
 */
void RankingSystem::addPlayer(const std::string& name, double initialRating)
{
    /**
     * Step 1: Check if player already exists
     *
     * findPlayer returns nullptr if not found
     * So if we get a non-nullptr, player exists
     */
    if (findPlayer(name) != nullptr)
        {
        std::cout << "Player '" << name << "' already exists!\n";
        return;
    }

    /**
     * Step 2: Create new Player with smart pointer
     *
     * std::make_unique<Player>(name, initialRating)
     * - Creates a new Player object on the heap
     * - Wraps it in a unique_ptr for automatic cleanup
     * - Arguments are passed to Player's constructor
     */
    players.push_back(std::make_unique<Player>(name, initialRating));

    std::cout << "Player '" << name << "' added successfully!\n";
}

/**
 * FIND PLAYER
 *
 * Searches for a player by name
 * Uses linear search (O(n)) through the vector
 *
 * For small systems this is fine
 * For large systems, consider using std::map<string, unique_ptr<Player>>
 */
Player* RankingSystem::findPlayer(const std::string& name)
{
    /**
     * Range-based for loop: for (auto& item : container)
     *
     * auto& means "deduce the type and get a reference"
     * player is a unique_ptr<Player>&
     *
     * The & is important - we don't want to copy the unique_ptr!
     */
    for (auto& player : players)
        {
        /**
         * player->getName() accesses the Player object through the pointer
         * The -> operator is used because player is a pointer type
         */
        if (player->getName() == name)
            {
            /**
             * player.get() extracts the raw pointer from the unique_ptr
             * This returns a Player* without releasing ownership
             * The unique_ptr still owns the object
             */
            return player.get();
        }
    }
    return nullptr;
}

/**
 * RECORD MATCH
 *
 * Records a match between two players and updates their ratings
 *
 * This is the orchestration method that brings Player and Match together
 */
void RankingSystem::recordMatch(const std::string& name1, const std::string& name2, const int result)
{
    /**
     * Step 1: Find both players
     */
    Player* p1 = findPlayer(name1);
    Player* p2 = findPlayer(name2);

    /**
     * Step 2: Validate both players exist
     *
     * We need to check both before proceeding
     * Print helpful error message if either is missing
     */
    if (p1 == nullptr)
    {
        std::cout << "Player '" << name1 << "' not found!\n";
        return;
    }
    if (p2 == nullptr)
    {
        std::cout << "Player '" << name2 << "' not found!\n";
        return;
    }

    /**
     * Step 3: Create a Match object
     *
     * We dereference the pointers (*p1, *p2) because:
     * - Match constructor expects references (Player&)
     * - We have pointers (Player*)
     * - * converts pointer to reference
     */
    Match match(*p1, *p2, result);

    /**
     * Step 4: Process the match
     *
     * This runs the Elo formula and updates both players
     */
    match.processMatch();

    std::cout << "Match recorded successfully!\n";
}

/**
 * DISPLAY LEADERBOARD
 *
 * Shows all players sorted by rating (highest first)
 * Displays in a nice formatted table
 */
void RankingSystem::displayLeaderboard() const
{
    /**
     * Step 1: Check if there are any players
     */
    if (players.empty())
    {
        std::cout << "No players in the system.\n";
        return;
    }

    /**
     * Step 2: Create temporary vector of raw pointers
     *
     * We create a vector of raw pointers for sorting
     * This way we don't modify the main vector
     *
     * Why not sort players directly?
     * - unique_ptr can't be copied, only moved
     * - unique_ptr can't be used in certain algorithms
     * - Easier to sort raw pointers
     */
    std::vector<Player*> sortedPlayers;

    for (const auto& player : players)
    {
        /**
         * .get() extracts raw pointer from unique_ptr
         */
        sortedPlayers.push_back(player.get());
    }

    /**
     * Step 3: Sort by rating in descending order
     *
     * std::sort(begin, end, comparator)
     *
     * The comparator is a lambda function:
     * [](const Player* a, const Player* b) { ... }
     *
     * Lambda syntax: [captures](parameters) { body }
     * - [] empty captures (doesn't use outer variables)
     * - Takes two Player pointers
     * - Returns true if a should come before b
     *
     * We want descending order (highest rating first)
     * So: a->getRating() > b->getRating()
     */
    std::sort(sortedPlayers.begin(), sortedPlayers.end(),
        [](const Player* a, const Player* b)
        {
            return a->getRating() > b->getRating();
        });

    /**
     * Step 4: Display header
     */
    std::cout << "\n";
    std::cout << "========== LEADERBOARD ==========\n";
    std::cout << std::left
              << std::setw(20) << "Name"
              << std::setw(10) << "Rating"
              << std::setw(8) << "Games"
              << std::setw(6) << "Wins"
              << std::setw(6) << "Loses"
              << std::setw(6) << "Draws" << std::endl;

    /**
     * std::string(56, '-') creates a string of 56 dashes
     * Used to create a nice separator line
     */
    std::cout << std::string(56, '-') << std::endl;

    /**
     * Step 5: Display each player
     *
     * Call displayStats() on each player
     * This outputs one formatted line per player
     */
    for (const auto& player : sortedPlayers)
    {
        player->displayStats();
    }

    std::cout << "=================================\n\n";
}

/**
 * SAVE TO FILE
 *
 * Saves all player data to a CSV file
 * Format: Name,Rating,GamesPlayed,Wins,Losses,Draws
 */
void RankingSystem::saveToFile(const std::string& filename) const
{
    /**
     * Step 1: Open file for writing
     *
     * std::ofstream = output file stream
     * Creates or overwrites the file
     */
    std::ofstream file(filename);

    /**
     * Step 2: Check if file opened successfully
     *
     * If file can't be opened, print error
     * Common reasons: permission denied, invalid path
     */
    if (!file)
    {
        std::cout << "Error opening file for writing!\n";
        return;
    }

    /**
     * Step 3: Write each player as one line
     *
     * Format: Name,Rating,GamesPlayed,Wins,Losses,Draws
     *
     * Using << operator to send data to file stream
     * Just like std::cout but for files
     */
    for (const auto& player : players)
    {
        file << player->getName() << ","
             << player->getRating() << ","
             << player->getGamesPlayed() << ","
             << player->getWins() << ","
             << player->getLosses() << ","
             << player->getDraws() << "\n";
    }

    /**
     * Step 4: File automatically closes when it goes out of scope
     *
     * This is called RAII: Resource Acquisition Is Initialization
     * When the ifstream object is destroyed, it closes the file
     * No need for manual close()
     */
    std::cout << "Data saved to " << filename << "\n";
}

/**
 * LOAD FROM FILE
 *
 * Loads player data from a CSV file
 * Parses each line and reconstructs Player objects
 */
void RankingSystem::loadFromFile(const std::string& filename)
{
    /**
     * Step 1: Open file for reading
     *
     * std::ifstream = input file stream
     * If file doesn't exist, ifstream fails silently
     * We check with the if statement
     */
    std::ifstream file(filename);

    /**
     * Step 2: Check if file exists/opened
     *
     * If file doesn't exist, start with empty system
     * Don't crash, just print message and return
     */
    if (!file) {
        std::cout << "No existing data file found. Starting fresh.\n";
        return;
    }

    /**
     * Step 3: Clear existing players
     *
     * vector::clear() removes all elements
     * The unique_ptrs are destroyed, which deletes the Player objects
     * Automatic cleanup!
     */
    players.clear();

    /**
     * Step 4: Read file line by line
     *
     * std::getline(stream, line) reads until newline
     * Returns false when end of file is reached
     * This loop reads every line in the file
     */
    std::string line;
    while (std::getline(file, line))
        {
        /**
         * Step 5: Parse the CSV line
         *
         * Create a stringstream to parse the line
         * stringstream treats a string like a file for reading
         * This lets us parse comma-separated values
         */
        std::stringstream ss(line);

        /**
         * Variables to hold the parsed data
         */
        std::string name;
        double rating;
        int games, wins, losses, draws;

        /**
         * Parse comma-separated values
         *
         * std::getline(ss, name, ',') reads until it hits a comma
         * ss >> variable reads the next value
         * ss.ignore() skips the next character (the comma)
         */
        std::getline(ss, name, ',');
        ss >> rating;
        ss.ignore();
        ss >> games;
        ss.ignore();
        ss >> wins;
        ss.ignore();
        ss >> losses;
        ss.ignore();
        ss >> draws;

        /**
         * Step 6: Create new Player with loaded data
         */
        auto player = std::make_unique<Player>(name, rating);

        /**
         * Step 7: Reconstruct game history
         *
         * We loaded the final counts, now we need to replay
         * each game to set the counters correctly
         *
         * recordWin() increments both wins AND gamesPlayed
         * So we call it once for each win
         * Same for losses and draws
         */
        for (int i = 0; i < wins; i++)
        {
            player->recordWin();
        }
        for (int i = 0; i < losses; i++)
        {
            player->recordLoss();
        }
        for (int i = 0; i < draws; i++)
        {
            player->recordDraw();
        }

        /**
         * Step 8: Add to players vector
         *
         * std::move transfers ownership from player to vector
         * The unique_ptr is now in the vector
         */
        players.push_back(std::move(player));
    }

    std::cout << "Loaded " << players.size() << " players from " << filename << "\n";
}

/**
 * GET PLAYER COUNT
 *
 * Returns the number of players in the system
 */
size_t RankingSystem::getPlayerCount() const
{
    return players.size();
}

/**
 * GET ALL PLAYER NAMES
 *
 * Returns a vector of all player names in the system
 * Useful for displaying lists and selecting opponents
 */
std::vector<std::string> RankingSystem::getAllPlayerNames() const
{
    /**
     * Create an empty vector to hold the names
     */
    std::vector<std::string> names;

    /**
     * Loop through all players and extract their names
     */
    for (const auto& player : players)
    {
        /**
         * Get the name and add it to our vector
         * push_back adds an element to the end
         */
        names.push_back(player->getName());
    }

    return names;
}