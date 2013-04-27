#include "Item.h"
#include "ItemEngine.h"
#include <iostream>

Item::Item(int objType, int objTile, bool col, std::string newname, int x, int y, int w, int h) {
    type = objType;
    tile = objTile;
    name = newname;
    collide = col;
    rect.left = x;
    rect.top = y;
    rect.width = w;
    rect.height = h;
	flags = 0;

    std::cout << "Item Created: " << name << std::endl;
}

std::string Item::GetName() {
    return name;
}

void Item::SetName(std::string newname) {
    name = newname;
}

int Item::GetType() {
    return type;
}

void Item::SetType(int objType) {
    type = objType;
}

int Item::GetTile() {
    return tile;
}

void Item::SetTile(int objTile) {
    tile = objTile;
}

bool Item::GetCollide() {
    return collide;
}

void Item::SetCollide(bool col) {
    collide = col;
}

int Item::GetFlags() {
    return flags;
}

void Item::SetFlags(int newflags) {
    flags = newflags;
}

void Item::Update(Tilesheet* tileSheet) {
    Animation* tempAnimData = tileSheet->GetAnimData(tile);
    if(tempAnimData != NULL) {
        tempAnimData->Update();
    }
}

void Item::Draw(Tilesheet* tileSheet) {
    if(tile > -1) {
        sf::Texture* tileSheetSurface = tileSheet->GetSurface();
        sf::IntRect* tileSheetRect = NULL;

        Animation* tempAnimData = tileSheet->GetAnimData(tile);
        if(tempAnimData != NULL) {
            tileSheetRect = tileSheet->GetRect(tempAnimData->CurrentFrame());
        }

        if(tileSheetRect != NULL) {
            RenderEngine::Instance()->Draw((rect.left * rect.width), (rect.top * rect.height), tileSheetSurface, tileSheetRect);
        }
    }
}
