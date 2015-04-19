#include <SFML/Graphics.hpp>
#include <vector>

using namespace sf;

class Bullet : public Drawable
      , public Transformable {
public:
    Bullet(Vector2f startPosition
           , Vector2f startSpeed
           , b2World* world
           , Texture texture
           , int nScale);
    void update();
private:
    int scale;
    b2Body* body;
    Sprite sprite;
    virtual void draw(RenderTarget& target
                      , RenderStates states);
};

class Enemy : public Drawable
    , public Transformable {
public:
    initialize(Vector2f position
               , Texture enemyTexture
               , Texture nBulletTexture
               , b2World* world
               , int nScale);
    update();
private:
    int scale;
    Texture bulletTexture;
    Sprite sprite;
    b2Body* body;
    b2World* world;
    Vector<Bullet> bullets;
    virtual void draw(RenderTarget& target
                      , RenderStates states);
};
