#pragma once
#include <SFML/Graphics.hpp>
#include "enemy.h"
#include <Box2D/Box2D.h>
#include <vector>
bool isSolid(int tileNum);
char getDisplayChar(int tileNum);

using namespace sf;

class groundTileMap : public sf::Drawable, public sf::Transformable
{
public:
    void genGroundTileMap (const char* filename, sf::Texture nTexture
                           , int tilesW, int tilesH
                           , int textureTileGridWidth, b2World *world, int nScale
                           , Texture nEnemyTexture, Texture bulletTexture);
    std::vector<Enemy> getEnemys();

private:
    virtual void draw (sf::RenderTarget& target, sf::RenderStates states) const;
    int width, height, SCALE;
    sf::VertexArray vertices;
    sf::Texture texture,
        enemyTexture;
    std::vector<Enemy> enemys;
    int tilesWidth, tilesHeight;
};
