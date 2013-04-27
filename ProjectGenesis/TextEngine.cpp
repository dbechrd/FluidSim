#include "TextEngine.h"
#include "RenderEngine.h"

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include <string>
#include <iostream>

TextEngine* TextEngine::pinstance = 0;
TextEngine* TextEngine::Instance(std::string filename) {
	//Create single instance if it doesn't already exist (singleton pattern)
    if (pinstance == 0) {
        pinstance = new TextEngine(filename);
    }
    return pinstance;
}

TextEngine* TextEngine::Instance() {
    if (pinstance == 0) {
        return NULL;
    }
    return pinstance;
}

TextEngine::TextEngine(std::string filename) {
    renderer = RenderEngine::Instance();

    font = new Font(filename);

    curData = NULL;
	oldTime = Clock.getElapsedTime().asMilliseconds();
	showDebugText = true;
}

TextEngine::~TextEngine() {
    std::cout << "Terminating Text Engine\n";
    delete font;
}

void TextEngine::DebugString(std::string message, int x, int y, bool screenCoords) {
	if(!showDebugText) return;

    //Generate surface from message and render it
    sf::Text msgSurface = font->Draw(message);
	msgSurface.setPosition((float)x, (float)y);

    if(screenCoords) {
        renderer->DisableWorldCoords();
    }
    renderer->Draw(msgSurface);
}

void TextEngine::Textbox(std::string title, std::string message, int delay, int x, int y, bool screenCoords) {
    //Generate surface from message
    sf::Text titleSurface = font->Draw(title);
    sf::Text msgSurface = font->Draw(message);

    //Create new textbox data object and queue it
    textboxes.push(new TextData(titleSurface, msgSurface, delay, x, y, screenCoords));
}

void TextEngine::Update() {
    //No textboxes being shown but at least one in queue
    if(curData == NULL && !textboxes.empty()) {
        std::cout.flush();
        curData = textboxes.front();
        oldTime = Clock.getElapsedTime().asMilliseconds();
    //Textbox being shown
    }else if(curData != NULL) {
        std::cout.flush();
        //Time since last successful update
        int deltaTime = Clock.getElapsedTime().asMilliseconds() - oldTime;

        //Time to update again
        if(deltaTime >= curData->delay) {
            textboxes.pop();
            delete curData;
            curData = NULL;
        }
    }

    if(curData != NULL) {
        if(curData->screenCoords) {
            renderer->DisableWorldCoords();
        }
        renderer->Draw(curData->title);
        if(curData->screenCoords) {
            renderer->DisableWorldCoords();
        }
        renderer->Draw(curData->text);
    }
}
