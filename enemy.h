#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <vector>

using namespace sf;

class Bullet {
public:
    void start();
    void initialize(Vector2f startPosition
                    , Vector2f startSpeed);
    void update();
    Vector2f getPosition();
    void setPosition(Vector2f nPosition);
    void setSpeed(Vector2f nSpeed);
private:
    Vector2f position;
    Vector2f speed;
};

class Enemy : public Drawable
            , public Transformable {
public:
    void initialize(Vector2f position
                    , Texture enemyTexture
                    , Texture nBulletTexture
                    , b2World* nWorld
                    , int nScale);
    void update();
    void hit();
    bool isAlive();
    void destroy();
private:
    int lifes;
    bool alive;
    int scale;
    Texture texture
        , bulletTexture;
    Sprite sprite;
    /* VertexArray vertices; */
    b2Body* body;
    b2World* world;
    /* std::vector<Bullet> bullets; */
    virtual void draw(RenderTarget& target
                      , RenderStates states) const;

    const static float toDegreesMultiple
        = 57.2957795131;
};
