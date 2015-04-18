#include "player.h"
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

using namespace sf;

void Player::initialize(
    b2World* nWorld
    , Vector2f startPosition
    , int nScale, int nSize
    , Texture nTexture) {
    world = nWorld;
    scale = nScale;
    size = nSize;

    // Constants
    moveForce = 6.0f;
    rotationTorque = 6.0f;

    // Setup sprite
    texMap = nTexture;
    sprite.setTexture(texMap);
    sprite.setOrigin(25, 25);
    sprite.setPosition(startPosition);

    // Setup body physics
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(startPosition.x / scale
                         , startPosition.y / scale);
    bodyDef.linearDamping = 0.5f;
    bodyDef.angularDamping = 0.5f;
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

void Player::update() {
    b2Vec2 newPlayerPosition = body->GetPosition();
    float angle = body->GetAngle();
    setPosition(newPlayerPosition.x * scale
                , newPlayerPosition.y * scale);
    setRotation(angle * toDegreesMultiple);
}


void Player::move(bool forward) {
    int directionMultiplier =
        forward ? 1 : -1;
    body->ApplyForce(b2Vec2(0, directionMultiplier
                            * moveForce)
                     , body->GetWorldCenter(), true);
}

void Player::turn(bool left) {
    int directionMultiplier =
        left ? 1 : -1;
    body->ApplyForce(b2Vec2(directionMultiplier
                            * moveForce, 0)
                     , body->GetWorldCenter(), true);
}

void Player::draw(RenderTarget& target
                  , RenderStates states) const {
    states.transform *= getTransform();
    target.draw(sprite, states);
}
