// Aleksandar Panich
// Version 1.0

#include "RankingSystem.h"
#include <iostream>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <vector>

/**
 * DISPLAY MENU
 *
 * Shows the main menu to the user
 * Lists all available options
 */
void displayMenu()
{
    std::cout << "\n";
    std::cout << "===== ELO RANKING SYSTEM =====\n";
    std::cout << "1. Add Player\n";
    std::cout << "2. Find Match (Random Opponent)\n";
    std::cout << "3. Show Leaderboard\n";
    std::cout << "4. Save & Exit\n";
    std::cout << "5. Exit without Saving\n";
    std::cout << "==============================\n";
    std::cout << "Enter choice: ";
}

/**
 * CLEAR INPUT
 *
 * Clears the input buffer
 *
 * When you read an int with cin >> choice,
 * the newline character stays in the buffer
 * This causes problems for the next getline()
 *
 * This function clears that leftover newline
 */
void clearInput()
{
    /**
     * std::numeric_limits<std::streamsize>::max()
     * This is the largest possible number of characters
     * It essentially means "read everything until newline"
     *
     * '\n' is the stop character (the newline)
     */
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

/**
 * GET ALL PLAYERS EXCEPT ONE
 *
 * Returns a vector of all player names except the specified player
 * Used to find potential opponents for a match
 */
std::vector<std::string> getOtherPlayers(const RankingSystem& system, const std::string& playerName)
{
    /**
     * Step 1: Get all player names from the system
     */
    std::vector<std::string> allPlayers = system.getAllPlayerNames();

    /**
     * Step 2: Create a new vector for opponents (everyone except the player)
     */
    std::vector<std::string> others;

    /**
     * Step 3: Loop through all players and add them if they're not the specified player
     */
    for (const auto& name : allPlayers)
        {
        if (name != playerName)
            {
            others.push_back(name);
        }
    }

    return others;
}

/**
 * FIND RANDOM OPPONENT
 *
 * Given a list of player names, randomly select one
 */
std::string findRandomOpponent(const std::vector<std::string>& opponents) {
    /**
     * Check if there are any opponents available
     */
    if (opponents.empty())
        {
        return "";
    }

    /**
     * rand() % size gives a random index
     * This selects a random player from the list
     */
    int randomIndex = rand() % opponents.size();
    return opponents[randomIndex];
}

/**
 * MAIN FUNCTION
 *
 * Entry point of the program
 * Runs the main loop that handles user interaction
 */
int main()
{
    /**
     * Seed the random number generator
     * This makes rand() produce different results each time
     */
    srand(static_cast<unsigned>(time(0)));

    /**
     * Create the ranking system
     * This is our main object that manages everything
     */
    RankingSystem system;

    /**
     * File path for storing player data
     * Players will be saved here and loaded from here
     */
    const std::string filename = "../data/players.csv";

    /**
     * Load existing data from file
     * If file doesn't exist, starts with empty system
     * If file exists, restores all players
     */
    system.loadFromFile(filename);

    /**
     * Main loop control variable
     * Set to false to exit the program
     */
    bool running = true;

    /**
     * MAIN LOOP
     *
     * Keep running until user chooses to exit
     * Display menu, get choice, execute action
     */
    while (running)
        {
        /**
         * Show menu and get user's choice
         */
        displayMenu();
        int choice;
        std::cin >> choice;
        clearInput();

        /**
         * Process user's choice
         * Each case handles a different menu option
         */
        switch (choice)
            {

            case 1:
                {
                /**
                 * ADD PLAYER
                 *
                 * Get player name from user
                 * Use default rating of 1200
                 */
                std::string name;
                std::cout << "Enter player name: ";
                std::getline(std::cin, name);
                system.addPlayer(name);
                break;
            }

            case 2:
                {
                /**
                 * FIND MATCH (RANDOM OPPONENT)
                 *
                 * Get the player who wants to find a match
                 * Get list of other available players
                 * Randomly select one opponent
                 * Get the match result from user
                 * Record the match
                 */
                std::string playerName;
                std::cout << "Enter your name: ";
                std::getline(std::cin, playerName);

                /**
                 * Check if player exists
                 */
                if (system.findPlayer(playerName) == nullptr) {
                    std::cout << "Player '" << playerName << "' not found!\n";
                    break;
                }

                /**
                 * Get all available opponents (all players except the current player)
                 */
                std::vector<std::string> availableOpponents = getOtherPlayers(system, playerName);

                /**
                 * Check if there are any opponents available
                 */
                if (availableOpponents.empty())
                    {
                    std::cout << "No other players available! Add more players first.\n";
                    break;
                }

                /**
                 * Randomly select an opponent
                 */
                std::string opponent = findRandomOpponent(availableOpponents);
                std::cout << "Opponent found: " << opponent << "\n";

                /**
                 * Get match result from user
                 * 1 = player wins, 0 = draw, -1 = opponent wins
                 */
                int result;
                std::cout << "Enter result (1 = " << playerName << " wins, "
                          << "0 = draw, -1 = " << opponent << " wins): ";
                std::cin >> result;
                clearInput();

                /**
                 * Validate result and record match
                 */
                if (result != 1 && result != 0 && result != -1)
                {
                    std::cout << "Invalid result! Must be 1, 0, or -1\n";
                }
                else
                {
                    system.recordMatch(playerName, opponent, result);
                }
                break;
            }

            case 3:
                {
                /**
                 * SHOW LEADERBOARD
                 *
                 * Display all players sorted by rating
                 * Shows their statistics
                 */
                system.displayLeaderboard();
                break;
            }

            case 4:
                {
                /**
                 * SAVE AND EXIT
                 *
                 * Save all data to file
                 * Set running to false to exit loop
                 * Program terminates after loop
                 */
                system.saveToFile(filename);
                std::cout << "Goodbye!\n";
                running = false;
                break;
            }

            case 5:
                {
                /**
                 * EXIT WITHOUT SAVING
                 *
                 * Don't save data
                 * Set running to false to exit loop
                 * Any unsaved changes are lost
                 */
                std::cout << "Exiting without saving. Goodbye!\n";
                running = false;
                break;
            }

            default:
                {
                /**
                 * INVALID CHOICE
                 *
                 * User entered something not in the menu
                 * Print error and loop again
                 */
                std::cout << "Invalid choice! Please try again.\n";
                break;
            }
        }
    }
    return 0;
}