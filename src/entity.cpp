#include "entity.h"

const int Entity::DIRECT_UP = 0, Entity::DIRECT_DOWN = 1, Entity::DIRECT_LEFT = 2, Entity::DIRECT_RIGHT = 3, Entity::DIRECT_NONE = -1;

// override something useful
void Entity::update() { ; }
// override something specific
void Entity::draw()
{
    // draws current frame
    if (currentFrame != NULL && active)
    {
        currentFrame->draw(animSet->spriteSheet, x, y);
    }
    // draw collision box
    if (solid && Globals::debugging)
    {
        // sets the current drawing color
        SDL_SetRenderDrawColor(Globals::renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawRect(Globals::renderer, &collisionBox);
    }
}
void Entity::move(float angle)
{
    moving = true;
    movingSpeed = movingSpeedMax;
    this->angle = angle;

    int newDirection = angleToDirection(angle);
    // if the direction change, update the current animation
    if (direction != newDirection)
    {
        direction = newDirection;
        changeAnimation(state, false);
    }
}
void Entity::updateMovement()
{
    updateCollisionBox();
    // store collision box before we move
    lastCollisionBox = collisionBox;
    // reset total moves
    totalXMove = 0;
    totalYMove = 0;
    if (movingSpeed > 0)
    {
        // works out move distance using speed, deltaTime(time last loop) and multiply by moveLerp
        float moveDist = movingSpeed * (TimeController::timeController.deltaTime) * moveLerp;
        if (moveDist > 0)
        {

            float xMove = moveDist * (cos(angle * Globals::PI / 180));
            float yMove = moveDist * (sin(angle * Globals::PI / 180));

            x += xMove;
            y += yMove;

            totalXMove = xMove;
            totalYMove = yMove;
            if (!moving)
            {
                movingSpeed -= moveDist;
            }
        }
    }
    // sliding around!
    if (slideAmount > 0)
    {
        float slideDist = slideAmount * TimeController::timeController.deltaTime * moveLerp;
        if (slideDist > 0)
        {
            float xMove = slideDist * (cos(slideAngle * Globals::PI / 180));
            float yMove = slideDist * (sin(slideAngle * Globals::PI / 180));
            x += xMove;
            y += yMove;

            totalXMove = xMove;
            totalYMove = yMove;
            slideAmount -= slideDist;
        }
        else
        {
            slideAmount = 0;
        }
    }
    // Now move collisionBox up to where we are now
    updateCollisionBox();
    // Check the collisionBox
    SDL_UnionRect(&collisionBox, &lastCollisionBox, &collisionBox);
}

void Entity::updateCollisionBox()
{
    collisionBox.x = x - collisionBox.w / 2;
    collisionBox.y = y + collisionBoxYOffset;
    collisionBox.w = collisionBoxW;
    collisionBox.h = collisionBoxH;
}
// How we bump into stuff in the world
void Entity::updateCollsions()
{
    if (active && collisionWithSolids && (movingSpeed > 0 || slideAmount > 0))
    {
        // list of pontential collisions
        list<Entity *> collisions;
        // list<Entity*>::iterator entity;
        for (auto entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++)
        {
            // if we collide with this entity with our currently unioned collisionbox, add to the list
            if ((*entity)->active && (*entity)->type != this->type && (*entity)->solid && Entity::checkCollision(collisionBox, (*entity)->collisionBox))
            {

                // add it to the list
                collisions.push_back(*entity);
            }
        }
        // if we have a list of potential entities we may hit, then lets check them properly to do collision resolution
        if (collisions.size() > 0)
        {
            updateCollisionBox();

            // multisample check for collisions from where we started to where we are planning to go to

            // first we are going to find the sample distance we should travel between checks
            float boxTravelSize = 0;
            if (collisionBox.w < collisionBox.h)
                boxTravelSize = collisionBox.w / 4;
            else
                boxTravelSize = collisionBox.h / 4;

            // use sampleBox to check for collisions from start point to end point, moving at boxTravelSize each sample
            SDL_Rect sampleBox = lastCollisionBox;
            float movementAngle = Entity::angleBetweenTwoRects(lastCollisionBox, collisionBox);

            bool foundCollision = false;
            while (!foundCollision)
            {
                // check samplebox for collisions where it is now
                SDL_Rect intersection;
                for (auto entity = collisions.begin(); entity != collisions.end(); entity++)
                {
                    if (SDL_IntersectRect(&sampleBox, &(*entity)->collisionBox, &intersection))
                    {
                        foundCollision = true;
                        movingSpeed = 0;
                        moving = false;
                        slideAngle = anglebetweenTwoEntities((*entity), this);

                        // currently intersecting a entity, now we need to do collsion resolution
                        if (intersection.w < intersection.h)
                        {
                            if (lastCollisionBox.x + lastCollisionBox.w / 2 < (*entity)->collisionBox.x + (*entity)->collisionBox.w / 2)
                                sampleBox.x -= intersection.w; // started on left, so move left out of collision
                            else
                                sampleBox.x += intersection.w; // otherwise, started on right
                        }
                        else
                        {
                            if (lastCollisionBox.y + lastCollisionBox.h / 2 < (*entity)->collisionBox.y + (*entity)->collisionBox.h / 2)
                                sampleBox.y -= intersection.h; // started abovem so move up out of collision
                            else
                                sampleBox.y += intersection.h; // otherwise, started below
                        }
                    }
                }

                // if collisionsfound or sampleBox is at same place as collisionBox, exit loop
                if (foundCollision || (sampleBox.x == collisionBox.x && sampleBox.y == collisionBox.y))
                    break;

                // move sample box up to check the next spot
                if (distanceBetweenTwoRects(sampleBox, collisionBox) > boxTravelSize)
                {
                    float xMove = boxTravelSize * (cos(movementAngle * Globals::PI / 180));
                    float yMove = boxTravelSize * (sin(movementAngle * Globals::PI / 180));

                    sampleBox.x += xMove;
                    sampleBox.y += yMove;
                }
                else
                {
                    sampleBox = collisionBox;
                }
            }

            if (foundCollision)
            {
                // move our entity to where the sampleBox ended up
                slideAmount = slideAmount / 2;
                x = sampleBox.x + sampleBox.w / 2;
                y = sampleBox.y - collisionBoxYOffset;
            }

            updateCollisionBox();
        }
    }
}

// Helper function
float Entity::distanceBetweenTwoRects(SDL_Rect &r1, SDL_Rect &r2)
{
    SDL_Point e1, e2;
    e1.x = r1.x + r1.w / 2;
    e1.y = r1.y + r1.h / 2;

    e2.x = r2.x + r2.w / 2;
    e2.y = r2.y + r2.h / 2;

    float d = abs(sqrt(pow(e2.x - e1.x, 2) + pow(e2.y - e1.y, 2)));
    return d;
}
float Entity::distancebetweenTwoEntities(Entity *e1, Entity *e2)
{
    float d = abs(sqrt(pow(e2->x - e1->x, 2) + pow(e2->y - e1->y, 2)));
    return d;
}
float Entity::anglebetweenTwoEntities(Entity *e1, Entity *e2)
{
    float dx, dy;
    float x1 = e1->x, y1 = e1->y, x2 = e2->x, y2 = e2->y;

    dx = x2 - x1;
    dy = y2 - y1;

    return atan2(dy, dx) * 180 / Globals::PI;
}
bool Entity::checkCollision(SDL_Rect cbox1, SDL_Rect cbox2)
{
    if (SDL_IntersectRect(&cbox1, &cbox2, nullptr))
    {
        return true;
    }

    // if a rectangle is in another rectangle
    // do it here

    return false;
}
int Entity::angleToDirection(float angle)
{
    if ((angle >= 0 && angle <= 45) || angle >= 315 && angle <= 360)
        return DIRECT_RIGHT;
    else if (angle >= 45 && angle <= 135)
        return DIRECT_DOWN;
    else if (angle >= 135 && angle <= 225)
        return DIRECT_LEFT;
    else
        return DIRECT_UP;
}
float Entity::angleBetweenTwoPoints(float cx1, float cy1, float cx2, float cy2)
{
    float dx = cx2 - cx1;
    float dy = cy2 - cy1;

    return atan2(dy, dx) * 180 / Globals::PI;
}
float Entity::angleBetweenTwoRects(SDL_Rect &r1, SDL_Rect &r2)
{
    float cx1 = r1.x + (r1.w / 2);
    float cy1 = r1.y + (r1.h / 2);

    float cx2 = r2.x + (r2.w / 2);
    float cy2 = r2.y + (r2.h / 2);

    return angleBetweenTwoPoints(cx1, cy1, cx2, cy2);
}
// global entities list I can refer to at anytime
list<Entity *> Entity::entities;
// Compare two entity on the list to help sorting(sorts base on the y)
bool Entity::EntityCompare(const Entity *const &a, const Entity *const &b)
{
    if (a != 0 && b != 0)
    {
        return (a->y < b->y);
    }
    else
    {
        return false;
    }
}
void Entity::removeInactiveEntitiesFromList(list<Entity *> *entityList, bool deleteEntities)
{
    for (auto entity = entityList->begin(); entity != entityList->end();)
    {
        // if entity is not active
        if (!(*entity)->active)
        {
            if (deleteEntities)
                delete (*entity);
            entity = entityList->erase(entity);
        }
        else
        {
            entity++;
        }
    }
}
void Entity::removeAllFromList(list<Entity *> *entityList, bool deleteEntities)
{
    for (auto entity = entityList->begin(); entity != entityList->end();)
    {
        if (deleteEntities)
            delete (*entity);
        entity = entityList->erase(entity);
    }
}