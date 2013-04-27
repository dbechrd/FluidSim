#ifndef TEXTENGINE_H_INCLUDED
#define TEXTENGINE_H_INCLUDED

#include "SFML/Graphics.hpp"
#include "Font.h"
#include "RenderEngine.h"
#include <queue>
#include <string>

struct TextData {
    public:
        //Title and message to display
        sf::Text title;
        sf::Text text;

        //How long (ms) to display text
        int delay;

        //If true coords are relative to the world not the screen
        bool screenCoords;

        //Set values
        TextData::TextData(sf::Text ntitle, sf::Text ntext, int ndelay, int nx, int ny, bool nscreenCoords) {
            title = ntitle;
			title.setPosition((float)nx, (float)ny - 20);
            text = ntext;
			title.setPosition((float)nx, (float)ny);
            delay = ndelay;
            screenCoords = nscreenCoords;
        }

        //Cleanup: Free surface
		//TextData::~TextData() {
		//	delete text;
		//	text = NULL;
		//}
};

class TextEngine {
    private:
        //Singleton instance
        static TextEngine* pinstance;

        //Renderer instance
        RenderEngine* renderer;

        //std::vector<Font*> fonts;
        Font* font;

        //Queue of textboxes to be shown
        std::queue<TextData*> textboxes;

        //Data for current textbox
        TextData* curData;

        //Time since last update
        int oldTime;
    protected:
        //Singleton constructor and assignment protection
        TextEngine(std::string filename);
        TextEngine(const TextEngine&);
        TextEngine& operator= (const TextEngine&);
    public:
		bool showDebugText;

        static TextEngine* Instance(std::string filename);
        static TextEngine* Instance();
        ~TextEngine();
        void TextEngine::DebugString(std::string message, int x = 0, int y = 0, bool screenCoords = true);
        void Textbox(std::string title, std::string message, int delay, int x = 0, int y = 0, bool screenCoords = true);
        void Update();
};

#endif // TEXTENGINE_H_INCLUDED
