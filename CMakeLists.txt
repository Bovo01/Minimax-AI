cmake_minimum_required(VERSION 3.0)

project(ProgettoScacchiera)

include_directories(include src executables AI/include AI/src)

# Logica di gioco
add_library(position include/position.h src/position.cpp) # Libreria per la posizione
add_library(piece include/piece.h src/piece.cpp)          # Libreria per i pezzi
add_library(board include/board.h src/board.cpp)          # Libreria per la scacchiera
# Computer
add_library(computer AI/include/computer.h AI/src/computer.cpp)                        # Libreria per il computer generico (superclasse)
add_library(RandomComputer AI/include/RandomComputer.h AI/src/RandomComputer.cpp)      # Libreria per il computer che muove casualmente
add_library(minimaxComputer AI/include/MiniMaxComputer.h AI/src/MiniMaxComputer.cpp)   # Libreria per il computer che muove con algoritmo minimax


add_executable(scacchiera executables/scacchiera.cpp src/position.cpp src/piece.cpp src/board.cpp AI/src/computer.cpp AI/src/RandomComputer.cpp)
add_executable(replay executables/replay.cpp src/position.cpp src/piece.cpp src/board.cpp )
add_executable(minimaxExe executables/minimaxScacchiera.cpp src/position.cpp src/piece.cpp src/board.cpp AI/src/computer.cpp AI/src/MiniMaxComputer.cpp)

target_link_libraries(scacchiera position piece board computer RandomComputer)
target_link_libraries(replay position piece board)
target_link_libraries(minimaxExe position piece board computer minimaxComputer)