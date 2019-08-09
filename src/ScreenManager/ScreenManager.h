/***********************************
  GAME ENGINE WITH A FANTASTIC NAME
  ------A NEW SCREEN MANAGER-------
  ---------------------------------
  by nilkun
  Rev: Aug 28, 2018
 ***********************************

    USE THIS TO CREATE AND MANAGE
    THE RENDERER AND MAIN WINDOW

************************************/
#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <SDL2/SDL.h>

class Screen {

/* CREATING A SINGLETON */

public:
    static Screen* instance()
      {
        return &singleton;
      }

private:
  static Screen singleton;
  Screen() {};                                  
  Screen(const Screen&);                 
  Screen& operator=(const Screen&);      
    
/* END OF SINGLETON CREATION */

private:
  static SDL_Window *m_window;
  static SDL_Renderer *m_renderer;

  static const char* m_title;
  int m_width;
  int m_height;

/* initialization.cpp */
public:
  void init(const char *title, int width, int height);
  void close();
  static SDL_Renderer* getRenderer();

  void clear();
  void render();
  int getWidth() { return m_width; };
  int getHeight() { return m_height; };
};

#endif
