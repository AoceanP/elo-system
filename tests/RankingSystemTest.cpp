// Aleksandar Panich
// Version 1.0

#include "../src/RankingSystem.h"
#include <iostream>
#include <cassert>
#include <fstream>

/**
 * TEST 1: Create Empty System
 *
 * Verify that a new RankingSystem starts empty
 */
void testEmptySystem()
{
    std::cout << "Test 1: Empty system..." << std::endl;

    RankingSystem system;

    assert(system.getPlayerCount() == 0);

    std::cout << "  PASSED" << std::endl;
}

/**
 * TEST 2: Add Single Player
 *
 * Verify that a player can be added to the system
 */
void testAddSinglePlayer()
{
    std::cout << "Test 2: Add single player..." << std::endl;

    RankingSystem system;

    system.addPlayer("Alice");

    assert(system.getPlayerCount() == 1);

    /**
     * Find the player to verify they exist
     */
    Player* p = system.findPlayer("Alice");
    assert(p != nullptr);
    assert(p->getName() == "Alice");
    assert(p->getRating() == 1200.0);

    std::cout << "  PASSED" << std::endl;
}

/**
 * TEST 3: Add Multiple Players
 *
 * Verify that multiple players can be added
 */
void testAddMultiplePlayers()
{
    std::cout << "Test 3: Add multiple players..." << std::endl;

    RankingSystem system;

    system.addPlayer("Alice");
    system.addPlayer("Bob");
    system.addPlayer("Charlie");

    assert(system.getPlayerCount() == 3);

    assert(system.findPlayer("Alice") != nullptr);
    assert(system.findPlayer("Bob") != nullptr);
    assert(system.findPlayer("Charlie") != nullptr);

    std::cout << "  PASSED" << std::endl;
}

/**
 * TEST 4: Add Player with Custom Rating
 *
 * Verify that custom ratings are stored
 */
void testAddPlayerCustomRating()
{
    std::cout << "Test 4: Add player with custom rating..." << std::endl;

    RankingSystem system;

    system.addPlayer("Alice", 1500.0);

    Player* p = system.findPlayer("Alice");
    assert(p != nullptr);
    assert(p->getRating() == 1500.0);

    std::cout << "  PASSED" << std::endl;
}

/**
 * TEST 5: Prevent Duplicate Players
 *
 * Verify that duplicate player names are rejected
 */
void testNoDuplicates()
{
    std::cout << "Test 5: Prevent duplicate players..." << std::endl;

    RankingSystem system;

    system.addPlayer("Alice");
    system.addPlayer("Alice");

    /**
     * Should still only have 1 player
     */
    assert(system.getPlayerCount() == 1);

    std::cout << "  PASSED" << std::endl;
}

/**
 * TEST 6: Find Nonexistent Player
 *
 * Verify that findPlayer returns nullptr for nonexistent player
 */
void testFindNonexistent()
{
    std::cout << "Test 6: Find nonexistent player..." << std::endl;

    RankingSystem system;

    system.addPlayer("Alice");

    Player* p = system.findPlayer("Bob");
    assert(p == nullptr);

    std::cout << "  PASSED" << std::endl;
}

/**
 * TEST 7: Record Match Between Players
 *
 * Verify that a match updates both players' ratings
 */
void testRecordMatch()
{
    std::cout << "Test 7: Record match..." << std::endl;

    RankingSystem system;

    system.addPlayer("Alice", 1200.0);
    system.addPlayer("Bob", 1200.0);

    Player* alice = system.findPlayer("Alice");
    Player* bob = system.findPlayer("Bob");

    double initialAliceRating = alice->getRating();
    double initialBobRating = bob->getRating();

    /**
     * Record a match where Alice wins
     */
    system.recordMatch("Alice", "Bob", 1);

    /**
     * Ratings should have changed
     */
    assert(alice->getRating() > initialAliceRating);
    assert(bob->getRating() < initialBobRating);

    /**
     * Game counts should be updated
     */
    assert(alice->getWins() == 1);
    assert(bob->getLosses() == 1);

    std::cout << "  PASSED" << std::endl;
}

/**
 * TEST 8: Record Match with Nonexistent Player
 *
 * Verify that matching fails gracefully if player doesn't exist
 */
void testMatchNonexistent()
{
    std::cout << "Test 8: Record match with nonexistent player..." << std::endl;

    RankingSystem system;

    system.addPlayer("Alice");

    /**
     * Try to record match with nonexistent Bob
     * Should not crash, just print error
     */
    system.recordMatch("Alice", "Bob", 1);

    /**
     * Alice should not have any games recorded
     * (because match failed)
     */
    Player* alice = system.findPlayer("Alice");
    assert(alice->getGamesPlayed() == 0);

    std::cout << "  PASSED" << std::endl;
}

