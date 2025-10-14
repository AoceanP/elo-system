ELO Ranking System
A terminal-based player ranking system using the Elo rating algorithm

The Story

You're running a competitive gaming community. Players keep asking "Who's the best?" and "How do I compare?" Manual
tracking on spreadsheets becomes a nightmare. You need a fair, automated system that updates ratings after every match.
This is where the Elo Rating System comes in.

How It Works

Every player starts with a rating of 1200. When two players compete, the system calculates expected win probabilities
based on their ratings. After the match, ratings are adjusted - winners gain points (more points if they beat a
higher-rated opponent) and losers lose points (fewer points lost if they lose to a higher-rated opponent).
The system is zero-sum, meaning points gained by one player equal points lost by the other. Over time, ratings
stabilize and accurately reflect each player's skill level.

Features

Add Players - Register new players with default 1200 rating
Find Match - Automatically pairs you with a random opponent
Record Results - Enter match outcomes (win/loss/draw) and watch ratings update in real-time
Leaderboard - View all players ranked by rating with complete statistics
Persistent Storage - All data saves to CSV files between sessions
Smart Statistics - Track games played, wins, losses, and draws for each player


Technologies Used

C++ - Core language
Object-Oriented Design - Player, Match, and RankingSystem classes
Smart Pointers - Modern C++ memory management with unique_ptr
File I/O - CSV-based data persistence
STL Containers - Vectors for dynamic player management

Run the program:

./elo_system

Navigate the menu by adding players to build your community, selecting "Find Match" to get a random opponent,
 entering the match result (1 = you win, 0 = draw, -1 = opponent wins), and watching the magic happen as ratings
 update automatically. Check the leaderboard to see the rankings.

Example Workflow

Alice joins with a rating of 1200. Bob joins with a rating of 1200. Alice defeats Bob, gaining approximately
16 points while Bob loses approximately 16 points. Their new ratings become Alice at 1216 and Bob at 1184.
When Bob defeats Charlie, Bob gains points based on the rating difference. The leaderboard updates automatically
to reflect current standings.

The Math Behind It

The system uses the standard Elo formula. Expected score calculation follows the formula:
E = 1 / (1 + 10^((opponent_rating - player_rating) / 400)). Rating adjustment is calculated as:
new_rating = old_rating + K * (actual_score - expected_score). The K-factor of 32 balances rating volatility
and stability.

Project Structure

elo-ranking-system/
    - main.cpp
    - RankingSystem.h
    - RankingSystem.cpp
    - Player.h
    - Player.cpp
    - Match.h
    - Match.cpp
    - data/
        - players.csv

What I Learned

Implementing mathematical algorithms in real-world applications
Object-oriented design principles and class relationships
Modern C++ features including smart pointers, RAII, and lambda functions
File I/O and data persistence strategies
User experience design for command-line interfaces


Future Enhancements

Matchmaking based on similar ratings instead of just random selection
Rating history tracking and visualization
Different K-factors for new versus experienced players
Export leaderboard to formatted reports
Web-based interface for easier access
implementing this elo system into a future game

Author: Aleksandar Panich
Version: 1.0