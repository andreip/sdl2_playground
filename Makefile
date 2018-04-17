all:
	g++ -std=c++11 -lSDL2 -lSDL2_image -lSDL2_mixer main.cpp Game.cpp SlotMachine.cpp
