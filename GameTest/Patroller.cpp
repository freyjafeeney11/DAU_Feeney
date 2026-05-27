
#include "stdafx.h"
#include "Patroller.h"
#include "PlayerArchetype.h"
#include <math.h>

Patroller::Patroller() :
    m_state(PATROLLER_INACTIVE),
    m_hideTimer(0.0f),
    m_outOfRangeTimer(0.0f),
    m_caughtTimer(0.0f),
    m_playerCaught(false),
    m_charismaticLives(2),
    m_justEscaped(false)
{
    m_sprite = App::CreateSprite(".\\TestData\\patroller.png", 4, 1);
    m_sprite->CreateAnimation(0, 0.15f, { 0, 1, 2, 3 });
    m_sprite->SetAnimation(0);
    m_sprite->SetScale(0.2f);
    m_sprite->SetPosition(-300.0f, 300.0f);

    m_questionIcon = App::CreateSprite(".\\TestData\\question_mark.png", 1, 1);
    m_questionIcon->SetScale(0.2f);
    m_isHovering = false;
}

Patroller::~Patroller() {
    delete m_sprite;
    delete m_questionIcon;
}

void Patroller::Activate() {
    if (m_state != PATROLLER_INACTIVE) return;

    m_state = PATROLLER_CHASING;
    m_hideTimer = 0.0f;
    m_outOfRangeTimer = 0.0f;
    m_playerCaught = false;
    m_justEscaped  = false;
    m_sprite->SetPosition(-200.0f, 300.0f);
}
void Patroller::Update(float deltaTime, float playerX, float playerY, bool playerInClump, float camX, int heatLevel) {
    if (m_state == PATROLLER_INACTIVE) return;
    float dt = deltaTime / 1000.0f;
    float x, y;
    m_sprite->GetPosition(x, y);

    float currentSpeed = ms_SPEED + (heatLevel * 2.0f);
    m_isHovering = false;

    if (m_state == PATROLLER_CHASING) {
        if (x < playerX) x += currentSpeed;
        else if (x > playerX) x -= currentSpeed;
        m_sprite->SetPosition(x, y);
        float dx = playerX - x;
        float dy = playerY - y;
        float distance = sqrtf(dx * dx + dy * dy);
        if (playerInClump) {
            m_isHovering = true;
            m_hideTimer += dt;
            m_sprite->Update(deltaTime);
            if (m_hideTimer >= ms_HIDE_TIME) {
                m_state = PATROLLER_LEAVING;
                m_isHovering = false;
            }
            return;
        }
        else {
            m_hideTimer = 0.0f;
        }
        if (distance < ms_CATCH_RADIUS) {
            if (PlayerArchetype::current == Archetype::CHARISMATIC && m_charismaticLives > 0) {
                m_charismaticLives--;
                m_justEscaped = true;
                m_state = PATROLLER_LEAVING;
            } else {
                m_playerCaught = true;
            }
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
        x -= currentSpeed;
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

    if (m_isHovering) {
        static float t = 0.0f;
        t += 0.05f;
        float bob = sinf(t) * 4.0f;
        m_questionIcon->SetPosition((actualX - camX) + 1.0f, (actualY - camY) + 150.0f + bob);
        m_questionIcon->Draw();
    }

    m_sprite->SetPosition(actualX, actualY);
}