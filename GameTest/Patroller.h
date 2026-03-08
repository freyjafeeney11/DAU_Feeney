// Patroller.h
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
    PatrollerState m_state;

    float m_hideTimer;
    float m_outOfRangeTimer;
    float m_caughtTimer;

    static constexpr float ms_SPEED = 3.0f;
    static constexpr float ms_CHASE_RANGE = 600.0f;
    static constexpr float ms_CATCH_RADIUS = 80.0f;
    static constexpr float ms_HIDE_TIME = 3.0f;
    static constexpr float ms_OUT_OF_RANGE_TIME = 7.0f;
    static constexpr float ms_CAUGHT_FREEZE_TIME = 3.0f; // seconds to show "caught" screen

    bool m_playerCaught;

public:
    Patroller();
    ~Patroller();

    void Activate();
    void Update(float deltaTime, float playerX, float playerY, bool playerInClump, float camX);
    void Render(float camX, float camY);

    bool IsPlayerCaught()   const { return m_playerCaught; }
    bool IsInactive()       const { return m_state == PATROLLER_INACTIVE; }
    PatrollerState GetState() const { return m_state; }
};