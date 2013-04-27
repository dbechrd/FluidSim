#include "Layer.h"
#include <iostream>
#include <vector>
#include <string>

#include "Tile.h"
#include "RenderEngine.h"
#include "Camera.h"

using namespace std;

Layer::Layer(int lWidth, int lHeight, int tWidth, int tHeight) {
    camera = NULL;

    levelWidth = lWidth;
    levelHeight = lHeight;
    tileWidth = tWidth;
    tileHeight = tHeight;
	layerWidth = levelWidth / tileWidth;
	layerHeight = levelHeight / tileHeight;

	tileData = new int*[layerWidth];
    for(int j = 0; j < layerWidth; j++) {
		tileData[j] = new int[layerHeight];
        for(int k = 0; k < layerHeight; k++) {
            tileData[j][k] = 0;
        }
    }

    camera = Camera::Instance()->Rect();
    visible = true;
    cout << "Layer Created\n";
}

Layer::~Layer() {
    /*for(int j = tileData.size() - 1; j >= 0; j--) {
        for(int k = tileData[j].size() - 1; k >= 0; k--) {
            if(tileData[j][k] != NULL) {
                delete tileData[j][k];
                tileData[j][k] = NULL;
            }
        }
        tileData[j].clear();
    }
    tileData.clear();*/

	for(int j = 0; j < layerWidth; j++) {
        delete tileData[j];
    }
	delete tileData;
    cout << "Layer Deleted\n";
}

int Layer::GetSprite(int x, int y) {
    return tileData[x][y];
}

void Layer::SetSprite(int x, int y, int tileSprite) {
    tileData[x][y] = tileSprite;
}

//Cleanup: Unused
//int Layer::GetX(int x, int y) {
//    if(tileData[x][y] != NULL) {
//        return tileData[x][y]->X();
//    }
//    return -1;
//}
//
//int Layer::GetY(int x, int y) {
//    if(tileData[x][y] != NULL) {
//        return tileData[x][y]->Y();
//    }
//    return -1;
//}

//void Layer::Update() {
//	//TODO: Update tile anims in Level.cpp
//    for(int j = 0; j < (levelWidth / tileWidth); j++) {
//        for(int k = 0; k < (levelHeight / tileHeight); k++) {
//            if(tileData[j][k] != NULL) {
//                tileData[j][k]->Update();
//            }
//        }
//    }
//}

//void Layer::Draw() {
//    int xAdd = (camera->left) / tileWidth;                   //-150 / 32 = -4
//    int yAdd = (camera->top) / tileHeight;                  //-200 / 32 = -6
//    int xMax = ((camera->left + camera->width) / tileWidth) + 1;  //-150 + 640 / 32 = 16
//    int yMax = ((camera->top + camera->height) / tileHeight) + 1; //-150 + 480 / 32 = 11
//
//    for(int j = 0; j < (camera->width / tileWidth) + 1; j++) // 20
//    {
//        for(int k = 0; k < (camera->height / tileHeight) + 1; k++) // 15
//        {
//            if(j + xAdd >= 0 && j + xAdd < xMax && j + xAdd < levelWidth / tileWidth && k + yAdd >= 0 && k + yAdd < yMax && k + yAdd < levelHeight / tileHeight) {
//                if(tileData[j + xAdd][k + yAdd] != NULL) {
//                    tileData[j + xAdd][k + yAdd]->Draw();
//                }
//            }
//        }
//    }
//}
