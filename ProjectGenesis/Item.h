#ifndef ITEM_H_INCLUDED
#define ITEM_H_INCLUDED

#include <string>

#include "SFML/Graphics.hpp"
#include "RenderEngine.h"

enum ItemType {
	WEAPON = 0,
	ARMOR = 1,
	ACCESSORY = 2,
	CONSUMABLE = 3,
	UTILITY = 4,
	QUEST = 5
};

class Item {
    private:
		//Position / Size
        sf::IntRect rect;
        int type;
        int tile;
        std::string name;
        bool collide;
        int flags;
    public:
        Item(int objType, int objTile, bool col, std::string newname, int x, int y, int w, int h);

        sf::IntRect* Rect() { return &rect; }
        std::string GetName();
        void SetName(std::string newname);
        int GetType();
        void SetType(int objType);
        int GetTile();
        void SetTile(int objTile);
        bool GetCollide();
        void SetCollide(bool col);
        int GetFlags();
        void SetFlags(int newflags);

        void Update();
        void Draw();
};

#endif // ITEM_H_INCLUDED
