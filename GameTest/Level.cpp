#include "stdafx.h"
#include "Level.h"
#include <math.h>

Level::Level() {
    m_city = App::CreateSprite(".\\TestData\\city.png", 1, 1);
    m_city->SetPosition(512.0f, 450.0f);
    m_city->SetScale(0.6f);

    m_background = App::CreateSprite(".\\TestData\\train_interior.png", 1, 1);
    m_background->SetPosition(500.0f, 500.0f);
    m_background->SetScale(0.6f);

    m_window = App::CreateSprite(".\\TestData\\cityscape.png", 7, 1);
    m_window->SetPosition(470.0f, 450.0f);
    m_window->SetScale(0.6f);
    m_window->CreateAnimation(0, 0.5f, { 0, 1, 2, 3, 4, 5, 6 });
    m_window->SetAnimation(0);

    m_rain = App::CreateSprite(".\\TestData\\rain.png", 3, 1);
    m_rain->SetPosition(475.0f, 450.0f);
    m_rain->SetScale(0.5f);
    m_rain->CreateAnimation(0, 0.2f, { 0, 1, 2 });
    m_rain->SetAnimation(0);

    m_roamingNPC = App::CreateSprite(".\\TestData\\npc_walk.png", 5, 1);
    m_roamingNPC->SetScale(0.2f);
    m_roamingNPC->CreateAnimation(0, 0.3f, { 0,1,2,3,4 });
    m_roamingNPC->SetAnimation(0);
    m_roamingNPC->SetPosition(-200.0f, -200.0f);

    m_dialogueBox = App::CreateSprite(".\\TestData\\dialogue.png", 1, 1);
    m_dialogueBox->SetPosition(500.0f, 980.0f);
    m_dialogueBox->SetScale(0.6f);

    m_npcActive = false;
    m_npcTimer = 0.0f;
    m_npcSpawnDelay = 3.0f;
    m_npcSpeed = 2.5f;
    m_npcMoveRight = true;

    m_guardSprite = App::CreateSprite(".\\TestData\\ticketman.png", 1, 1);
    m_guardSprite->SetScale(0.4f);

    m_ladderSprite = App::CreateSprite(".\\TestData\\ladder.png", 1, 1);
    m_ladderSprite->SetScale(0.6f);
    m_ladderSprite->SetPosition(LADDER_WORLD_X, LADDER_WORLD_Y);

    m_cityScrollOffset = 0.0f;

    // ticketman
    m_currentCar = 1;
    m_guardState = GuardState::NONE;
    m_guardChoice = 0;
    m_enterWasDown = false;
    m_navDown = false;
    m_msgTimer = 0.0f;
}

Level::~Level() {
    delete m_background;
    delete m_city;
    delete m_dialogueBox;
    delete m_window;
    delete m_rain;
    delete m_roamingNPC;
    delete m_guardSprite;
    delete m_ladderSprite;
}

bool Level::IsPlayerInWalkingNPCVision(float playerX, float playerY) const {
    if (!m_npcActive) return false;
    float nx, ny;
    m_roamingNPC->GetPosition(nx, ny);
    float dx = playerX - nx;
    if (m_npcMoveRight && (dx < 0 || dx > VISION_RANGE)) return false;
    if (!m_npcMoveRight && (dx > 0 || dx < -VISION_RANGE)) return false;
    return true;
}

