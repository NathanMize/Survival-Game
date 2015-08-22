all: game

game: SurvivalGame.o hunter.o world.o creature.o resources.h
	g++ SurvivalGame.o hunter.o world.o creature.o -o game -Wall -std=c++0x


hunter.o: hunter.cpp resources.h
	g++ -c hunter.cpp -Wall -std=c++0x
	
world.o: world.cpp resources.h
	g++ -c world.cpp -Wall -std=c++0x

main.o: main.cpp resources.h
	g++ -c main.cpp	-Wall -std=c++0x
	
creature.o: creature.cpp resources.h
	g++ -c creature.cpp -Wall -std=c++0x
	
SurvivalGame.o: SurvivalGame.cpp resources.h
	g++ -c SurvivalGame.cpp -Wall -std=c++0x