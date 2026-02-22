// Level.h
#pragma once
#include "app\app.h"
#include <stdlib.h>

class Level {
private:
    CSimpleSprite* background;
    CSimpleSprite* window;
    CSimpleSprite* rain;
    CSimpleSprite* roamingNPC;

    // Roaming NPC variables
    bool npcActive;
    float npcTimer;
    float npcSpawnDelay;
    float npcSpeed;
    bool npcMoveRight;

public:
    Level();
    ~Level();

    void Update(float deltaTime);
    void RenderBackground(float camX);
    void RenderForeground(float camX, float camY);
};