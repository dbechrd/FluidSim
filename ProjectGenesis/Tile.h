#ifndef TILE_H_INCLUDED
#define TILE_H_INCLUDED

#include "IAnimate.h"

class RenderEngine;

class Tile : public sf::Sprite, protected IAnimate {
    private:
        RenderEngine* renderer;
        int sprite;
		bool col;
    public:
		Tile() {} //TODO: Delete me and fix object constructor
        Tile(int spritesheetId, int nsprite, bool ncol);

		int X() { return getTextureRect().left; }
		int Y() { return getTextureRect().top; }

        int Sprite() { return sprite; }
        void SetSprite(int tileSprite) { sprite = tileSprite; }

		bool IsSolid() { return col; }

		AnimationStruct* Animation() { return this->animation; }

        void Update();
        void Draw();
};

#endif // TILE_H_INCLUDED
