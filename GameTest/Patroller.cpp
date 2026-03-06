// Patroller.cpp
#include "stdafx.h"
#include "Patroller.h"
#include <math.h>

Patroller::Patroller() :
    m_state(PATROLLER_INACTIVE),
    m_hideTimer(0.0f),
    m_outOfRangeTimer(0.0f),
    m_caughtTimer(0.0f),
    m_playerCaught(false)
{
    m_sprite = App::CreateSprite(".\\TestData\\exclamation.png", 1, 1); // swap with patroller sprite later
    m_sprite->SetScale(0.2f);
    m_sprite->SetPosition(-300.0f, 300.0f); // start offscreen
}

Patroller::~Patroller() {
    delete m_sprite;
}

void Patroller::Activate() {
    if (m_state != PATROLLER_INACTIVE) return;

    m_state = PATROLLER_CHASING;
    m_hideTimer = 0.0f;
    m_outOfRangeTimer = 0.0f;
    m_playerCaught = false;
    m_sprite->SetPosition(-200.0f, 300.0f); // enter from the left
}

void Patroller::Update(float deltaTime, float playerX, float playerY, bool playerInClump, float camX) {
    if (m_state == PATROLLER_INACTIVE) return;

    float dt = deltaTime / 1000.0f; // convert to seconds

    float x, y;
    m_sprite->GetPosition(x, y);
    if (m_state == PATROLLER_CHASING) {

        // move toward player
        if (x < playerX) x += ms_SPEED;
        else if (x > playerX) x -= ms_SPEED;
        m_sprite->SetPosition(x, y);

        float dx = playerX - x;
        float dy = playerY - y;
        float distance = sqrtf(dx * dx + dy * dy);

        // check hiding FIRST - if in clump, patroller can't catch you
        if (playerInClump) {
            m_hideTimer += dt;
            if (m_hideTimer >= ms_HIDE_TIME) {
                m_state = PATROLLER_LEAVING;
            }
            return; // early return
        }
        else {
            m_hideTimer = 0.0f;
        }

        // only check catch if NOT in clump
        if (distance < ms_CATCH_RADIUS) {
            m_playerCaught = true;
            return;
        }

        // check out of range
        if (distance > ms_CHASE_RANGE) {
            m_outOfRangeTimer += dt;
            if (m_outOfRangeTimer >= ms_OUT_OF_RANGE_TIME) {
                m_state = PATROLLER_LEAVING;
            }
        }
        else {
            m_outOfRangeTimer = 0.0f;
        }
    }

    if (m_state == PATROLLER_LEAVING) {
        x -= ms_SPEED;
        m_sprite->SetPosition(x, y);
        if (x < -200.0f) {
            m_state = PATROLLER_INACTIVE;
            m_sprite->SetPosition(-300.0f, 300.0f);
        }
    }
}

void Patroller::Render(float camX, float camY) {
    if (m_state == PATROLLER_INACTIVE) return;

    float actualX, actualY;
    m_sprite->GetPosition(actualX, actualY);
    m_sprite->SetPosition(actualX - camX, actualY - camY);
    m_sprite->Draw();
    m_sprite->SetPosition(actualX, actualY);
}