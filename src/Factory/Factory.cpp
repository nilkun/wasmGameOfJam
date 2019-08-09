#include <SDL2/SDL_image.h>
#include "./Factory.h"
// #include "./FileLocation.h"
#include "../ScreenManager/ScreenManager.h"
#include <iostream>

Factory Factory::singleton;
// int Factory::spacing = 10;
// Font Factory::font;

/* INITIALIZATION OF THE FACTORY */
void Factory::init() {
  font = bitAtlas();
  SDL_Color color = { 255, 0, 0, 0 };
  setColor(color);
};

SDL_Texture* Factory::textBox(std::deque<std::string> messages)
{
  SDL_Point temp;
  return textBox(messages, &temp);
}

SDL_Texture* Factory::textBox(std::deque<std::string> messages, SDL_Point* size)
{
  SDL_Renderer* renderer = Screen::getRenderer();

  // ERROR CHECKING
  if(!renderer)
  SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
    "Textbox creation",
    "Unable to get renderer",
    NULL);

  if(!font.texture) SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
    "Textbox creation",
    "Unable to load font",
    NULL);

  // Get the longest line of strings
  unsigned int max = 0;
  for (auto line : messages) { if (line.length() > max) { max = line.length(); } }

  // Set up some padding etc.
  // In the future, this should not be hard coded.
  int padding = 2;
  // Set the color of the font
  SDL_SetTextureColorMod(font.texture, color.r, color.g, color.b);
  // DOES THIS DO ANYTHING???

  //setup
  // Initialize variables and textures
  SDL_Texture* m_targetTexture;
  int width = max * 7;
  int height = (9 + padding) * messages.size();

  // return the size to the asker
  size -> x = width;
  size -> y = height;

  m_targetTexture = SDL_CreateTexture(renderer,
                    SDL_PIXELFORMAT_RGBA8888,
                    SDL_TEXTUREACCESS_TARGET,
                    width,
                    height);

  SDL_Rect currentPosition = { 0, 0, 7, 9 };
  SDL_SetRenderTarget(renderer, m_targetTexture);

  for(auto text : messages) {
    for(char& c : text) {
      SDL_RenderCopy(renderer, font.texture, &font.fontMap[(int) c], &currentPosition);
      currentPosition.x += currentPosition.w;
    }

    // NEW LINE
    currentPosition.y += height;
    currentPosition.x = 0;
  }
  return m_targetTexture;
}

Font Factory::bitAtlas()
{
  Font bitfont;

  // TEMPORARILY OFFLINE


  // bitfont.texture = image(Files::FONT);
  // SDL_Rect destination = { 0, 0, 7, 9 };
  // for(int i = 32; i <= 128; i++) {
  //   /* set location data in atlas */
  //   bitfont.fontMap[i] = destination;
  //   destination.x += 6;
  // }
  return bitfont;
}

SDL_Texture* Factory::image(const char* imageLocation)
{
  SDL_Renderer* renderer = Screen::getRenderer();
  SDL_Texture *texture = nullptr;
  SDL_Surface* loadImage = IMG_Load(imageLocation);
  if( loadImage == NULL )
  {
    // std::cout<<"Error loading" << std::endl;
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
        "Error Loading Image",
        imageLocation,
        NULL);
  }

  texture = SDL_CreateTextureFromSurface(renderer, loadImage);
  if(texture == NULL)
  {

    // std::cout<<"Error creating" << std::endl;
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
        "Error Creating texture",
        imageLocation,
        NULL);
  }

  // Clean up
  SDL_FreeSurface(loadImage);
  return texture;
}

void Factory::setColor(SDL_Color color)
{
  this -> color = color;
}

SDL_Texture* Factory::text(std::string text)
{
  SDL_Renderer* renderer = Screen::getRenderer();
  // ERROR CHECKING
  if(!renderer) SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
    "Text creation",
    "Error with renderer in text()",
    NULL);

  if(!font.texture) SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
    "Font failure",
    "Error with font in text()",
    NULL);

  // Initialize temporary variables and textures
  SDL_Texture* processing;
  SDL_Texture* temp;
  SDL_Rect letterPosition = { 0, 0, 0, font.fontMap[33].h };
  int textureWidth = 100;
  // Set the color of the font
  SDL_SetTextureColorMod(font.texture, color.r, color.g, color.b);

  // Instantiation
  processing = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, textureWidth, letterPosition.h);
  SDL_SetRenderTarget(renderer, processing);

  for(char& c : text) {
    letterPosition.w = font.fontMap[(int) c].w;
    // Resize if necessary
    if(textureWidth < letterPosition.w+letterPosition.x) // While oob
    {
      SDL_Rect targetTexture = { 0, 0, textureWidth, letterPosition.h }; // target location of temp texture
      while(textureWidth < letterPosition.w+letterPosition.x) textureWidth *= 2; // In the unlikely case that the letters are bigger than the texture!
      temp = processing;
      processing = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, textureWidth, letterPosition.h);
      SDL_SetRenderTarget(renderer, processing);
      SDL_RenderCopy(renderer, temp, NULL, &targetTexture);
      SDL_DestroyTexture(temp);
    }
    SDL_RenderCopy(renderer, font.texture, &font.fontMap[(int) c], &letterPosition);
    letterPosition.x += letterPosition.w;
  }
  SDL_Texture* returnTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, letterPosition.x, letterPosition.h);
  SDL_SetRenderTarget(renderer, returnTexture);
  // Recycle letterPosition for placing in texture
  letterPosition = { 0, letterPosition.y, letterPosition.x, letterPosition.h };
  SDL_RenderCopy(renderer, processing, &letterPosition, &letterPosition);
  SDL_DestroyTexture(processing);
  SDL_SetRenderTarget(renderer, NULL);
  return returnTexture;
}
