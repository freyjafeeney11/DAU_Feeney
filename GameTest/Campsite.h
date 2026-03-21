#pragma once
#include "app\app.h"

class Rooftop {
private:
    CSimpleSprite* m_background;
    CSimpleSprite* m_nightsky;
    CSimpleSprite* m_daysky;
    CSimpleSprite* m_dayclouds;
    CSimpleSprite* m_nightclouds;
    CSimpleSprite* m_tentflaps;
    CSimpleSprite* m_tentsleep;
    CSimpleSprite* m_plant;
    CSimpleSprite* m_campfire;

    float m_cloudOffset;
    bool  m_playerInTentZone;
    bool  m_playerSleeping;
    bool  m_enterWasDown;

    static constexpr float HATCH_X = 200.0f;
    static constexpr float HATCH_Y = 250.0f;
    static constexpr float HATCH_RADIUS = 120.0f;
    static constexpr float PLAYER_SPAWN_X = 200.0f;
    static constexpr float PLAYER_SPAWN_Y = 250.0f;
    static constexpr float TENT_TRIGGER_X = 700.0f;
    static constexpr float CLOUD_WIDTH = 1024.0f;
    static constexpr float CLOUD_SPEED = 80.0f;

public:
    Rooftop();
    ~Rooftop();

    void Update(float deltaTime, float playerX);
    void Render();
    void RenderPlant();

    bool IsPlayerNearHatch(float px) const;
    bool IsSleeping()               const { return m_playerSleeping; }
    float GetSpawnX()               const { return PLAYER_SPAWN_X; }
    float GetSpawnY()               const { return PLAYER_SPAWN_Y; }
};