/**
 * TEST 9: Save to File
 *
 * Verify that player data is saved to CSV file
 */
void testSaveToFile()
{
    std::cout << "Test 9: Save to file..." << std::endl;

    RankingSystem system;

    system.addPlayer("Alice", 1234.5);
    system.addPlayer("Bob", 1100.0);

    /**
     * Record some matches to give them game history
     */
    system.recordMatch("Alice", "Bob", 1);

    /**
     * Save to test file
     */
    const std::string testFile = "test_players.csv";
    system.saveToFile(testFile);

    /**
     * Verify file was created and has content
     */
    std::ifstream file(testFile);
    assert(file.is_open());

    /**
     * Read first line and verify format
     */
    std::string line;
    std::getline(file, line);

    /**
     * Should contain at least one comma (CSV format)
     */
    assert(line.find(',') != std::string::npos);

    file.close();

    /**
     * Clean up test file
     */
    std::remove(testFile.c_str());

    std::cout << "  PASSED" << std::endl;
}

/**
 * TEST 10: Load from File
 *
 * Verify that player data is loaded from CSV file
 */
void testLoadFromFile()
{
    std::cout << "Test 10: Load from file..." << std::endl;

    /**
     * Create first system and save data
     */
    {
        RankingSystem system1;

        system1.addPlayer("Alice", 1250.0);
        system1.addPlayer("Bob", 1150.0);

        /**
         * Record a match
         */
        system1.recordMatch("Alice", "Bob", 1);

        /**
         * Save to test file
         */
        const std::string testFile = "test_load.csv";
        system1.saveToFile(testFile);
    }

    /**
     * Create second system and load data
     */
    {
        RankingSystem system2;

        const std::string testFile = "test_load.csv";
        system2.loadFromFile(testFile);

        /**
         * Verify data was loaded
         */
        assert(system2.getPlayerCount() == 2);

        Player* alice = system2.findPlayer("Alice");
        Player* bob = system2.findPlayer("Bob");

        assert(alice != nullptr);
        assert(bob != nullptr);

        /**
         * Verify game history was restored
         */
        assert(alice->getWins() == 1);
        assert(bob->getLosses() == 1);

        /**
         * Clean up test file
         */
        std::remove(testFile.c_str());
    }

    std::cout << "  PASSED" << std::endl;
}

/**
 * TEST 11: Multiple Matches in System
 *
 * Verify that system correctly handles multiple matches
 */
void testMultipleMatchesInSystem()
{
    std::cout << "Test 11: Multiple matches in system..." << std::endl;

    RankingSystem system;

    system.addPlayer("Alice");
    system.addPlayer("Bob");
    system.addPlayer("Charlie");

    /**
     * Record several matches
     */
    system.recordMatch("Alice", "Bob", 1);
    system.recordMatch("Bob", "Charlie", 1);
    system.recordMatch("Alice", "Charlie", -1);

    /**
     * Verify game counts
     */
    Player* alice = system.findPlayer("Alice");
    Player* bob = system.findPlayer("Bob");
    Player* charlie = system.findPlayer("Charlie");

    assert(alice->getGamesPlayed() == 2);
    assert(bob->getGamesPlayed() == 2);
    assert(charlie->getGamesPlayed() == 2);

    std::cout << "  PASSED" << std::endl;
}

/**
 * TEST 12: Empty System Load
 *
 * Verify that loading from nonexistent file creates empty system
 */
void testLoadNonexistent()
{
    std::cout << "Test 12: Load from nonexistent file..." << std::endl;

    RankingSystem system;

    /**
     * Try to load from file that doesn't exist
     */
    system.loadFromFile("nonexistent_file.csv");

    /**
     * System should be empty
     */
    assert(system.getPlayerCount() == 0);

    std::cout << "  PASSED" << std::endl;
}

/**
 * MAIN TEST RUNNER
 */
int main()
{
    std::cout << std::endl;
    std::cout << "Running RankingSystem Tests..." << std::endl;
    std::cout << std::endl;

    try {
        testEmptySystem();
        testAddSinglePlayer();
        testAddMultiplePlayers();
        testAddPlayerCustomRating();
        testNoDuplicates();
        testFindNonexistent();
        testRecordMatch();
        testMatchNonexistent();
        testSaveToFile();
        testLoadFromFile();
        testMultipleMatchesInSystem();
        testLoadNonexistent();

        std::cout << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "All RankingSystem tests passed!" << std::endl;
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