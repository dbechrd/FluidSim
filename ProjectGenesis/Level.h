#ifndef LEVEL_H_INCLUDED
#define LEVEL_H_INCLUDED

#include <vector>
#include <string>
#include "SFML/Graphics.hpp"
#include "Layer.h"
#include "Object.h"
#include "Camera.h"
#include "ObjectEngine.h"

enum LayerType { layer_Floor, layer_Walls, layer_Roof, layer_Objects };

#define MAX_TILES 256
#define MAX_OBJECTS 256

class Level {
    private:
		int spritesheetId;

		sf::IntRect levelRect;
        int tileWidth;
        int tileHeight;
        int tileSprites;
        int tileColumn;
        int tileRow;
        int totalTiles;

		//All tile and object types
		int tiles;
		Tile** tileCollection;
		int objects;
        ObjectData** objCollection;

		//In-game tiles
		int levelLayers;
		int activeLayer;
		std::vector<Layer*> layerData;

		//In-game objects
		std::vector<Object*> gameObjects;

        sf::IntRect* camera;
        std::string mapFileName;

		static int LoadTileCB(void *instance, int argc, char **argv, char **azColName);
		static int LoadObjectCB(void *instance, int argc, char **argv, char **azColName);
    public:
        Level(const std::string &file_map);
        ~Level();

		int LevelWidth();
        int LevelHeight();
		sf::IntRect* LevelRect();
		void ToggleLayer(int layer);

        int TileWidth();
        int TileHeight();
        sf::Texture* GetTileSheet();
        int GetTileNumber(int x, int y);
        //Cleanup: Unused: sf::IntRect GetTileCoords(int x, int y);
        int SetTileSprite(int x, int y, int sprite);
        int GetTileSprite(int x, int y);
		int GetTileSprites() { return tileSprites; }

        Object* GetObject(const sf::IntRect& charRect);
        bool IsCollision(int x, int y);

        //int Save();
		void Update();
        void Draw(int drawCode = 0);
		void DrawLayer(LayerType layer);
};

#endif // LEVEL_H_INCLUDED
