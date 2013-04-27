#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Tilesheet.h"
#include "RenderEngine.h"
#include "Animation.h"

Tilesheet::Tilesheet() {
    renderer = NULL;
    surface = NULL;

    renderer = RenderEngine::Instance();
}

Tilesheet::~Tilesheet() {
    for(int i = tileRects.size() - 1; i >= 0; i--) {
        if(tileRects[i] != NULL) {
            delete tileRects[i];
            tileRects[i] = NULL;
        }
    }
    tileRects.clear();

    for(int j = tileAnims.size() - 1; j >= 0; j--) {
        if(tileAnims[j] != NULL) {
            delete tileAnims[j];
            tileAnims[j] = NULL;
        }
    }
    tileAnims.clear();
}

int Tilesheet::Load(const std::string &filename, int frameWidth, int frameHeight) {
    ////////////////////////////////////////////////////////////////////////////////////
    //Load Animation Data Stream

    std::ifstream animDataStream(filename.c_str());

    if(animDataStream == NULL) {
        std::cout << "Tile Animation Data Error" << std::endl;
    }

    ////////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////////
    //Load tilesheet surface

    //surface = renderer->LoadSurface(tileSheetFilename);
    rect = surface->getSize();
    ////////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////////
    //Load Frame Animation Data

	std::string header;
	do {
		getline(animDataStream, header);
	} while(header[0] == '#');

	std::stringstream headerStream(header);
    headerStream >> tileSprites;
    headerStream >> tileFrames;

    int spriteNumber;
    int spriteCol;
    int startFrame;
    int totalFrames;
    int delay;
    int animFlag;

	std::cout << "Loading Tile Animation Data" << std::endl;
	std::cout << "{spriteNumber, collision, startFrame, frames, delay, animFlag}" << std::endl;

	std::string dataEntry;
    for(int i = 0; i < tileSprites; i++) {
		//Discard comments
		do {
			getline(animDataStream, dataEntry);
		} while(dataEntry[0] == '#');

		std::stringstream dataEntryStream(dataEntry);
        dataEntryStream >> spriteNumber;
        dataEntryStream >> spriteCol;
        dataEntryStream >> startFrame;
        dataEntryStream >> totalFrames;
        dataEntryStream >> delay;
        dataEntryStream >> animFlag;

		std::cout << "{" << spriteNumber << ", " << spriteCol << ", " << startFrame << ", " << totalFrames << ", " << delay << ", " << animFlag  << "}" << std::endl;

        //tileAnims.push_back(new Animation(spriteNumber, startFrame, totalFrames, delay, animFlag));
        tileAnims[spriteNumber] = new Animation(startFrame, totalFrames, delay, animFlag);
        if(spriteCol == 1) {
            tileCol[spriteNumber] = true;
        }else{
            tileCol[spriteNumber] = false;
        }
    }
    ////////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////////
    //Load Frame Rects

    int tileNumber = 0;

	for(int j = 0; j < ((int)rect.y / frameHeight); j++) {
        for(int k = 0; k < ((int)rect.x / frameWidth); k++) {
            tileRects.push_back(new sf::IntRect);
            tileRects[tileNumber]->left = k * frameWidth;
            tileRects[tileNumber]->top = j * frameHeight;
            tileRects[tileNumber]->width = frameWidth;
            tileRects[tileNumber]->height = frameHeight;
            tileNumber++;
        }
    }
    ////////////////////////////////////////////////////////////////////////////////////

    return tileSprites;
}

sf::Texture* Tilesheet::GetSurface() {
    return surface;
}

sf::IntRect* Tilesheet::GetRect(int currentFrame) {
    return tileRects[currentFrame];
}

bool Tilesheet::GetCollide(int tileType) {
    iterCol = tileCol.find(tileType);
    if(iterCol != tileCol.end()) {
        return iterCol->second;
    }
    return false;
}


Animation* Tilesheet::GetAnimData(int tileType) {
    if(tileType > -1) {
        iterAnims = tileAnims.find(tileType);
        if(iterAnims != tileAnims.end()) {
            return iterAnims->second;
        }
    }
    return NULL;
}


/*int Tilesheet::GetStartFrame(int sprite) {
    //std::cout << "GetNextFrame: " << sprite << ", " << currentFrame << std::endl;
    int startFrame = tileAnims[sprite]->StartFrame();
    return startFrame;
}

int Tilesheet::GetNextFrame(int sprite, int currentFrame) {
    //std::cout << "GetNextFrame: " << sprite << ", " << currentFrame << std::endl;
    std::cout << "1";
    int nextFrame = tileAnims[sprite]->NextFrame(currentFrame);
    std::cout << "5";
    return nextFrame;
}*/
