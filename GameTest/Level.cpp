#include "stdafx.h"
#include "Level.h"
#include <math.h>
#include "GearManager.h"

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

    m_roamingNPC2 = App::CreateSprite(".\\TestData\\npc_walk.png", 5, 1);
    m_roamingNPC2->SetScale(0.2f);
    m_roamingNPC2->CreateAnimation(0, 0.3f, { 0,1,2,3,4 });
    m_roamingNPC2->SetAnimation(0);
    m_roamingNPC2->SetPosition(-200.0f, -200.0f);

    m_dialogueBox = App::CreateSprite(".\\TestData\\dialogue.png", 1, 1);
    m_dialogueBox->SetPosition(500.0f, 980.0f);
    m_dialogueBox->SetScale(0.6f);

    m_npcActive = false;
    m_npcTimer = 0.0f;
    m_npcSpawnDelay = 3.0f;
    m_npcSpeed = 2.5f;
    m_npcMoveRight = true;

    m_npcActive2 = false;
    m_npcTimer2 = 0.0f;
    m_npcMoveRight2 = false;

    m_heatLevel = 0;

    m_guardSprite = App::CreateSprite(".\\TestData\\ticketman.png", 1, 1);
    m_guardSprite->SetScale(0.4f);

    m_ladderSprite = App::CreateSprite(".\\TestData\\ladder.png", 1, 1);
    m_ladderSprite->SetScale(0.6f);
    m_ladderSprite->SetPosition(LADDER_WORLD_X, LADDER_WORLD_Y);

    m_cityScrollOffset = 0.0f;

    
    m_questionIcon = App::CreateSprite(".\\TestData\\question_mark.png", 1, 1);
    m_questionIcon->SetScale(0.2f);
    m_alertIcon = App::CreateSprite(".\\TestData\\exclamation.png", 1, 1);
    m_alertIcon->SetScale(0.2f);
    m_npcAlerted = false;

    
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
    delete m_roamingNPC2;
    delete m_guardSprite;
    delete m_questionIcon;
    delete m_alertIcon;
}

bool Level::IsPlayerInWalkingNPCVision(float playerX, float playerY) const {
    bool seen = false;
    if (m_npcActive) {
        float nx, ny;
        m_roamingNPC->GetPosition(nx, ny);
        float dx = playerX - nx;
        if (m_npcMoveRight && (dx >= 0 && dx <= VISION_RANGE)) seen = true;
        if (!m_npcMoveRight && (dx <= 0 && dx >= -VISION_RANGE)) seen = true;
    }
    if (m_npcActive2) {
        float nx, ny;
        m_roamingNPC2->GetPosition(nx, ny);
        float dx = playerX - nx;
        if (m_npcMoveRight2 && (dx >= 0 && dx <= VISION_RANGE)) seen = true;
        if (!m_npcMoveRight2 && (dx <= 0 && dx >= -VISION_RANGE)) seen = true;
    }
    return seen;
}

