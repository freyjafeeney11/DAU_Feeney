#pragma once
#include "app\app.h"

class Rooftop {
private:
    CSimpleSprite* m_background;

    static constexpr float HATCH_X = 200.0f;
    static constexpr float HATCH_Y = 250.0f;
    static constexpr float HATCH_RADIUS = 120.0f;
    static constexpr float PLAYER_SPAWN_X = 200.0f;
    static constexpr float PLAYER_SPAWN_Y = 250.0f;

public:
    Rooftop();
    ~Rooftop();

    void Update(float deltaTime);
    void Render();

    bool IsPlayerNearHatch(float px) const;
    float GetSpawnX() const { return PLAYER_SPAWN_X; }
    float GetSpawnY() const { return PLAYER_SPAWN_Y; }
};