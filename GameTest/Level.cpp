// Level.cpp
#include "stdafx.h"
#include "Level.h"
#include <math.h>

Level::Level() {
    // Backgrounds
    m_city = App::CreateSprite(".\\TestData\\city.png", 1, 1);
    m_city->SetPosition(512.0f, 450.0f);
    m_city->SetScale(0.6f);

    m_background = App::CreateSprite(".\\TestData\\train_bg.png", 1, 1);
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

    // Roaming NPC
    m_roamingNPC = App::CreateSprite(".\\TestData\\npc_walk.png", 5, 1);
    m_roamingNPC->SetScale(0.2f);
    m_roamingNPC->CreateAnimation(0, 0.3f, { 0,1,2,3, 4 });
    m_roamingNPC->SetAnimation(0);
    m_roamingNPC->SetPosition(-200.0f, -200.0f);

    m_npcActive = false;
    m_npcTimer = 0.0f;
    m_npcSpawnDelay = 3.0f;
    m_npcSpeed = 2.5f;
    m_npcMoveRight = true;
}

Level::~Level() {
    delete m_background;
    delete m_window;
    delete m_rain;
    delete m_roamingNPC;
    delete m_city;
}

void Level::Update(float deltaTime) {
    m_window->Update(deltaTime);
    m_rain->Update(deltaTime);

    // Roaming NPC Logic
    m_npcTimer += deltaTime / 1000.0f;

    if (!m_npcActive) {
        if (m_npcTimer >= m_npcSpawnDelay) {
            m_npcTimer = 0.0f;
            m_npcActive = true;
            m_npcMoveRight = (rand() % 2) == 0;
            float startX = m_npcMoveRight ? -100.0f : 1100.0f;
            float startY = 280.0f;
            m_roamingNPC->SetPosition(startX, startY);
            m_roamingNPC->SetFlipX(m_npcMoveRight);
            m_roamingNPC->SetAnimation(0);
        }
    }
    else {
        float x, y;
        m_roamingNPC->GetPosition(x, y);
        x += (m_npcMoveRight ? m_npcSpeed : -m_npcSpeed);
        m_roamingNPC->SetPosition(x, y);
        m_roamingNPC->SetFlipX(!m_npcMoveRight);

        if (x > 1200.0f || x < -200.0f) {
            m_npcActive = false;
            m_npcTimer = 0.0f;
            m_roamingNPC->SetPosition(-200.0f, -200.0f);
        }
        m_roamingNPC->Update(deltaTime);
    }
}

void Level::RenderBackground(float camX) {
    float bgWidth = 1075.0f;
    float windowWidth = 1024.0f;

    float windowScrollX = fmodf(camX * 0.3f, windowWidth);
    float bgScrollX = fmodf(camX, bgWidth);

   // m_window->SetPosition(512.0f - windowScrollX, 450.0f);
   // m_window->Draw();
   // m_window->SetPosition(512.0f - windowScrollX + windowWidth, 450.0f);
   // m_window->Draw();

    float cityScrollX = fmodf(camX * 0.5f, 1024.0f); // 0.5f for parallax speed
    m_city->SetPosition(512.0f - cityScrollX, 450.0f);
    m_city->Draw();
    m_city->SetPosition(512.0f - cityScrollX + 1024.0f, 450.0f);
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
}