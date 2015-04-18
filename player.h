#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

using namespace sf;

enum playerState {
      STANDING
    , RUNNING
};

class Player : public Drawable, public Transformable {
public:
    void initialize(b2World* nWorld
                    , Vector2f startPosition
                    , int nScale, int nSize
                    , Texture nTexture);
    void update();
    void move(bool forward);
    void turn(bool left);
private:
    float moveForce, rotationTorque;
    int scale, size;
    Texture texMap;
    Sprite sprite;
    IntRect currentRec;
    b2Body* body;
    b2World* world;
    playerState state;
    const static float toDegreesMultiple = 57.2957795131;

    virtual void draw(RenderTarget& target, RenderStates states) const;
};
