#include <SDL2/SDL.h>
#include <vector>
#include "./ScreenManager/ScreenManager.h"
#include "./Factory/Factory.h"
#include "./WorldManager/World.hpp"

#include <emscripten.h>

class StateMan;
class Play;
class StartGame;

// void StateMan::switchState(int i);
class GameState {
  public:
  virtual void enter()=0;
  virtual bool execute()=0;
  virtual void exit()=0;
};

class StateMan {
  public:    
    GameState *play;
    GameState *pause;
    GameState *start;
    GameState *current;
  StateMan(Factory *factory, Screen *screen) {
    
  }

  void switchState(int i) {
    current -> exit();
    if(i == 1) current = play;

    current -> enter();
  }
};

class Play : public GameState {
  public:
    SDL_Texture* playerTexture;
    std::vector<SDL_Texture *> monsterTextures;
    std::vector<SDL_Texture *> tileTextures;
    World world;
    // bool running;
    const Uint8 *keyboard = SDL_GetKeyboardState(NULL);
    Uint32 lastTime, currentTime;
    float elapsedTime = 0;
    float tileSize;
    
    SDL_Event event;
    SDL_Renderer *renderer;

    Play(Factory *factory, Screen *screen) {

      SDL_Texture* playerTexture = factory -> image("resources/heroWalking2.png");
      
      monsterTextures = std::vector<SDL_Texture *> {
        factory -> image("resources/trollsWalk.png"),
        factory -> image("resources/trollsDie.png"),
        factory -> image("resources/redTrollsWalk.png"),
        factory -> image("resources/redTrollsDie.png"),
        factory -> image("resources/blueTrollsWalk.png"),
        factory -> image("resources/blueTrollsDie.png"),
        factory -> image("resources/lordEvil.png"),
        factory -> image("resources/evilDies.png")
      };
      tileTextures = std::vector<SDL_Texture *> {
        factory -> image("resources/dungeonFloor.png"),
        factory -> image("resources/dungeonWall.png"),
        factory -> image("resources/grass.png"),
        factory -> image("resources/rock.png"),
        factory -> image("resources/tree.png"),
        factory -> image("resources/water.png")
      };

      // Initialize some variables
      // running = true;
      lastTime = 0;
      currentTime = 0;
      elapsedTime = 0;
      lastTime = currentTime = SDL_GetTicks();
      tileSize = screen -> getWidth() / (17 + 2);
      renderer = screen -> getRenderer();
      world.init2(tileTextures, monsterTextures, playerTexture, tileSize);

  }  
  void enter(){};
  void exit() {};
  bool execute() {
    elapsedTime = (currentTime - lastTime) * .001f;
    while(SDL_PollEvent(&event)) {
      switch(event.type)
      {
        case SDL_QUIT:
        {
          return false;
          break;
        }
      }
      world.handleEvents(keyboard);   
    }
    world.update(elapsedTime, currentTime);
    world.render(renderer, tileSize);

    SDL_RenderPresent(renderer);
    lastTime = currentTime;
    currentTime = SDL_GetTicks();
    return true;
  }
};

class StartGame : public GameState {
  SDL_Texture* titleTexture;
  SDL_Event event;
  SDL_Rect rect = { 0, 0, 0, 0};
  SDL_Renderer *renderer;
  StateMan *parent;

  public:
    StartGame(Factory *factory, Screen *screen, StateMan *parent2) {
      titleTexture = factory -> image("resources/title.png");
      rect.w = screen -> getWidth();
      rect.h = screen -> getHeight();
      renderer = screen -> getRenderer();
      parent = parent2;
    };

    void enter(){};
    void exit() {};
    bool execute() {
      while(SDL_PollEvent(&event)) {
        switch(event.type)
        {
          case SDL_QUIT:
          {
            return false;
            break;
          }
          case SDL_KEYDOWN:
          {
            parent -> switchState(1);
            break;
          }
        }
      }
    SDL_RenderCopy(renderer, titleTexture, NULL, &rect);
    SDL_RenderPresent(renderer);
    return true;
  }
};

StateMan *stateMan;

void gameLoop() {
  stateMan -> current -> execute();
}

int main() {

  // Create game screen and 
  Screen *screen = Screen::instance();
  screen -> init("A Game of Jam", 1024, 576);

  // Create textures
  Factory *factory = Factory::instance();

  StateMan state(factory, screen);
  stateMan = &state;


  state.play = new Play(factory, screen);
  state.start = new StartGame(factory, screen, &state);
  state.current = state.start;

  // #ifdef __EMSCRIPTEN__
    std::cout << "Game of Jam (alpha) :: WebAssembly version." << std::endl;
    std::cout << "USING EMSCRIPTEN..." << std::endl;

    // void emscripten_set_main_loop(em_callback_func func, int fps, int simulate_infinite_loop);
    emscripten_set_main_loop(gameLoop, -1, 1);
  // #else
    // while(state.current -> execute())
    // {    };
  // #endif

  screen -> close();  
  return 1;
};
