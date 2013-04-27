#include "ObjectEngine.h"
#include <fstream>
#include <iostream>
#include <sstream>

ObjectEngine::ObjectEngine(std::string filename) {
    std::ifstream objDataStream(filename.c_str());

    if(objDataStream == NULL) {
        std::cout << "Object Data File Error" << std::endl;
    }

	std::string header;
	do {
		getline(objDataStream, header);
	} while(header[0] == '#');

	std::stringstream headerStream(header);
    headerStream >> totalObjects;
    headerStream >> objWidth;
    headerStream >> objHeight;

	std::cout << "Loading Objects" << std::endl;
	std::cout << "{totalObjects, objWidth, objHeight}" << std::endl;
	std::cout << "{" << totalObjects << ", " << objWidth << ", " << objHeight << "}" << std::endl;

    int objNumber;
    int tileNumber;
    int objCol;
    std::string objName;

	std::cout << "{objId, tileId, collide, name}\n";

	std::string dataEntry;
    for(int i = 0; i < totalObjects; i++) {
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

ObjectEngine::~ObjectEngine() {
    for(int i = objects.size() - 1; i >= 0; i--) {
        if(objects[i] != NULL) {
            delete objects[i];
            objects[i] = NULL;
        }
    }
    objects.clear();
}

Object* ObjectEngine::AddObject(int objType, int x, int y) {
    int objTile = GetObjectTile(objType);
    bool collide = GetObjectCol(objType);
    std::string name = GetObjectName(objType);

	//TODO: FIX ME: Create object
    //Object* newobj = new Object(objType, objTile, collide, name, x, y, objWidth, objHeight);
    //objects.push_back(newobj);

    //return newobj;
	return NULL;
}

int ObjectEngine::GetObjectWidth(int objType) {
    return objWidth;
}

int ObjectEngine::GetObjectHeight(int objType) {
    return objHeight;
}

int ObjectEngine::GetObjectTile(int objType) {
    iterTile = objTileData.find(objType);
    if(iterTile != objTileData.end()) {
        return iterTile->second;
    }
    return -1;
}

bool ObjectEngine::GetObjectCol(int objType) {
    iterCol = objColData.find(objType);
    if(iterCol != objColData.end()) {
        return iterCol->second;
    }
    return false;
}

std::string ObjectEngine::GetObjectName(int objType) {
    iterName = objNameData.find(objType);
    if(iterName != objNameData.end()) {
        return iterName->second;
    }
    return "GetObjectName Failed";
}

void ObjectEngine::UpdateObjects() {
    for(int i = 0; i < (int)objects.size(); i++) {
        objects[i]->Update();
    }
}

void ObjectEngine::DrawObjects() {
    for(int i = 0; i < (int)objects.size(); i++) {
        objects[i]->Draw();
    }
}
