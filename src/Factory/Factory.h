/***********************************
  GAME ENGINE WITH A FANTASTIC NAME
  ----------A NEW FACTORY----------
  ---------------------------------
  by nilkun

 **********************************
 
  THIS SINGLETON HANDLERS AND 
  STORES ALL THE TEXTURES.

 **********************************/

#ifndef FACTORY_H_
#define FACTORY_H_

#include <SDL2/SDL.h>
#include <deque>
#include <string>
#include <unordered_map>

class Font 
        {
        public:
            std::unordered_map<int, SDL_Rect> fontMap;
            SDL_Texture *texture;
        };

class Factory {

/* CREATING A SINGLETON */

public:
    static Factory* instance()
      {
        return &singleton;
      }

private:
    static Factory singleton;
      Factory() {};                            
      Factory(const Factory&);                 
      Factory& operator=(const Factory&);      
    
/* END OF SINGLETON CREATION */

private:

    SDL_Color color;
    int spacing;


public:
    Font font;

    // functions
    //void setRenderer(SDL_Renderer *renderer); // SHOULD CHECK IF IT HAS RENDERER ALREADY!!
    SDL_Texture* textBox(std::deque<std::string> messages);
    SDL_Texture* textBox(std::deque<std::string> messages, SDL_Point* size);
    SDL_Texture* image(const char* imageLocation);
    SDL_Texture* text(std::string text);

    Font bitAtlas();
    void init();
    void setColor(SDL_Color color);
};


#endif