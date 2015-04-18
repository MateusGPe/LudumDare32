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
    framesRunning = 0;

    // Constants
    moveForce = 20.0f;
    rotationTorque = 2.0f;

    // Setup sprite
    texMap = nTexture;
    sprite.setTexture(texMap);
    sprite.setOrigin(25, 25);
    currentRec = IntRect(0,0,size,size);
    sprite.setTextureRect(currentRec);
    setPosition(startPosition);

    state = STANDING;

    // Setup body physics
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(startPosition.x / scale
                         , startPosition.y / scale);
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

void Player::update() {
    b2Vec2 newPlayerPosition = body->GetPosition();
    float angle = body->GetAngle();
    setPosition(newPlayerPosition.x * scale
                , newPlayerPosition.y * scale);
    setRotation(angle * toDegreesMultiple);

    b2Vec2 linearVelocity = body->GetLinearVelocity();
    float speed = b2Vec2(linearVelocity.x
                         , linearVelocity.y)
        .Normalize();
    if (speed < 0.5) {
        state = STANDING;
    } else {
        state = RUNNING;
    }

    if (state == STANDING) {
        currentRec = IntRect(0,0,50,50);
    } else {
        if ((int) (framesRunning)
            % 2 == 0) {
            currentRec = IntRect(50,0,50,50);
        } else {
            currentRec = IntRect(100,0,50,50);
        }
        framesRunning += (speed / 80);
    }
    sprite.setTextureRect(currentRec);
}


void Player::move(bool forward) {
    float directionMultiplier =
        forward ? 0.5 : -1;
    b2Vec2 force = rotateVec(
        b2Vec2(0, directionMultiplier
            * moveForce)
        , body->GetAngle());
    body->ApplyForce(force
                     , body->GetWorldCenter(), true);
}

void Player::turn(bool left) {
    int directionMultiplier =
        left ? 1 : -1;
    body->ApplyTorque(rotationTorque
                      * directionMultiplier, true);
}

void Player::draw(RenderTarget& target
                  , RenderStates states) const {
    states.transform *= getTransform();
    target.draw(sprite, states);
}

b2Vec2 Player::rotateVec(b2Vec2 vector, float radians) {
    float x = vector.x * cos(radians)
        - vector.y * sin(radians);
    float y = vector.x * sin(radians)
        + vector.y * cos(radians);
    return b2Vec2(x, y);
}
