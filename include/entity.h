#ifndef ENTITY_H
#define ENTITY_H

#include "Globals.h"
#include "timeController.h"
#include "AnimationSet.h"

// Abstract class
class Entity
{
public:
    // reference constant for directions;
    static const int DIRECT_UP, DIRECT_DOWN, DIRECT_LEFT, DIRECT_RIGHT, DIRECT_NONE;
    // quick label to see what entity is up to
    int state;
    float x, y;
    int direction;
    // Is the thing can pass me
    bool solid = true;
    bool collisionWithSolids = true;
    // Entity turns on and off
    bool active = true;
    // what type of entity is it?
    string type = "entity";
    // is the entity moving
    bool moving;
    // angle to move entity (360 degree)
    float angle;
    float movingSpeed;
    // Direction pushed in
    float movingSpeedMax;
    // amount of push in the slideAngle
    float slideAngle;
    float slideAmount;
    float moveLerp = 4;
    // Keep track of total x and y, movement per turn. Just in case we need to retract movement
    float totalXMove, totalYMove;
    // box describing the size of our entity and this is used to bump into things.
    SDL_Rect collisionBox;
    // Where the collision box was last
    SDL_Rect lastCollisionBox;
    // our default collisionBox sizes
    int collisionBoxW, collisionBoxH;
    // From my entity y value, where should I draw this collisionBox
    float collisionBoxYOffset;
    // Set of all animations this entity can have
    AnimationSet *animSet;
    // Current animation this entity is using
    Animation *currentAnim;
    // The current frame in the above animation the entity using
    Frame *currentFrame;
    // help animate frame to frame
    float frameTimer;

    // VIRTUAL FUNCTIONS
    virtual void update();
    virtual void draw();
    virtual void move(float angle);
    virtual void updateMovement();
    virtual void updateCollisionBox();
    // Abstract function
    virtual void changeAnimation(int newState, bool resetFrameToBeginning) = 0;
    // How we bump into stuff in the world
    virtual void updateCollsions();

    // Helper function
    static float distanceBetweenTwoRects(SDL_Rect &r1, SDL_Rect &r2);
    static float distancebetweenTwoEntities(Entity *e1, Entity *e2);
    static float anglebetweenTwoEntities(Entity *e1, Entity *e2);
    static bool checkCollision(SDL_Rect cbox1, SDL_Rect cbox2);
    static int angleToDirection(float angle);
    static float angleBetweenTwoPoints(float cx1, float cy1, float cx2, float cy2);
    static float angleBetweenTwoRects(SDL_Rect &r1, SDL_Rect &r2);
    // global entities list I can refer to at anytime
    static list<Entity *> entities;
    // Compare two entity on the list to help sorting(sorts base on the y)
    static bool EntityCompare(const Entity *const &a, const Entity *const &b);
    static void removeInactiveEntitiesFromList(list<Entity *> *entityList, bool deleteEntities);
    static void removeAllFromList(list<Entity *> *entityList, bool deleteEntities);
};

#endif