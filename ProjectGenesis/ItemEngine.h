#ifndef ItemENGINE_H_INCLUDED
#define ItemENGINE_H_INCLUDED

#include "Item.h"
#include <vector>
#include <map>

class ItemEngine{
    private:
        int totalItems;
        int objWidth;
        int objHeight;

        //Item type, tile
        std::map<int, int> objTileData;
        std::map<int, int>::iterator iterTile;

        //Item type, collision
        std::map<int, bool> objColData;
        std::map<int, bool>::iterator iterCol;

        //Item type, name
        std::map<int, std::string> objNameData;
        std::map<int, std::string>::iterator iterName;

        std::vector<Item*> Items;
    public:
        ItemEngine(std::string filename);
        ~ItemEngine();

        Item* AddItem(int objType, int x, int y);

        int GetItemWidth(int objType);
        int GetItemHeight(int objType);
        int GetItemTile(int objType);
        bool GetItemCol(int objType);
        std::string GetItemName(int objType);

        void UpdateItems(Tilesheet* tileSheet = NULL);
        void DrawItems(Tilesheet* tileSheet = NULL);
};

#endif // ItemENGINE_H_INCLUDED
