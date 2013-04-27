#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED

#include <string>

#include "SFML/Graphics.hpp"
#include "RenderEngine.h"
#include "Tile.h"

//TODO: Figure out interaction details
enum {
    WALK = 1,
    TALK = 2,
    TAKE = 4
    //STYLE4  =   8,
    //STYLE5  =  16,
    //STYLE6  =  32,
    //STYLE7  =  64,
    //STYLE8  = 128
};

struct ObjectData {
	int id;
	int spritesheetId;
	int spriteId;
	std::string name;
	bool collide;
	int flags;
};

class Object : public sf::Sprite, protected IAnimate {
    private:
        ObjectData* objData;
    public:
		Object(ObjectData* nobjData, sf::Vector2f position); //TODO: Refactor col into ICollide

        std::string GetName();
        void SetName(std::string newname);
        int GetId();
        void SetId(int objId);
        bool GetCollide();
        void SetCollide(bool col);
        int GetFlags();
        void SetFlags(int newflags);
		sf::FloatRect Rect() { return getGlobalBounds(); }
		sf::IntRect ColRect() {
			sf::FloatRect rect = Rect();
			return sf::IntRect((int)rect.left, (int)rect.top, (int)rect.width, (int)rect.height);
		}

        void Update();
        void Draw();
};

#endif // OBJECT_H_INCLUDED
