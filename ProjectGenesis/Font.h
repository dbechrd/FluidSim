#ifndef FONT_H_INCLUDED
#define FONT_H_INCLUDED

#include "SFML/Graphics.hpp"
#include <string>

class Font {
    private:
		sf::Font* font;
        sf::Color color;
    public:
        Font(std::string filename, int size = 16);
        ~Font();
        sf::Text Draw(std::string text);
};

#endif // FONT_H_INCLUDED
