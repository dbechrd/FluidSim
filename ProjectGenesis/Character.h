#ifndef CHARACTER_H_INCLUDED
#define CHARACTER_H_INCLUDED

//#include "Animation.h"
#include "IAnimate.h"
#include "Object.h"
#include <string>
#include <vector>
#include "SFML/Graphics.hpp"

class RenderEngine;
class Object;

class Character : public sf::Sprite, protected IAnimate {
public:
	enum Direction {
		DIR_UP = 0,
		DIR_DOWN = 1,
		DIR_LEFT = 2,
		DIR_RIGHT = 3
	};
	enum Special {
		SPECIAL_NONE = 0, //NOTE: None is standing
		SPECIAL_WALKING = 1,
		SPECIAL_SLIDING = 2,
		SPECIAL_ROLLING = 3
	};

private:
    static std::vector<Character*> characters; //All Characters

    RenderEngine* renderer;

    int UID;
    std::string name;
	bool NPC;

	//TODO: This should go in ICollide
	int tileWidth;
    int tileHeight;

	int healthTotal;
	int health;

    bool colliding;

    Object* objColMain; //Object colliding with most
    Object* objCol[4]; //All objects colliding with

    //Character colliding with
    Character* charCol;

    int walkSpeed;
    bool walking;
	bool attacking;
    Direction facing;
	Special special;

	static int LoadCharacterCB(void *instance, int argc, char **argv, char **azColName);

public:
    Character(int uid, int new_x, int new_y);
    ~Character();

    int GetUID() { return UID; }
    std::string GetName() { return name; }
	int Speed() { return walkSpeed; }
    sf::FloatRect Rect() { return getGlobalBounds(); }
	sf::IntRect ColRect() {
		sf::FloatRect rect = Rect();
		return sf::IntRect((int)rect.left, (int)rect.top + ((int)rect.height / 2), (int)rect.width, (int)rect.height / 2);
	}

    #ifdef ORIGCOL
    Object* CollidingWith() { return objCol; }
    #else
    Object* CollidingWith() { return objColMain; }
    #endif

	int Health() { return health; }
	void Health(int nhealth) { health = nhealth; }

	int HP() { return healthTotal; }
	void HP(int nhealth) { healthTotal = nhealth; }

    void Walk(Direction dir, Special spec);
	void Stop();
    void Action();
    bool Collision();
    void Update();
    void Render();
};

#endif // CHARACTER_H_INCLUDED
