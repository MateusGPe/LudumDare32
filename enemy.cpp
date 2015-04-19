#include "enemy.h"
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <iostream>

using namespace sf;

void Enemy::initialize(Vector2f position
                       , Texture enemyTexture
                       , Texture nBulletTexture
                       , b2World* nWorld
                       , int nScale) {
    int size = 50;
    setPosition(position);
    texture = enemyTexture;
    alive = true;

    lifes = 3;

    sprite.setTexture(enemyTexture);
    // vertices.setPrimitiveType(Quads);
    // vertices.resize(4);

    // vertices[2].position = Vector2f(0,0);
    // vertices[0].position = Vector2f(size,0);
    // vertices[1].position = Vector2f(size,size);
    // vertices[3].position = Vector2f(0,size);

    // vertices[2].texCoords = Vector2f(0,0);
    // vertices[0].texCoords = Vector2f(size,0);
    // vertices[1].texCoords = Vector2f(size,size);
    // vertices[3].texCoords = Vector2f(0,size);
    // origin?
    bulletTexture = nBulletTexture;
    world = nWorld;
    scale = nScale;

    // Setup body physics
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position.Set((position.x + (size / 2))
                         / scale
                         , (position.y + (size / 2))
                         / scale);
    bodyDef.linearDamping = 2.0f;
    bodyDef.angularDamping = 2.0f;
    body = world->CreateBody(&bodyDef);

    b2PolygonShape box;
    box.SetAsBox(((float) (size)) / (2 * scale)
                 , ((float) (size)) / (2 * scale));
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &box;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 1.0f;

    body->CreateFixture(&fixtureDef);
}

void Enemy::update() {
    b2Vec2 newPosition = body->GetPosition();
    float angle = body->GetAngle();
    setPosition(newPosition.x * scale
                , newPosition.y * scale);
    setRotation(angle * toDegreesMultiple);
}

void Enemy::hit() {

}

bool Enemy::isAlive() {
    return alive;
}

void Enemy::destroy() {
    world->DestroyBody(body);
}

void Enemy::draw(RenderTarget& target
                     , RenderStates states) const {
    if (alive) {
        states.transform *= getTransform();
        states.texture = &texture;
        target.draw(sprite, states);
    }
}
