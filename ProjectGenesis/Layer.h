#ifndef LAYER_H_INCLUDED
#define LAYER_H_INCLUDED

//#include <vector>
#include <string>
#include "SFML/Graphics.hpp"

class Tile;

class Layer {
    private:
        int levelWidth;
        int levelHeight;

        int tileWidth;
        int tileHeight;

		int layerWidth;
		int layerHeight;

        int** tileData;
        sf::IntRect* camera;
    public:
		bool visible;

        Layer(int lWidth, int lHeight, int tWidth, int tHeight);
        ~Layer();

        int GetSprite(int x, int y);
        void SetSprite(int x, int y, int tileSprite);
        //Cleanup: int GetX(int x, int y);
        //Cleanup: int GetY(int x, int y);

        //void Update();
        //void Draw();
};

#endif // LAYER_H_INCLUDED
