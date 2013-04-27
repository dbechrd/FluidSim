#include "Font.h"
#include <iostream>
#include <string>

using namespace std;

Font::Font(std::string filename, int size) {
	font = new sf::Font();
    if(!font->loadFromFile(filename))
        cout << "Error loading font: " << filename << endl;

    color.r = 255;
    color.g = 255;
    color.b = 255;
}

Font::~Font() {
	delete font;
    cout << "Font Deleted\n";
}

sf::Text Font::Draw(std::string text) {
	return sf::Text(sf::String(text),* font, 14);
}
