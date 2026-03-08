// Level.h
#pragma once
#include "app\app.h"
#include <stdlib.h>

class Level {
private:
    CSimpleSprite* m_background;
    CSimpleSprite* m_city;
    CSimpleSprite* m_window;
    CSimpleSprite* m_rain;
    CSimpleSprite* m_roamingNPC;

    // Roaming NPC variables
    bool m_npcActive;
    float m_npcTimer;
    float m_npcSpawnDelay;
    float m_npcSpeed;
    bool m_npcMoveRight;

public:
    Level();
    ~Level();

    void Update(float deltaTime);
    void RenderBackground(float camX);
    void RenderForeground(float camX, float camY);
};