void Level::RenderWalkingNPCVision(float camX, float camY, bool isColliding) const {
    if (!m_npcActive && !m_npcActive2) return;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_TEXTURE_2D);

    auto drawCone = [&](CSimpleSprite* npc, bool movingRight) {
        float nx, ny;
        npc->GetPosition(nx, ny);
        float sx = nx - camX;
        float sy = (ny + 150.0f) - camY;

#if APP_USE_VIRTUAL_RES
        APP_VIRTUAL_TO_NATIVE_COORDS(sx, sy);
#endif

        const int NUM_RAYS = 16;
        const float forwardX = movingRight ? 1.0f : -1.0f;
        const float TILT_DOWN = 20.0f;
        
        float coneWidth = (m_heatLevel >= 2) ? VISION_CONE_HALF * 2.0f : VISION_CONE_HALF;

        glBegin(GL_TRIANGLE_FAN);

        if (isColliding || m_npcAlerted) {
            glColor4f(1.0f, 0.0f, 0.0f, 0.25f);
            glVertex2f(sx, sy);
        }
        else {
            glColor4f(1.0f, 0.5f, 0.1f, 0.25f);
            glVertex2f(sx, sy);
        }

        for (int i = 0; i <= NUM_RAYS; i++) {
            float t = (float)i / NUM_RAYS;
            float angle = (t - 0.5f) * 2.0f * coneWidth;
            if (!movingRight) angle = 3.14159f - angle;
            
            
            float edgeFade = 1.0f - fabsf((t - 0.5f) * 2.0f);
            float alpha = 0.09f * edgeFade;

            if (isColliding || m_npcAlerted) {
                glColor4f(1.0f, 0.0f, 0.0f, alpha);
            } else {
                glColor4f(1.0f, 0.5f, 0.1f, alpha);
            }
            
            float vx = (nx - camX) + cosf(angle) * VISION_RANGE;
            float vy = sy + sinf(angle) * VISION_RANGE - TILT_DOWN;
#if APP_USE_VIRTUAL_RES
            APP_VIRTUAL_TO_NATIVE_COORDS(vx, vy);
#endif
            glVertex2f(vx, vy);
        }
        glEnd();
    };

    if (m_npcActive) drawCone(m_roamingNPC, m_npcMoveRight);
    if (m_npcActive2) drawCone(m_roamingNPC2, m_npcMoveRight2);

    glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
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
            m_npcAlerted = false;
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

    if (m_heatLevel >= 1) {
        m_npcTimer2 += deltaTime / 1000.0f;
        if (!m_npcActive2) {
            if (m_npcTimer2 >= m_npcSpawnDelay * 1.5f) {
                m_npcTimer2 = 0.0f;
                m_npcActive2 = true;
                m_npcMoveRight2 = !m_npcMoveRight;
                const float startX = m_npcMoveRight2 ? -100.0f : 3000.0f;
                m_roamingNPC2->SetPosition(startX, 280.0f);
                m_roamingNPC2->SetFlipX(m_npcMoveRight2);
                m_roamingNPC2->SetAnimation(0);
            }
        }
        else {
            float x, y;
            m_roamingNPC2->GetPosition(x, y);
            x += m_npcMoveRight2 ? m_npcSpeed : -m_npcSpeed;
            m_roamingNPC2->SetPosition(x, y);
            m_roamingNPC2->SetFlipX(!m_npcMoveRight2);

            if (x > GUARD_WORLD_X + 50.0f && m_npcMoveRight2)  m_npcMoveRight2 = false;
            if (x < 50.0f && !m_npcMoveRight2) m_npcMoveRight2 = true;

            m_roamingNPC2->Update(deltaTime);
        }
    }
    else {
        m_npcActive2 = false;
        m_roamingNPC2->SetPosition(-200.0f, -200.0f);
    }
}

bool Level::IsPlayerNearGuard(float px) const {
    return fabsf(px - (GUARD_WORLD_X + 200.0f)) < 120.0f;
}

