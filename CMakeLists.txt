   cmake_minimum_required(VERSION 3.10)
   project(elo-system)

   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
   set(CMAKE_CXX_STANDARD 23)

   add_executable(elo-system
           main.cpp
           # Add other .cpp files here if you have them
   )