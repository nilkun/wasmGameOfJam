#include "./ScreenManager.h"

Screen Screen::singleton;
SDL_Window* Screen::m_window;
SDL_Renderer* Screen::m_renderer;
const char* Screen::m_title;

void Screen::init(const char *title, int width, int height) 
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0) 
    {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
        "Error initializing SDL.",
        "SDL could not be initialized. What a pity...",
        NULL);
    }
    m_window = SDL_CreateWindow( 
                    title,
                    SDL_WINDOWPOS_UNDEFINED,
                    SDL_WINDOWPOS_UNDEFINED,
                    width,
                    height,
                    SDL_WINDOW_SHOWN);
    if(m_window == NULL) {
        SDL_Quit();
    }
    m_renderer =  SDL_CreateRenderer(m_window, -1,
                    SDL_RENDERER_PRESENTVSYNC);

    if(m_renderer == NULL) {
        SDL_DestroyWindow(m_window);
        SDL_Quit();
    }
    m_width = width;
    m_height = height;
}

void Screen::close() {
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

// ScreenManager::resolution getResolution() {
    
// }

SDL_Renderer* Screen::getRenderer() {
    return m_renderer;
}

void Screen::render() {
    SDL_RenderPresent(m_renderer);
}

void Screen::clear() {
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0); // SET SCREEN TO BLACK
    SDL_RenderClear(m_renderer);
}
