
class Play : public GameState {
  SDL_Texture* playerTexture;
  std::vector<SDL_Texture *> monsterTextures;
  std::vector<SDL_Texture *> tileTextures;
  World world;
  bool running;
  const Uint8 *keyboard = SDL_GetKeyboardState(NULL);
  Uint32 lastTime, currentTime;
  float elapsedTime = 0;
  float tileSize;
  SDL_Event event;
  SDL_Renderer *renderer;

  Play(Factory *factory, Screen *screen) {

    SDL_Texture* playerTexture = factory -> image("./resources/heroWalking2.png");
    
    monsterTextures = std::vector<SDL_Texture *> {
      factory -> image("./resources/trollsWalk.png"),
      factory -> image("./resources/trollsDie.png"),
      factory -> image("./resources/redTrollsWalk.png"),
      factory -> image("./resources/redTrollsDie.png"),
      factory -> image("./resources/blueTrollsWalk.png"),
      factory -> image("./resources/blueTrollsDie.png"),
      factory -> image("./resources/lordEvil.png"),
      factory -> image("./resources/evilDies.png")
    };
    tileTextures = std::vector<SDL_Texture *> {
      factory -> image("./resources/dungeonFloor.png"),
      factory -> image("./resources/dungeonWall.png"),
      factory -> image("./resources/grass.png"),
      factory -> image("./resources/rock.png"),
      factory -> image("./resources/tree.png"),
      factory -> image("./resources/water.png")
    };

    // Initialize some variables
    running = true;
    lastTime = 0;
    currentTime = 0;
    elapsedTime = 0;
    lastTime = currentTime = SDL_GetTicks();
    float tileSize = screen -> getWidth() / (17 + 2);
    renderer = screen -> getRenderer();
    world = World(tileTextures, monsterTextures, playerTexture, tileSize);
  }  

  void execute() {
    elapsedTime = (currentTime - lastTime) * .001f;
    while(SDL_PollEvent(&event)) {
      switch(event.type)
      {
        case SDL_QUIT:
        {
          running = false;
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
  }
};