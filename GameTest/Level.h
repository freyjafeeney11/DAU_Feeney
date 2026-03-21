#pragma once
#include "app\app.h"
#include <stdlib.h>

enum class SceneState {
    MAIN_MENU,
    INTRO,
    TRAIN_INTERIOR,
    ROOFTOP
};

class Level {
private:
    CSimpleSprite* m_background;
    CSimpleSprite* m_city;
    CSimpleSprite* m_window;
    CSimpleSprite* m_rain;

    CSimpleSprite* m_roamingNPC;
    bool  m_npcActive;
    float m_npcTimer;
    float m_npcSpawnDelay;
    float m_npcSpeed;
    bool  m_npcMoveRight;

    CSimpleSprite* m_guardSprite;
    static constexpr float BG_WIDTH = 1075.0f;
    static constexpr float GUARD_WORLD_X = 2.0f * BG_WIDTH + 695.0f;
    static constexpr float GUARD_WORLD_Y = 370.0f;

    CSimpleSprite* m_ladderSprite;
    static constexpr float LADDER_WORLD_X = 2750.0f;
    static constexpr float LADDER_WORLD_Y = 467.0f;

    float m_cityScrollOffset;

public:
    Level();
    ~Level();

    void Update(float deltaTime);
    void RenderBackground(float camX);
    void RenderForeground(float camX, float camY);

    float GetLadderX() const { return LADDER_WORLD_X; }
    float GetLadderY() const { return LADDER_WORLD_Y; }
};