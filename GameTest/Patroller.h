
#pragma once
#include "app\app.h"

enum PatrollerState {
    PATROLLER_INACTIVE,
    PATROLLER_CHASING,
    PATROLLER_LEAVING
};

class Patroller {
private:
    CSimpleSprite* m_sprite;
    CSimpleSprite* m_questionIcon;
    PatrollerState m_state;
    bool m_isHovering;

    float m_hideTimer;
    float m_outOfRangeTimer;
    float m_caughtTimer;

    static constexpr float ms_SPEED = 5.0f;
    static constexpr float ms_CHASE_RANGE = 600.0f;
    static constexpr float ms_CATCH_RADIUS = 80.0f;
    static constexpr float ms_HIDE_TIME = 3.0f;
    static constexpr float ms_OUT_OF_RANGE_TIME = 7.0f;
    static constexpr float ms_CAUGHT_FREEZE_TIME = 2.0f;

    bool m_playerCaught;
    int  m_charismaticLives;
    bool m_justEscaped;

public:
    Patroller();
    ~Patroller();

    void Activate();
    void Update(float deltaTime, float playerX, float playerY, bool playerInClump, float camX, int heatLevel);
    void Render(float camX, float camY);

    bool IsPlayerCaught()   const { return m_playerCaught; }
    bool JustHadEscape()          { bool v = m_justEscaped; m_justEscaped = false; return v; }
    int  GetLivesRemaining() const { return m_charismaticLives; }
    void Reset() { m_playerCaught = false; m_justEscaped = false; m_charismaticLives = 2; m_state = PATROLLER_INACTIVE; m_sprite->SetPosition(-300.0f, 300.0f); }
    bool IsInactive()       const { return m_state == PATROLLER_INACTIVE; }
    PatrollerState GetState() const { return m_state; }
};