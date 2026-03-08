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
    m_sprite = App::CreateSprite(".\\TestData\\patroller.png", 4, 1);// swap with patroller sprite later
    m_sprite->CreateAnimation(0, 0.15f, { 0, 1, 2, 3 });
    m_sprite->SetAnimation(0);
    m_sprite->SetScale(0.2f);
    m_sprite->SetPosition(-300.0f, 300.0f);
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
    float dt = deltaTime / 1000.0f;
    float x, y;
    m_sprite->GetPosition(x, y);
    if (m_state == PATROLLER_CHASING) {
        if (x < playerX) x += ms_SPEED;
        else if (x > playerX) x -= ms_SPEED;
        m_sprite->SetPosition(x, y);
        float dx = playerX - x;
        float dy = playerY - y;
        float distance = sqrtf(dx * dx + dy * dy);
        if (playerInClump) {
            m_hideTimer += dt;
            m_sprite->Update(deltaTime);
            if (m_hideTimer >= ms_HIDE_TIME) {
                m_state = PATROLLER_LEAVING;
            }
            return;
        }
        else {
            m_hideTimer = 0.0f;
        }
        if (distance < ms_CATCH_RADIUS) {
            m_playerCaught = true;
            return;
        }
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
    if (m_state == PATROLLER_CHASING) m_sprite->SetFlipX(x > playerX);
    if (m_state == PATROLLER_LEAVING) m_sprite->SetFlipX(true);
    m_sprite->Update(deltaTime);
}

void Patroller::Render(float camX, float camY) {
    if (m_state == PATROLLER_INACTIVE) return;

    float actualX, actualY;
    m_sprite->GetPosition(actualX, actualY);
    m_sprite->SetPosition(actualX - camX, actualY - camY);
    m_sprite->Draw();
    m_sprite->SetPosition(actualX, actualY);
}