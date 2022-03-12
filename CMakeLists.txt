cmake_minimum_required(VERSION 3.0)

project(ProgettoScacchiera)

include_directories(include src executables)

add_library(position include/position.h src/position.cpp)
add_library(piece include/piece.h src/piece.cpp)
add_library(board include/board.h src/board.cpp)
add_library(computer include/computer.h src/computer.cpp)


add_executable(scacchiera executables/scacchiera.cpp src/position.cpp src/piece.cpp src/board.cpp src/computer.cpp )
add_executable(replay executables/replay.cpp src/position.cpp src/piece.cpp src/board.cpp )

target_link_libraries(scacchiera position piece board computer)
target_link_libraries(replay position piece board)