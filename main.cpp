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
#include <vector>
#include <math.h>
#include <iostream>
#include <time.h>
#include <stdlib.h>

#define SCALE 50

using namespace sf;

void handleEvents(RenderWindow* window);
void simulatePhysics(RenderWindow* window);
void handleInput(RenderWindow* window);
void update(RenderWindow* window);
void draw(RenderWindow* window);

void detectCollision(Enemy enemy, int index);
void checkBulletCollisions();
void playerHit();
void loadSprites();
Vector2f normalize(Vector2f source);
void shoot(Vector2f position, Vector2f speed);

Texture spritesMap, playerSprite
                  , chairTexture;
Texture enemyTexture, bulletTexture;
Sprite enemy;

RenderWindow* rWindow;

groundTileMap tileMap;

b2Vec2 gravity(0.0f, 0.0f);
b2World world(gravity);

CircleShape rect(30);

Vector2f startPosition = Vector2f(100
                                 ,100);
std::vector<int> enemyLifes;

Player player;
Chair chair;

float box2DTimeStep = 1.0f / 60.0f;
int velocityIterations = 8
    , positionIterations = 3;


// Bullets
int playerLifes = 5;

float bulletSpeed = 9.0f;
int reloadFrames = 100;
int bulletsPerEnemy = 10;
int bulletSize = 8;

int currentIndexInBullets = 0;
int totalBullets;

RectangleShape bullet(Vector2f(bulletSize, bulletSize));

std::vector<Bullet> bullets;
std::vector<int> timeTillShoot;

int main() {
    RenderWindow window(VideoMode(50 * 20, 50 * 20), "Float");
    window.setVerticalSyncEnabled(true);
srand(time(NULL));
 
    rWindow = &window;

    loadSprites();

    enemy.setTexture(enemyTexture);

    tileMap.genGroundTileMap("maps/test_map_3.pgm", spritesMap
                             , 25, 25, 4, &world, SCALE
                             , enemyTexture, bulletTexture);
    player.initialize(&world, startPosition, SCALE
                      , 50, playerSprite
                      , chairTexture);
    chair.initialize(chairTexture
                     , &player);

    int enemys = tileMap.getEnemys().size();
    enemyLifes.resize(enemys
                      , 0);
    totalBullets = enemys * bulletsPerEnemy;
    bullets.resize(totalBullets);

    bullet.setFillColor(Color::Red);

    for (int i = 0; i < totalBullets; i++) {
        bullets[i].start();
    }

    timeTillShoot.resize(enemys, reloadFrames);
    for (int i = 0; i < enemys; i++) {
timeTillShoot[i] += rand() % 200 - 100;
    }

    //Debug
    rect.setFillColor(Color::Red);
    rect.setOrigin(30,30);

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
    std::vector<Enemy> enemys = tileMap.getEnemys();
    for (int i = 0; i < enemys.size(); i++) {
        enemys[i].update();
        bool alive = enemyLifes[i] >= 0;
        if (alive) {
            detectCollision(enemys[i], i);
        }
    }
    for (int i = 0; i < totalBullets; i++) {
        bullets[i].update();
    }
    checkBulletCollisions();
    rect.setPosition(chair.getHitPosition());

    // Shoot bullets
    for (int i = 0; i < enemys.size(); i++) {
        if (timeTillShoot[i] < 0 && enemyLifes[i] >= 0) {
            timeTillShoot[i] = reloadFrames + (rand() % 200)
                - 100;
            // Shoot
            Vector2f startPos = enemys[i].getPosition();
            startPos += Vector2f(25,25);
            Vector2f endPos = player.getPosition();
            Vector2f direction = endPos - startPos;
            direction += Vector2f((float) (rand() % 100) / 5000
                ,(float) (rand() % 100) / 5000); 
            Vector2f norm = normalize(direction);
            Vector2f speed = norm * bulletSpeed;
            shoot(startPos, speed);
        } else {
            timeTillShoot[i]--;
        }
    }
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
    std::vector<Enemy> enemys = tileMap.getEnemys();
    for (int i = 0; i < enemys.size(); i++) {
        bool alive = enemyLifes[i] >= 0;
        if (alive) {
            enemy.setPosition(enemys[i].getPosition());
            window->draw(enemy);
        }
    }
    window->draw(player);
    // Draw bullets
    for (int i = 0; i < totalBullets; i++) {
        bullet.setPosition(bullets[i].getPosition());
        window->draw(bullet);
    }
    window->draw(chair);
    // window->draw(rect);
    window->display();
}

void detectCollision(Enemy enemy, int index) {
    Vector2f chairPos = chair.getHitPosition();
    float chairRadius = chair.getRadius();
    Vector2f enemyPos = enemy.getPosition();
    IntRect enemyRect = IntRect(enemyPos.x
                                , enemyPos.y
                                , enemyPos.x + 50
                                , enemyPos.y + 50);
    Vector2f centerEnemyPos = Vector2f(enemyPos.x + 25
                                       , enemyPos.y + 25);
    float enemyRadius = 25;
    Vector2f dif = chairPos - centerEnemyPos;
    float distance = sqrt(dif.x * dif.x
                          + dif.y * dif.y);
    if (distance < chairRadius + enemyRadius
        && chair.hitting) {
        enemyLifes[index]--;
        if (enemyLifes[index] < 0) {
            enemy.destroy();
        }
    }
}

void shoot(Vector2f position
           , Vector2f speed) {
    bullets[currentIndexInBullets++].initialize(position
                                                , speed);
    if (currentIndexInBullets == totalBullets) {
        currentIndexInBullets = 0;
    }
}

void checkBulletCollisions() {
    for (int i = 0; i < totalBullets; i++) {
        Vector2f bulletPos = bullets[i].getPosition();
        IntRect bulletRect = IntRect(bulletPos.x
                                     - (bulletSize / 2)
                                     , bulletPos.y
                                     - (bulletSize / 2)
                                     , bulletSize
                                     , bulletSize);
        Vector2f playerPos = player.getPosition();
        IntRect playerRect = IntRect(playerPos.x
                                     - 25
                                     , playerPos.y
                                     - 25
                                     , 50
                                     , 50);
        if (bulletRect.intersects(playerRect)) {
            playerHit();
            bullets[i].setPosition(Vector2f(-100,-100));
            bullets[i].setSpeed(Vector2f(0,0));
        }
    }
}

void playerHit() {
    playerLifes--;
    if (playerLifes == 0) {
        rWindow->close();
    }
}

Vector2f normalize(Vector2f source)
{
    float length = sqrt((source.x * source.x) + (source.y * source.y));
    if (length != 0)
        return Vector2f(source.x / length, source.y / length);
    else
        return source;
}

void loadSprites() {
    spritesMap.loadFromFile("sprites/spriteMap2.png");
    playerSprite.loadFromFile("sprites/player5.png");
    chairTexture.loadFromFile("sprites/chair5.png");
    enemyTexture.loadFromFile("sprites/enemy1.png");
}
