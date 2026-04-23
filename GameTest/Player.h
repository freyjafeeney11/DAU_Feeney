// Player.h
#pragma once
#include "app\app.h"

class Player {
private:
    CSimpleSprite* m_sprite;
    CSimpleSprite* m_hideSprite;
    bool m_isHiding;

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
    bool IsHiding() const { return m_isHiding; }
    void SetColor(float r, float g, float b) { m_sprite->SetColor(r, g, b); }
    void Update(float deltaTime);
    void SetPosition(float x, float y);
    void Render(float camX, float camY, bool hidden);
    void GetPosition(float& x, float& y);
};