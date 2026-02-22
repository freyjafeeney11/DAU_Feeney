#pragma once
#include "app\app.h"

class Player {
private:
    CSimpleSprite* sprite;

    const float WALK_SPEED = 3.0f;
    const float RUN_SPEED = 5.0f;

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