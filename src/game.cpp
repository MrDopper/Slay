#include "game.h"

Game::Game()
{
    string resPath = getResourcePath();
    backgroundImage = loadTexture(resPath + "map.png", Globals::renderer);

    // Lists of group the type can have
    list<DataGroupType> dataGroupTypes;

    // collisionbox
    DataGroupType collisionBoxType;
    collisionBoxType.groupName = "collisionBox";
    collisionBoxType.dataType = DataGroupType::DATATYPE_BOX;

    // hitbox
    DataGroupType hitBoxType;
    hitBoxType.groupName = "hitbox";
    hitBoxType.dataType = DataGroupType::DATATYPE_BOX;

    // damage
    DataGroupType dmgType;
    dmgType.groupName = "damage";
    dmgType.dataType = DataGroupType::DATATYPE_NUMBER;

    // Add all the data types to the list
    dataGroupTypes.push_back(collisionBoxType);
    dataGroupTypes.push_back(hitBoxType);
    dataGroupTypes.push_back(dmgType);

    // Set up the animation for hero
    heroAnimate = new AnimationSet();
    heroAnimate->loadAnimationSet("hero.text", dataGroupTypes, true, 0, true);

    // Set up the loading screen for wall
    wallAnimate = new AnimationSet();
    wallAnimate->loadAnimationSet("wall.txt", dataGroupTypes);

    hero = new Hero(heroAnimate);
    hero->invincibleTimer = 0;
    // Make the hero appears in the middle of the screen
    hero->x = Globals::ScreenWidth / 2;
    hero->y = Globals::ScreenHeight / 2;
    // It will move the hero using button
    heroInput.hero = hero;
    // add hero into the game
    Entity::entities.push_back(hero);
    // Build wall around the border
    // build top and bottom border
    int borderSize = 32;
    for (int i = 0; i < Globals::ScreenWidth / borderSize; i++)
    {
        // Top border
        Wall *newWall = new Wall(wallAnimate);
        newWall->x = i * borderSize + borderSize / 2;
        newWall->y = borderSize / 2;
        walls.push_back(newWall);
        Entity::entities.push_back(newWall);

        // Fill the walls with bottom border
        newWall = new Wall(wallAnimate);
        newWall->x = i * borderSize + borderSize / 2;
        newWall->y = Globals::ScreenHeight - borderSize / 2;
        walls.push_back(newWall);
        Entity::entities.push_back(newWall);
    }
    // Draw wall in both side
    for (int i = 1; i < Globals::ScreenHeight / borderSize - 1; i++)
    {
        // left
        Wall *newWall = new Wall(wallAnimate);
        newWall->x = borderSize / 2;
        newWall->y = i * borderSize + borderSize / 2;
        walls.push_back(newWall);
        Entity::entities.push_back(newWall);

        // Fill the walls with bottom border
        newWall = new Wall(wallAnimate);
        newWall->x = Globals::ScreenWidth - borderSize / 2;
        newWall->y = i * borderSize + borderSize / 2;
        walls.push_back(newWall);
        Entity::entities.push_back(newWall);
    }
}

Game::~Game()
{
    cleanup(backgroundImage);
    Entity::removeAllFromList(&Entity::entities, false);
    delete heroAnimate;
    delete wallAnimate;

    delete hero;

    Entity::removeAllFromList(&walls, true);
}
void Game::update()
{
    bool quit = false;

    SDL_Event e;
    // Set the timecontroller before the game starts
    TimeController::timeController.reset();
    while (!quit)
    {
        TimeController::timeController.updatetime();
        Entity::removeInactiveEntitiesFromList(&Entity::entities, false);

        // Check for any event might happen
        while (SDL_PollEvent(&e))
        {
            // Close the window
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            // Keydown
            if (e.type == SDL_KEYDOWN)
            {
                switch (e.key.keysym.scancode)
                {
                case SDL_SCANCODE_ESCAPE:
                    quit = true;
                    break;
                case SDL_SCANCODE_SPACE:
                    hero->revive();
                    break;
                }
            }
            heroInput.update(&e);
        }
        for (list<Entity *>::iterator entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++)
        {
            // update all the entity
            (*entity)->update();
        }
        // draw all entities
        draw();
    }
}
void Game::draw()
{
    // clear screen
    SDL_SetRenderDrawColor(Globals::renderer, 145, 133, 129, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(Globals::renderer);

    // draw the background
    renderTexture(backgroundImage, Globals::renderer, 0, 0);

    // Draw all the entities
    for (list<Entity *>::iterator entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++)
    {
        (*entity)->draw();
    }
    // show it to the screen
    SDL_RenderPresent(Globals::renderer);
}
