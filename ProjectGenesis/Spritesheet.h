#ifndef SPRITESHEET_H_INCLUDED
#define SPRITESHEET_H_INCLUDED

#include <vector>
#include <string>
#include <assert.h>
#include "SFML/Graphics.hpp"

#define MAX_FRAMES 128
#define MAX_ANIMATIONS 128

class AnimationStruct;

class Spritesheet {
private: //TODO: Make these private probably..
	int id;
	sf::Texture* texture;
	sf::Vector2i frameSize;

	int frames;
	int animations;

	sf::IntRect* frameCollection[MAX_FRAMES];
	AnimationStruct* animationCollection[MAX_ANIMATIONS];

	static int LoadAnimationsCB(void *instance, int argc, char **argv, char **azColName);

public:
	Spritesheet(int nid, sf::Texture* ntexture, int nframes, int nframeWidth, int nframeHeight);
	~Spritesheet();

	int FrameCount(){ return frames; }
	const sf::Vector2i* FrameSize(){ return &frameSize; }
	sf::Texture* Texture(){ return texture; }
	sf::IntRect* FrameRect(int frameId){ assert(frameId < frames); return frameCollection[frameId]; }
	AnimationStruct* Animation(int id);
};

//class Tilesheet{
//    private:
//        RenderEngine* renderer;
//        sf::Texture* surface;
//        sf::Vector2u rect;
//        int tileSprites;
//        int tileFrames;
//        std::vector<sf::IntRect*> tileRects;
//
//		//TODO: This is TERRIBLE use arrays!!
//        std::map<int, bool> tileCol;
//        std::map<int, bool>::iterator iterCol;
//        std::map<int, Animation*> tileAnims;
//        std::map<int, Animation*>::iterator iterAnims;
//    public:
//        Tilesheet();
//        ~Tilesheet();
//        int Load(const std::string &filename, int frameWidth, int frameHeight);
//        sf::Texture* GetSurface();
//        sf::IntRect* GetRect(int currentFrame);
//        bool GetCollide(int tileType);
//        Animation* GetAnimData(int tileType);
//        //int GetStartFrame(int sprite);
//        //int GetNextFrame(int sprite, int currentFrame);
//};

#endif // SPRITESHEET_H_INCLUDED