void Level::RenderWalkingNPCVision(float camX, float camY) const {
    if (!m_npcActive) return;
    float nx, ny;
    m_roamingNPC->GetPosition(nx, ny);
    float sx = nx - camX;
    float sy = (ny + 150.0f) - camY;  // eye height?

#if APP_USE_VIRTUAL_RES
    APP_VIRTUAL_TO_NATIVE_COORDS(sx, sy);
#endif

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_TEXTURE_2D);

    const int NUM_RAYS = 16;
    const float forwardX = m_npcMoveRight ? 1.0f : -1.0f;
    const float TILT_DOWN = 20.0f;  // how far down the cone points

    glBegin(GL_TRIANGLE_FAN);
    glColor4f(1.0f, 0.5f, 0.1f, 0.18f);
    glVertex2f(sx, sy);

    for (int i = 0; i <= NUM_RAYS; i++) {
        float t = (float)i / NUM_RAYS;
        float spread = (t - 0.5f) * 2.0f * VISION_CONE_HALF * VISION_RANGE;
        float vx = nx - camX + forwardX * VISION_RANGE;
        float vy = (ny + 150.0f) - camY + spread - TILT_DOWN;
#if APP_USE_VIRTUAL_RES
        APP_VIRTUAL_TO_NATIVE_COORDS(vx, vy);
#endif
        glVertex2f(vx, vy);
    }

    glEnd();
    glDisable(GL_BLEND);
}

void Level::Update(float deltaTime) {
    m_window->Update(deltaTime);
    m_rain->Update(deltaTime);

    m_cityScrollOffset += 200.0f * (deltaTime / 1000.0f);
    if (m_cityScrollOffset > 1024.0f) m_cityScrollOffset -= 1024.0f;

    m_npcTimer += deltaTime / 1000.0f;

    if (!m_npcActive) {
        if (m_npcTimer >= m_npcSpawnDelay) {
            m_npcTimer = 0.0f;
            m_npcActive = true;
            m_npcMoveRight = (rand() % 2) == 0;
            const float startX = m_npcMoveRight ? -100.0f : 3000.0f;
            m_roamingNPC->SetPosition(startX, 280.0f);
            m_roamingNPC->SetFlipX(m_npcMoveRight);
            m_roamingNPC->SetAnimation(0);
        }
    }
    else {
        float x, y;
        m_roamingNPC->GetPosition(x, y);
        x += m_npcMoveRight ? m_npcSpeed : -m_npcSpeed;
        m_roamingNPC->SetPosition(x, y);
        m_roamingNPC->SetFlipX(!m_npcMoveRight);

        if (x > GUARD_WORLD_X + 50.0f && m_npcMoveRight)  m_npcMoveRight = false;
        if (x < 50.0f && !m_npcMoveRight) m_npcMoveRight = true;

        m_roamingNPC->Update(deltaTime);
    }
}

bool Level::IsPlayerNearGuard(float px) const {
    return fabsf(px - (GUARD_WORLD_X + 200.0f)) < 120.0f;
}

void Level::UpdateGuard(float playerX, int& playerGold, bool& outChangeCar, float deltaTime) {
    if (m_currentCar >= 3) return; // no guard in the final car

    float dt = deltaTime / 1000.0f;
    bool enterDown = App::IsKeyPressed(VK_RETURN);
    bool escDown = App::IsKeyPressed(VK_ESCAPE);

    if (m_guardState == GuardState::NO_FUNDS) {
        m_msgTimer += dt;
        if (m_msgTimer >= 2.0f || escDown) {
            m_guardState = GuardState::NONE;
        }
        return;
    }

    if (m_guardState == GuardState::NONE) {
        if (IsPlayerNearGuard(playerX) && enterDown && !m_enterWasDown) {
            m_guardState = GuardState::PROMPT;
            m_guardChoice = 0;
            m_navDown = false;
        }
    }
    else if (m_guardState == GuardState::PROMPT) {
        if (escDown) {
            m_guardState = GuardState::NONE;
        }
        else {
            if (!m_navDown) {
                if (App::IsKeyPressed(VK_LEFT) || App::IsKeyPressed(VK_RIGHT)) {
                    m_guardChoice = 1 - m_guardChoice;
                    m_navDown = true;
                }
            }
            if (!App::IsKeyPressed(VK_LEFT) && !App::IsKeyPressed(VK_RIGHT)) {
                m_navDown = false;
            }

            if (enterDown && !m_enterWasDown) {
                if (m_guardChoice == 0) {
                    if (playerGold >= 10) {
                        playerGold -= 10;
                        outChangeCar = true;
                        m_currentCar++;
                        m_guardState = GuardState::NONE;
                    }
                    else {
                        m_guardState = GuardState::NO_FUNDS;
                        m_msgTimer = 0.0f;
                    }
                }
                else {
                    m_guardState = GuardState::NONE;
                }
            }
        }
    }
    m_enterWasDown = enterDown;
}

