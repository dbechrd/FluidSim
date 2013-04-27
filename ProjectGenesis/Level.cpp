#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "Level.h"
#include "Tile.h"
#include "RenderEngine.h"
#include "ObjectEngine.h"

using namespace std;

Level::Level(const std::string &file_map) {
    camera = Camera::Instance()->Rect();

    //Open the map
    std::ifstream mapStream(file_map.c_str());

    //If the map couldn't be loaded
    if(mapStream == NULL) {
        std::cout << "Map Load Error" << std::endl;
    }
    mapFileName = file_map;

	std::string header;
	do {
		getline(mapStream, header);
	} while(header[0] == '#');

	levelRect.left = 0;
	levelRect.top = 0;

	std::stringstream headerStream(header);
	headerStream >> spritesheetId;
	headerStream >> levelRect.width;
	headerStream >> levelRect.height;
    headerStream >> levelLayers;

	if(headerStream.fail()) {
        mapStream.close();
        std::cout << "Parameter error" << std::endl << "Expected Format: [spritesheetId levelWidth levelHeight numberOfLayers]" << std::endl;
		return;
    }

	std::cout << "Creating Level" << std::endl;
	std::cout << "{spritesheetId, width, height, layers}" << std::endl;
	std::cout << "{" << spritesheetId << ", " << LevelWidth() << ", " << LevelHeight() << ", " << levelLayers << "}" << std::endl;

    ////Load the tilesheet and get the number of sprites
    //tileSprites = tileSheet.Load(tileDataFileName, tileWidth, tileHeight);
    //std::cout << "Load tilesheet.. " << tileSprites << " sprites" << std::endl;

	Spritesheet* ss = RenderEngine::Instance()->LoadSpritesheet(spritesheetId);

	//tileColumn = LevelHeight() / tileHeight;
	//tileRow = LevelWidth() / tileWidth;
	//totalTiles = tileColumn * tileRow;

	const sf::Vector2i* frameSize = ss->FrameSize();
	tileWidth = frameSize->x;
	tileHeight = frameSize->y;
	tileRow = LevelWidth() / frameSize->x;
	tileColumn = LevelHeight() / frameSize->y;
	totalTiles = tileColumn * tileRow;
	tileSprites = ss->FrameCount();

	tiles = 0;
	objects = 0;
	tileCollection = new Tile*[MAX_TILES];
	objCollection = new ObjectData*[MAX_OBJECTS];

	////////////////////////////////////////////////////////////////////////////////////
    //Load tile data

	//TODO: Factor out this call to SQLiteExec(char* zSQL, Callback, this)
	char* zErrMsg = NULL;
	char *zSQL = sqlite3_mprintf("SELECT * FROM Tile WHERE SpritesheetId = %d", spritesheetId);
	if(sqlite3_exec(RenderEngine::db, zSQL, LoadTileCB, this, &zErrMsg) != SQLITE_OK) {
		printf("SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	sqlite3_free(zSQL);

	////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////
    //Load object data

	//TODO: Factor out this call to SQLiteExec(char* zSQL, Callback, this)
	zErrMsg = NULL;
	zSQL = sqlite3_mprintf("SELECT * FROM Object WHERE SpritesheetId = %d", spritesheetId);
	if(sqlite3_exec(RenderEngine::db, zSQL, LoadObjectCB, this, &zErrMsg) != SQLITE_OK) {
		printf("SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	sqlite3_free(zSQL);

	////////////////////////////////////////////////////////////////////////////////////

	//The tile offsets
    int x, y;
    int t;

    //Load tile layers
    for(int l = 0; l < levelLayers; l++) {
		x = 0;
        y = 0;

        layerData.push_back(new Layer(LevelWidth(), LevelHeight(), tileWidth, tileHeight));
        for(t = 0; t < totalTiles; t++) {
            
			int tileType;
            mapStream >> tileType;

            if(mapStream.fail()) {
                mapStream.close();
                std::cout << "Map Tile Read Error" << std::endl;
				return;
            }

            if((tileType >= -1) && (tileType < tileSprites)) {
                layerData[l]->SetSprite(x, y, tileType);
            }

            x++;
            if(x >= tileRow) {
                x = 0;
                y++;
            }
        }
    }

    //Load objects
    x = 0;
    y = 0;

    cout << "Loading Level Objects\n";

	Object* newobj;
    for(t = 0; t < totalTiles; t++) {
        int objType;
        mapStream >> objType;

        if(mapStream.fail() == true) {
            mapStream.close();
            std::cout << "Map Object Read Error" << std::endl;
        }

        if((objType > -1) && (objType < tileSprites)) {
            //newobj = objEngine->AddObject(objType, x, y);
            //objData[x][y] = newobj;
			newobj = new Object(objCollection[objType], sf::Vector2f((float)(x * tileWidth), (float)(y * tileHeight)));
			gameObjects.push_back(newobj);
        }

        x++;

        if(x >= tileRow) {
            x = 0;
            y++;
        }
    }

    //Close the file
    mapStream.close();
    activeLayer = 0;
    cout << "Level Created\n";
}

Level::~Level() {
	int i;
    for(i = layerData.size() - 1; i >= 0; --i) {
        if(layerData[i] != NULL) {
            delete layerData[i];
            layerData[i] = NULL;
        }
    }
    layerData.clear();

    for(i = gameObjects.size() - 1; i >= 0; --i) {
        delete gameObjects[i];
        gameObjects[i] = NULL;
    }
    gameObjects.clear();

	for(i = tiles - 1; i >= 0; --i) {
        delete tileCollection[i];
		tileCollection[i] = NULL;
    }
	delete tileCollection;
	tileCollection = NULL;

	for(i = objects - 1; i >= 0; --i) {
        delete objCollection[i];
		objCollection[i] = NULL;
    }
	delete objCollection;
	objCollection = NULL;
}

int Level::LoadTileCB(void *instance, int argc, char **argv, char **azColName) {
	Level* me = ((Level*)instance);

	int i;
	for(i=0; i<argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
		std::cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << "\n";
	}
	printf("\n");
	std::cout << "\n";

	int spritesheetId = atoi(argv[0]);
	int spriteId = atoi(argv[1]);
	bool collision = atoi(argv[2]) > 0;

	me->tileCollection[spriteId] = new Tile(spritesheetId, spriteId, collision);
	me->tiles++;

	return SQLITE_OK;
}

int Level::LoadObjectCB(void *instance, int argc, char **argv, char **azColName) {
	Level* me = ((Level*)instance);

	int i;
	for(i=0; i<argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
		std::cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << "\n";
	}
	printf("\n");
	std::cout << "\n";
	
	int objectId = atoi(argv[0]);
	int spritesheetId = atoi(argv[1]);
	int spriteId = argv[2] ? atoi(argv[2]) : -1;
	std::string name = argv[3];
	bool collide = atoi(argv[4]) > 0;

	//TODO: Create new Object() which inherits from ??
	ObjectData* obj = new ObjectData();
	obj->id = objectId;
	obj->spritesheetId = spritesheetId;
	obj->spriteId = spriteId;
	obj->name = name;
	obj->collide = collide;
	obj->flags = 0;
	me->objCollection[objectId] = obj;
	me->objects++;

	/*
	me->spritesheet = me->renderer->LoadSpritesheet(spritesheetId);
	me->animation = me->spritesheet->Animation(1);
    me->setTexture(*me->spritesheet->Texture());
	me->setTextureRect(*me->TextureRect());
	me->name = name;
	me->NPC = (NPC != 0);
	me->walkSpeed = walkSpeed;
	*/

	return SQLITE_OK;
}

void Level::ToggleLayer(int layer) {
    if(layer <= (int)layerData.size()) {
        if(layerData[layer]->visible == false) {
            layerData[layer]->visible = true;
            Draw();
        }
        else if(layerData[layer]->visible == true) {
            layerData[layer]->visible = false;
            Draw();
        }
    }
}

void Level::Update() {
	int i;
	//Cleanup
    /*for(int l = 0; l < levelLayers; l++) {
        if(layerData[l]->visible == true) {
            layerData[l]->Update();
        }
    }*/

	for(i = 0; i < tiles; ++i) {
		tileCollection[i]->Update();
	}

	for(i = gameObjects.size() - 1; i >= 0; --i) {
		gameObjects[i]->Update();
	}
}

//OBJECT DRAW FUNCTION
void Level::Draw(int drawCode) {
    if(drawCode == 0 || drawCode == 1) {
        //Layer Floor Draw
		DrawLayer(layer_Floor);

        //Character Draw
        //NULL

        //Layer Walls Draw
		DrawLayer(layer_Walls);

        //Layer Objects Draw
        //TODO: FIX ME: objEngine->DrawObjects(&tileSheet);
		for(int i = gameObjects.size() - 1; i >= 0; --i) {
			gameObjects[i]->Draw();
		}
    }
    if(drawCode == 0 || drawCode == 2) {
        //Layer Roof Draw
		DrawLayer(layer_Roof);
    }
}

void Level::DrawLayer(LayerType layer) {
    int xAdd = (camera->left) / tileWidth;							//-150 / 32 = -4
    int yAdd = (camera->top) / tileHeight;							//-200 / 32 = -6
    int xMax = ((camera->left + camera->width) / tileWidth) + 1;	//-150 + 640 / 32 = 16
    int yMax = ((camera->top + camera->height) / tileHeight) + 1;	//-150 + 480 / 32 = 11

	int sprite;

    for(int j = 0; j < (camera->width / tileWidth) + 1; j++)		//20
    {
        for(int k = 0; k < (camera->height / tileHeight) + 1; k++)	//15
        {
            if(j + xAdd >= 0 && j + xAdd < xMax && j + xAdd < LevelWidth() / tileWidth && k + yAdd >= 0 && k + yAdd < yMax && k + yAdd < LevelHeight() / tileHeight) {
				sprite = layerData[layer]->GetSprite(j + xAdd, k + yAdd);
				if(sprite >= 0 && sprite < tileSprites) {
                    RenderEngine::Instance()->Draw((j + xAdd) * tileWidth, (k + yAdd) * tileHeight, *tileCollection[sprite]);
                }
            }
        }
    }
}

int Level::TileWidth() {
    return tileWidth;
}

int Level::TileHeight() {
    return tileHeight;
}

int Level::LevelWidth() {
	return levelRect.width;
}

int Level::LevelHeight() {
	return levelRect.height;
}

sf::IntRect* Level::LevelRect() {
	return &levelRect;
}

sf::Texture* Level::GetTileSheet() {
    return NULL; //TODO: FIX ME: tileSheet.GetSurface();
}

int Level::GetTileNumber(int x, int y) {
    int tempa = x / tileWidth;
    int tempb = y / tileHeight;

    int tilenum = tempa + tempb * tileRow;
    return tilenum;
}

//Cleanup: Unused
//sf::IntRect Level::GetTileCoords(int x, int y) {
//    x = x / tileWidth;
//    y = y / tileHeight;
//
//    sf::IntRect coords;
//    coords.left = layerData[activeLayer]->GetX(x, y) * tileWidth;
//    coords.top = layerData[activeLayer]->GetY(x, y) * tileHeight;
//
//    return coords;
//}

int Level::SetTileSprite(int x, int y, int sprite) {
    x = x / tileWidth;
    y = y / tileHeight;

    if((sprite >= -1) && (sprite < tileSprites)) {
        layerData[activeLayer]->SetSprite(x, y, sprite);
		RenderEngine::Instance()->Draw(*tileCollection[layerData[activeLayer]->GetSprite(x, y)]);
    }else{
        return 1;
    }

    return 0;
}

//Take pixel coords
int Level::GetTileSprite(int x, int y) {
    x = x / tileWidth;
    y = y / tileHeight;

    int sprite = layerData[activeLayer]->GetSprite(x, y);

    return sprite;
}

//Take index coords
Object* Level::GetObject(const sf::IntRect& charRect) {
	//TODO:
	for(int i = gameObjects.size() - 1; i >= 0; --i) {
		if(gameObjects[i]->GetCollide() && gameObjects[i]->ColRect().intersects(charRect))
			return gameObjects[i];
	}

	return NULL;
}

//Take index coords
bool Level::IsCollision(int x, int y) {
    /*if(x < 0 || y < 0 || x >= (levelWidth / tileWidth) || y >= (levelHeight / tileHeight)) {
        return true;
    }*/

	//Collision
	int floor = layerData[layer_Floor]->GetSprite(x, y);
	if(floor >= 0 && tileCollection[floor]->IsSolid()) {
        return true;
    }

	int wall = layerData[layer_Walls]->GetSprite(x, y);
    if(wall >= 0 && tileCollection[wall]->IsSolid()) {
        return true;
    }

    /*if(objData[x][y] != NULL && objData[x][y]->GetCollide()) {
        return true;
    }*/
    return false;
}

//int Level::Save() {
//    remove(mapFileName.c_str());
//    std::ofstream mapStream(mapFileName.c_str(), std::ios::app);
//
//    //If the map couldn't be loaded
//    if(mapStream == NULL) {
//        cout << "Map Save Error";
//        return 0;
//    }
//
//    //write tilesheet name and other attribute to map file
//    mapStream << tileDataFileName << std::endl;
//    mapStream << LevelWidth() << " " << LevelHeight() << " " << levelLayers << " " << tileWidth << " " << tileHeight << " " << tileSprites << " -1\n";
//
//    for(int i = 0; i < (int)layerData.size(); i++) {
//        for(int j = 0; j < tileColumn; j++) {
//            for(int k = 0; k < tileRow; k++) {
//                int test = layerData[i]->GetSprite(k, j);
//                if(test >= 0 && test < 10) {
//                    mapStream << "0" << test << " ";
//                }else{
//                    mapStream << test << " ";
//                }
//            }
//            mapStream << std::endl;
//        }
//        mapStream << std::endl << std::endl;
//    }
//    mapStream.close();
//	return 1;
//}
