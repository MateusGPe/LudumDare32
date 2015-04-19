/*
A game made for Ludum Dare 2015
Copyright (C) 2015  Marien Raat

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "groundTileMap.h"
#include "player.h"

#define SCALE 50

using namespace sf;

void handleEvents(RenderWindow* window);
void simulatePhysics(RenderWindow* window);
void handleInput(RenderWindow* window);
void update(RenderWindow* window);
void draw(RenderWindow* window);

void loadSprites();

Texture spritesMap, playerSprite
                  , chairTexture;

groundTileMap tileMap;

b2Vec2 gravity(0.0f, 0.0f);
b2World world(gravity);

Vector2f startPosition = Vector2f(100
                                 ,100);

Player player;
Chair chair;

float box2DTimeStep = 1.0f / 60.0f;
int velocityIterations = 8
    , positionIterations = 3;

int main() {
    RenderWindow window(VideoMode(50 * 20, 50 * 20), "Float");
    window.setVerticalSyncEnabled(true);

    loadSprites();

    tileMap.genGroundTileMap("maps/test_map_1.pgm", spritesMap
                             , 50, 50, 4, &world, SCALE);
    player.initialize(&world, startPosition, SCALE
                      , 50, playerSprite
                      , chairTexture);
    chair.initialize(chairTexture
                     , &player);

    while(window.isOpen()) {
        handleEvents(&window);
        simulatePhysics(&window);
        handleInput(&window);
        update(&window);
        draw(&window);
    }
}

void handleEvents(RenderWindow* window) {
    Event event;
    while(window->pollEvent(event)) {
        if (event.type == Event::Closed) {
            window->close();
        }
    }
}

void simulatePhysics(RenderWindow* window) {
    world.Step(box2DTimeStep, velocityIterations
               , positionIterations);
    world.ClearForces();
}

void update(RenderWindow* window) {
    player.update();
    chair.update();
}

void handleInput(RenderWindow* window) {
    float movementForce = 6.0f;
    if (Keyboard::isKeyPressed(Keyboard::W)
        || Keyboard::isKeyPressed(Keyboard::Up)) {
        player.move(false);
    }
    if (Keyboard::isKeyPressed(Keyboard::S)
        || Keyboard::isKeyPressed(Keyboard::Down)) {
        player.move(true);
    }
    if (Keyboard::isKeyPressed(Keyboard::A)
        || Keyboard::isKeyPressed(Keyboard::Left)) {
        player.turn(false);
    }
    if (Keyboard::isKeyPressed(Keyboard::D)
        || Keyboard::isKeyPressed(Keyboard::Right)) {
        player.turn(true);
    }
}

void draw(RenderWindow* window) {
    window->clear(sf::Color(120,170,10));
    window->draw(tileMap);
    window->draw(player);
    window->draw(chair);
    window->display();
}

void loadSprites() {
    spritesMap.loadFromFile("sprites/spriteMap2.png");
    playerSprite.loadFromFile("sprites/player2.png");
    chairTexture.loadFromFile("sprites/chair3.png");
}
