// Player.h
#pragma once
#include "app\app.h"

class Player {
private:
    CSimpleSprite* m_sprite;

    static constexpr float ms_WALK_SPEED = 3.5f;
    static constexpr float ms_RUN_SPEED = 7.0f;

    enum {
        ANIM_IDLE,
        ANIM_HIDE,
        ANIM_WALK,
        ANIM_STEAL,
        ANIM_RUN
    };

public:
    Player();
    ~Player();

    void Update(float deltaTime);
    void Render(float camX, float camY);
    void GetPosition(float& x, float& y);
};