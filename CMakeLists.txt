   cmake_minimum_required(VERSION 3.10)
   project(elo-system)

   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
   set(CMAKE_CXX_STANDARD 23)

   add_executable(elo-system
           src/main.cpp
           src/Player.cpp
           src/Match.cpp
           src/RankingSystem.cpp
   )

   add_executable(player_test
           tests/PlayerTest.cpp
           src/Player.cpp
   )

   add_executable(match_test
           tests/MatchTest.cpp
           src/Match.cpp
           src/Player.cpp
   )

   add_executable(ranking_test
           tests/RankingSystemTest.cpp
           src/RankingSystem.cpp
           src/Match.cpp
           src/Player.cpp
   )