void Level::RenderGuardUI() {
    if (m_guardState == GuardState::NONE) return;

    m_dialogueBox->Draw();

    if (m_guardState == GuardState::NO_FUNDS) {
        App::PrintTTF(169, 710, "You don't have enough gold!", 1.0f, 0.4f, 0.4f, 0);
        return;
    }

    // dialogue text pos
    if (m_guardState == GuardState::PROMPT) {
        App::PrintTTF(169, 730, "Ticketmaster: Ticket for the next car is 10 gold.", 1.0f, 1.0f, 1.0f, 0);
        App::PrintTTF(169, 705, "Pay 10 Gold?", 1.0f, 1.0f, 0.0f, 1);

        float yesR = (m_guardChoice == 0) ? 1.0f : 0.5f;
        float noR = (m_guardChoice == 1) ? 1.0f : 0.5f;
        App::PrintTTF(179, 680, "YES", yesR, yesR, 0.0f, 1);
        App::PrintTTF(240, 680, "NO", noR, noR, noR, 1);
        float cursorX = (m_guardChoice == 0) ? 165.0f : 226.0f;
        App::PrintTTF(cursorX, 680, ">", 1.0f, 1.0f, 0.0f, 0);
    }
}

void Level::RenderBackground(float camX) {
    const float bgWidth = 1075.0f;
    const float windowWidth = 1024.0f;

    const float cityScrollX = m_cityScrollOffset;
    const float windowScrollX = fmodf(camX * 0.3f, windowWidth);
    const float bgScrollX = fmodf(camX, bgWidth);

    m_city->SetPosition(512.0f - cityScrollX, 450.0f);
    m_city->Draw();
    m_city->SetPosition(512.0f - cityScrollX + windowWidth, 450.0f);
    m_city->Draw();

    m_rain->SetPosition(475.0f - windowScrollX, 450.0f);
    m_rain->Draw();
    m_rain->SetPosition(475.0f - windowScrollX + windowWidth, 450.0f);
    m_rain->Draw();

    m_background->SetPosition(500.0f - bgScrollX, 400.0f);
    m_background->Draw();
    m_background->SetPosition(500.0f - bgScrollX + bgWidth, 400.0f);
    m_background->Draw();
}

void Level::RenderForeground(float camX, float camY) {
    if (m_npcActive) {
        float actualX, actualY;
        m_roamingNPC->GetPosition(actualX, actualY);
        m_roamingNPC->SetPosition(actualX - camX, actualY - camY);
        m_roamingNPC->Draw();
        m_roamingNPC->SetPosition(actualX, actualY);
    }

    if (m_currentCar < 3) {
        const float guardScreenX = GUARD_WORLD_X - camX;
        if (guardScreenX > -600.0f && guardScreenX < 1224.0f) {
            m_guardSprite->SetPosition(guardScreenX, GUARD_WORLD_Y - camY);
            m_guardSprite->Draw();
        }
    }

    const float ladderScreenX = LADDER_WORLD_X - camX;
    if (ladderScreenX > -200.0f && ladderScreenX < 1224.0f) {
        m_ladderSprite->SetPosition(ladderScreenX, LADDER_WORLD_Y - camY);
        m_ladderSprite->Draw();
        m_ladderSprite->SetPosition(LADDER_WORLD_X, LADDER_WORLD_Y);
    }
}