void Level::UpdateGuard(float playerX, int& playerGold, bool& outChangeCar, float deltaTime) {
    if (m_currentCar >= 3) return; 

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

    if (m_guardState == GuardState::NOT_READY) {
        m_msgTimer += dt;
        if (m_msgTimer >= 3.0f || escDown) {
            m_guardState = GuardState::NONE;
        }
        return;
    }

    if (m_guardState == GuardState::NONE) {
        if (IsPlayerNearGuard(playerX) && enterDown && !m_enterWasDown) {
            int currentGear = GearManager::GetInstance().GetGearCount();
            int requiredGear = 0; 
            if (currentGear < requiredGear) {
                m_guardState = GuardState::NOT_READY;
                m_msgTimer = 0.0f;
            }
            else {
                m_guardState = GuardState::PROMPT;
                m_guardChoice = 0;
                m_navDown = false;
            }
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
                    if (playerGold >= 30) {
                        playerGold -= 30;
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
        App::PrintTTF(169, 710, "You don't have enough gold!", 0.239f, 0.0f, 0.0f, 0);
        return;
    }

    if (m_guardState == GuardState::NOT_READY) {
        App::PrintTTF(169, 730, "Ticketman: Thou shall not pass.", 1.0f, 1.0f, 1.0f, 0);
        App::PrintTTF(169, 710, "The goblin is not satisfied...", 1.0f, 0.7f, 0.7f, 0);
        return;
    }

    
    if (m_guardState == GuardState::PROMPT) {
        App::PrintTTF(169, 730, "Ticketman: Ticket for the next car is 30 gold.", 1.0f, 1.0f, 1.0f, 0);
        App::PrintTTF(169, 705, "Pay 30 Gold?", 0.239f, 0.0f, 0.0f, 1);

        float yesR = (m_guardChoice == 0) ? 1.0f : 0.5f;
        float noR = (m_guardChoice == 1) ? 1.0f : 0.5f;
        App::PrintTTF(185, 680, "YES", yesR * 0.239f, 0.0f, 0.0f, 1);
        App::PrintTTF(245, 680, "NO", noR, noR, noR, 1);
        float cursorX = (m_guardChoice == 0) ? 170.0f : 230.0f;
        App::PrintTTF(cursorX, 680, ">", 0.239f, 0.0f, 0.0f, 0);
    }
}
void Level::RenderBackground(float camX, float brightness) {
    const float bgWidth = 1075.0f;
    const float windowWidth = 1024.0f;

    const float cityScrollX = m_cityScrollOffset;
    const float windowScrollX = fmodf(camX * 0.3f, windowWidth);
    const float bgScrollX = fmodf(camX, bgWidth);

    float b = brightness;

    m_city->SetColor(b, b, b);
    m_city->SetPosition(512.0f - cityScrollX, 450.0f);
    m_city->Draw();
    m_city->SetPosition(512.0f - cityScrollX + windowWidth, 450.0f);
    m_city->Draw();

    m_rain->SetColor(b, b, b);
    m_rain->SetPosition(475.0f - windowScrollX, 450.0f);
    m_rain->Draw();
    m_rain->SetPosition(475.0f - windowScrollX + windowWidth, 450.0f);
    m_rain->Draw();

    m_background->SetColor(1.0f, 1.0f, 1.0f);
    m_background->SetPosition(500.0f - bgScrollX, 400.0f);
    m_background->Draw();
    m_background->SetPosition(500.0f - bgScrollX + bgWidth, 400.0f);
    m_background->Draw();
}

void Level::RenderForeground(float camX, float camY, bool isColliding) {
    if (m_npcActive) {
        float actualX, actualY;
        m_roamingNPC->GetPosition(actualX, actualY);
        m_roamingNPC->SetPosition(actualX - camX, actualY - camY);
        m_roamingNPC->Draw();
        m_roamingNPC->SetPosition(actualX, actualY);

        static float t = 0.0f;
        t += 0.05f;
        float bob = sinf(t) * 4.0f;
        float worldHeight = m_roamingNPC->GetHeight() * m_roamingNPC->GetScale();

        float iconX = (actualX - camX) + 1.0f;
        float iconY = (actualY - camY) + (worldHeight * 0.5f) + 60.0f + bob;

        if (m_npcAlerted) {
            m_alertIcon->SetPosition(iconX, iconY);
            m_alertIcon->Draw();
        }
        else if (isColliding) {
            m_questionIcon->SetPosition(iconX, iconY);
            m_questionIcon->Draw();
        }
    }

    if (m_npcActive2) {
        float actualX, actualY;
        m_roamingNPC2->GetPosition(actualX, actualY);
        m_roamingNPC2->SetPosition(actualX - camX, actualY - camY);
        m_roamingNPC2->Draw();
        m_roamingNPC2->SetPosition(actualX, actualY);

        static float t2 = 0.0f;
        t2 += 0.05f;
        float bob = sinf(t2) * 4.0f;
        float worldHeight = m_roamingNPC2->GetHeight() * m_roamingNPC2->GetScale();

        float iconX = (actualX - camX) + 1.0f;
        float iconY = (actualY - camY) + (worldHeight * 0.5f) + 22.0f + bob;

        if (m_npcAlerted) {
            m_alertIcon->SetPosition(iconX, iconY);
            m_alertIcon->Draw();
        }
        else if (isColliding) {
            m_questionIcon->SetPosition(iconX, iconY);
            m_questionIcon->Draw();
        }
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