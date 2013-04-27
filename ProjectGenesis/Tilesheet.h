#ifndef TILESHEET_H_INCLUDED
#define TILESHEET_H_INCLUDED

#include <string>
#include <vector>

#include "Animation.h"
#include "RenderEngine.h"

class RenderEngine;

class Tilesheet {
    private:
        RenderEngine* renderer;
        sf::Texture* surface;
        sf::Vector2u rect;
        int tileSprites;
        int tileFrames;
        std::vector<sf::IntRect*> tileRects;

		//TODO: This is TERRIBLE use arrays!!
        std::map<int, bool> tileCol;
        std::map<int, bool>::iterator iterCol;
        std::map<int, Animation*> tileAnims;
        std::map<int, Animation*>::iterator iterAnims;
    public:
        Tilesheet();
        ~Tilesheet();
        int Load(const std::string &filename, int frameWidth, int frameHeight);
        sf::Texture* GetSurface();
        sf::IntRect* GetRect(int currentFrame);
        bool GetCollide(int tileType);
        Animation* GetAnimData(int tileType);
        //int GetStartFrame(int sprite);
        //int GetNextFrame(int sprite, int currentFrame);
};

#endif // TILESHEET_H_INCLUDED
