# LFLAGS= -Wno-all
LFLAGS= -std=c++14 -Wno-all 
# --embed-file resources
# --preload-file resources
#  -g
CFLAGS=-s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' --preload-file resources
# CFLAGS=-lSDL2 -lSDL2_image -lSDL2_gfx -lSDL2_ttf
COMPILER=emcc
# g++

# BUILD DIRECTORY
BUILDDIR = ./build


# SOURCE
MAIN.CPP = ./src/main.cpp

# MONSTERS 
MONSTERS.CPP = ./src/Monsters.cpp
MONSTERS.HPP = ./src/Monsters.h
MONSTERS.O = $(BUILDDIR)Monsters.o

SCREENMANAGER.CPP = ./src/ScreenManager/ScreenManager.cpp
SCREENMANAGER.HPP = ./src/ScreenManager/ScreenManager.h
FACTORY.CPP = ./src/Factory/Factory.cpp
FACTORY.HPP = ./src/Factory/Factory.h
STATEMACHINE.CPP = ./src/StateMachine.cpp
STATEMACHINE.HPP = ./src/StateMachine.h
STATEMACHINE.O = StateMachine.o

#OBJECTS
MAIN.O = main.o
SCREENMANAGER.O = ScreenManager.o
FACTORY.O = Factory.o

# %o: %cpp %hpp

game: main.o Factory.o ScreenManager.o Player.o Collision.o Animation.o Monsters.o Pathfinding.o States.o World.o
	$(COMPILER) $(LFLAGS) -o game.html $(MAIN.O) ScreenManager.o  $(FACTORY.O) Player.o Collision.o Animation.o Monsters.o Pathfinding.o States.o World.o $(CFLAGS) 

main.o: $(MAIN.CPP) $(SCREENMANAGER.HPP) $(FACTORY.HPP) ./src/WorldManager/World.hpp
	$(COMPILER) $(LFLAGS) -c $(MAIN.CPP) $(CFLAGS)

World.o: ./src/WorldManager/World.hpp ./src/WorldManager/World.cpp ./src/Characters/Monsters.hpp ./src/Functions/Nilkun.hpp ./src/Characters/Player.hpp
	$(COMPILER) $(LFLAGS) -c ./src/WorldManager/World.cpp $(CFLAGS)

# States.o: ./src/Characters/Monsters/States.hpp ./src/Characters/Monsters/States.cpp ./src/Characters/Player.hpp ./src/Characters/Monsters.hpp
# 	$(COMPILER) -c c $(CFLAGS) 

Monsters.o: ./src/Characters/Monsters.cpp ./src/Functions/Pathfinding.hpp ./src/Functions/Nilkun.hpp ./src/Graphics/Animation.hpp ./src/Characters/Player.hpp ./src/Characters/Monsters.hpp ./src/Characters/Monsters/States.cpp
	$(COMPILER) $(LFLAGS) -c ./src/Characters/Monsters.cpp ./src/Characters/Monsters/States.cpp $(CFLAGS)

Player.o: ./src/Characters/Player.cpp ./src/Characters/Player.hpp ./src/Characters/Monsters.hpp ./src/Functions/Nilkun.hpp ./src/Functions/Collision.hpp ./src/Graphics/Animation.hpp
	$(COMPILER) $(LFLAGS) -c ./src/Characters/Player.cpp $(CFLAGS)

Factory.o: ./src/Factory/Factory.cpp ./src/Factory/Factory.h $(SCREENMANAGER.HPP)
	$(COMPILER) $(LFLAGS) -c ./src/Factory/Factory.cpp $(CFLAGS)

Collision.o: ./src/Functions/Collision.cpp ./src/Functions/Collision.hpp
	$(COMPILER) $(LFLAGS) -c ./src/Functions/Collision.cpp $(CFLAGS)

Animation.o: ./src/Graphics/Animation.cpp ./src/Graphics/Animation.hpp ./src/Functions/Nilkun.hpp
	$(COMPILER) $(LFLAGS) -c ./src/Graphics/Animation.cpp $(CFLAGS)

ScreenManager.o: $(SCREENMANAGER.CPP) $(SCREENMANAGER.HPP)
	$(COMPILER) $(LFLAGS) -c $(SCREENMANAGER.CPP) $(CFLAGS)

Pathfinding.o: ./src/Functions/Pathfinding.hpp ./src/Functions/Pathfinding.cpp ./src/Functions/Nilkun.hpp
	$(COMPILER) $(LFLAGS) -c ./src/Functions/Pathfinding.cpp $(CFLAGS)




# $@ matches the target  $< matches the first dependent
# States.o: ./src/Monsters/States.cpp ./src/Monsters/States.hpp
# 	$(COMPILER) $(LFLAGS) -c ./src/Monsters/States.cpp $(CFLAGS)











# all:
# 	$(COMPILER) $(LFLAGS) -o $(TARGETFILE) $(SOURCEFILES) $(GAMEENGINE) $(UPDATING) $(CFLAGS)

# eventHandler.o: $(EVENTHANDLER)
# 	$(COMPILER) -c $(EVENTHANDLER)


# PATHFINDING HAS NO CPP
