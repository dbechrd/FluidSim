#include "ItemEngine.h"
#include <fstream>
#include <iostream>
#include <sstream>

ItemEngine::ItemEngine(std::string filename) {
    std::ifstream objDataStream(filename.c_str());

    if(objDataStream == NULL) {
        std::cout << "Item Data File Error" << std::endl;
    }

	std::string header;
	do {
		getline(objDataStream, header);
	} while(header[0] == '#');

	std::stringstream headerStream(header);
    headerStream >> totalItems;
    headerStream >> objWidth;
    headerStream >> objHeight;

	std::cout << "Loading Items" << std::endl;
	std::cout << "{totalItems, objWidth, objHeight}" << std::endl;
	std::cout << "{" << totalItems << ", " << objWidth << ", " << objHeight << "}" << std::endl;

    int objNumber;
    int tileNumber;
    int objCol;
    std::string objName;

	std::cout << "{objId, tileId, collide, name}\n";

	std::string dataEntry;
    for(int i = 0; i < totalItems; i++) {
		//Discard comments
		do {
			getline(objDataStream, dataEntry);
		} while(dataEntry[0] == '#');

		std::stringstream dataEntryStream(dataEntry);
        dataEntryStream >> objNumber;
        dataEntryStream >> tileNumber;
        dataEntryStream >> objCol;
        dataEntryStream.ignore(5, ' ');
        getline(dataEntryStream, objName);
        objTileData[objNumber] = tileNumber;
        objColData[objNumber] = (objCol != 0) ? true : false;
        objNameData[objNumber] = objName;
		std::cout << "{" << objNumber << ", " << tileNumber << ", " << objCol << ", " << objName << "}" << std::endl;
    }
}

ItemEngine::~ItemEngine() {
    for(int i = Items.size() - 1; i >= 0; i--) {
        if(Items[i] != NULL) {
            delete Items[i];
            Items[i] = NULL;
        }
    }
    Items.clear();
}

Item* ItemEngine::AddItem(int objType, int x, int y) {
    int objTile = GetItemTile(objType);
    bool collide = GetItemCol(objType);
    std::string name = GetItemName(objType);

    Item* newobj = new Item(objType, objTile, collide, name, x, y, objWidth, objHeight);
    Items.push_back(newobj);

    return newobj;
}

int ItemEngine::GetItemWidth(int objType) {
    return objWidth;
}

int ItemEngine::GetItemHeight(int objType) {
    return objHeight;
}

int ItemEngine::GetItemTile(int objType) {
    iterTile = objTileData.find(objType);
    if(iterTile != objTileData.end()) {
        return iterTile->second;
    }
    return -1;
}

bool ItemEngine::GetItemCol(int objType) {
    iterCol = objColData.find(objType);
    if(iterCol != objColData.end()) {
        return iterCol->second;
    }
    return false;
}

std::string ItemEngine::GetItemName(int objType) {
    iterName = objNameData.find(objType);
    if(iterName != objNameData.end()) {
        return iterName->second;
    }
    return "GetItemName Failed";
}

void ItemEngine::UpdateItems(Tilesheet* tileSheet) {
    for(int i = 0; i < (int)Items.size(); i++) {
        Items[i]->Update(tileSheet);
    }
}

void ItemEngine::DrawItems(Tilesheet* tileSheet) {
    for(int i = 0; i < (int)Items.size(); i++) {
        Items[i]->Draw(tileSheet);
    }
}
