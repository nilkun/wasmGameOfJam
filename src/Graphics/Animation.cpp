#include "Animation.hpp"
#include <iostream>
Animation::Animation() {};

Animation::Animation(const Animation &oldAnimation) {
    m_spriteSheet = oldAnimation.m_spriteSheet;
    m_spriteSize = nilkun::Vector{ oldAnimation.m_spriteSize.x, oldAnimation.m_spriteSize.y };
    m_spriteCenter = nilkun::Vector{ oldAnimation.m_spriteCenter.x, oldAnimation.m_spriteCenter.y };
    m_offset = nilkun::Vector { oldAnimation.m_offset.x, oldAnimation.m_offset.y };
    m_playbackSpeed = oldAnimation.m_playbackSpeed;
    m_currentFrame = oldAnimation.m_currentFrame;
    m_noOfFrames = oldAnimation.m_noOfFrames;
    m_elapsed = oldAnimation.m_elapsed;
    m_isPlaying = oldAnimation.m_isPlaying;
    m_playbackMode = oldAnimation.m_playbackMode;
}

void Animation::operator=(const Animation &oldAnimation) {
    m_spriteSheet = oldAnimation.m_spriteSheet;
    m_spriteSize = nilkun::Vector{ oldAnimation.m_spriteSize.x, oldAnimation.m_spriteSize.y };
    m_spriteCenter = nilkun::Vector{ oldAnimation.m_spriteCenter.x, oldAnimation.m_spriteCenter.y };
    m_offset = nilkun::Vector { oldAnimation.m_offset.x, oldAnimation.m_offset.y };
    m_playbackSpeed = oldAnimation.m_playbackSpeed;
    m_currentFrame = oldAnimation.m_currentFrame;
    m_noOfFrames = oldAnimation.m_noOfFrames;
    m_elapsed = oldAnimation.m_elapsed;
    m_isPlaying = oldAnimation.m_isPlaying;
    m_playbackMode = oldAnimation.m_playbackMode;
}

Animation::Animation(
    SDL_Texture* spriteSheet,
    nilkun::Vector spriteSize,
    nilkun::Vector spriteCenter,
    nilkun::Vector offset,
    playbackModes playbackMode,
    int noOfFrames,
    int playbackSpeed,
    int startFrame = 0,
    bool isPlaying = true
) {
    init(spriteSheet, spriteSize, spriteCenter, offset, playbackMode, noOfFrames, playbackSpeed, startFrame, isPlaying);
};

void Animation::init(
    SDL_Texture* spriteSheet,
    nilkun::Vector spriteSize,
    nilkun::Vector spriteCenter,
    nilkun::Vector offset,
    playbackModes playbackMode,
    int noOfFrames,
    int playbackSpeed,
    int startFrame = 0,
    bool isPlaying = true
) {
    m_playbackSpeed = playbackSpeed;
    m_spriteSize = spriteSize;
    m_spriteCenter = spriteCenter;
    m_playbackMode = playbackMode;
    m_offset = offset;
    m_noOfFrames = noOfFrames;
    m_spriteSheet = spriteSheet;
    m_currentFrame = startFrame;
    m_isPlaying = isPlaying;
    m_elapsed = 0;
}

void Animation::rewind() {
    switch(m_playbackMode) {
        case BACKANDFORTH: {
            m_elapsed = 0;
            m_currentFrame = -m_noOfFrames;            
            break;
        }
        case LOOP: {
            m_elapsed = 0;
            m_currentFrame = 0;
        }
        case ONCE: {
            m_elapsed = 0;
            m_currentFrame = 0;
        }
    }
};

bool Animation::play(float elapsedTime) {
    switch(m_playbackMode) {
        case BACKANDFORTH: {
            m_elapsed += elapsedTime * m_playbackSpeed;
            m_currentFrame = static_cast<int>(abs(m_elapsed - m_noOfFrames));
            if(m_currentFrame >= m_noOfFrames) {
                m_isPlaying = false;
                m_currentFrame = m_noOfFrames - 1;
                return false;
            }
            break;
        }
        case LOOP: {
            m_elapsed += elapsedTime * m_playbackSpeed;
            m_currentFrame = static_cast<int>(m_elapsed);
            if(m_currentFrame >= m_noOfFrames) {
                m_currentFrame = 0;
                m_elapsed = 0;
                return false;
            }
            break;
        }
        case ONCE: {
            m_elapsed += elapsedTime * m_playbackSpeed;
            m_currentFrame = static_cast<int>(m_elapsed);
            if(m_currentFrame >= m_noOfFrames) {
                return false;
            }
            break;
        }
    }
    return true;
};

void Animation::start(){
    m_isPlaying = true;
}

nilkun::Vector Animation::getCenter(float scalar) {
    return nilkun::Vector {
        m_spriteCenter.x * scalar,
        m_spriteCenter.y * scalar
    };
};

nilkun::Vector Animation::getSize(float scalar) {
    return nilkun::Vector {
        m_spriteSize.x * scalar,
        m_spriteSize.y * scalar
    };
};

SDL_Texture* Animation::getSprites() {
    return m_spriteSheet;
}

SDL_Rect Animation::getSource() {
    return SDL_Rect { 
        static_cast<int>(m_spriteSize.x * m_currentFrame), 
        0, 
        static_cast<int>(m_spriteSize.x), 
        static_cast<int>(m_spriteSize.y) 
    }; 
}
int Animation::getCurrentFrame() {
    return m_currentFrame;
}

