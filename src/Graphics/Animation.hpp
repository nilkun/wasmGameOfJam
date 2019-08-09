#pragma once

#include <iostream>
#include "../Functions/Nilkun.hpp"
#include <SDL2/SDL.h>

enum playbackModes {
    LOOP, 
    BACKANDFORTH,
    ONCE
};

class Animation {
    public:
        Animation(    
            SDL_Texture* spriteSheet,
            nilkun::Vector spriteSize,
            nilkun::Vector spriteCenter,
            nilkun::Vector offset,
            playbackModes playbackMode,
            int noOfFrames,
            int playbackSpeed,
            int startFrame,
            bool isPlaying
        );
        Animation();
        ~Animation() {
            // std::cout << "Animation destructor." << std::endl;
        };
        void operator=(const Animation &animation);
        void init(    
            SDL_Texture* spriteSheet,
            nilkun::Vector spriteSize,
            nilkun::Vector spriteCenter,
            nilkun::Vector offset,
            playbackModes playbackMode,
            int noOfFrames,
            int playbackSpeed,
            int startFrame,
            bool isPlaying
        );
        Animation(const Animation &oldAnimation);
        void rewind();
        bool play(float elapsedTime);
        void start();
        nilkun::Vector getCenter(float scalar);
        nilkun::Vector getSize(float scalar);
        SDL_Texture* getSprites();
        SDL_Rect getSource();
        int getCurrentFrame();
    
    private:
        SDL_Texture* m_spriteSheet;

        nilkun::Vector m_spriteSize;
        nilkun::Vector m_spriteCenter;
        nilkun::Vector m_offset;

        int m_playbackSpeed;
        int m_currentFrame;
        int m_noOfFrames;

        float m_elapsed;
        bool m_isPlaying;
        playbackModes m_playbackMode;
};