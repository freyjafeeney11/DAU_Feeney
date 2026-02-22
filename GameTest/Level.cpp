// Level.cpp
#include "stdafx.h"
#include "Level.h"
#include <math.h>

Level::Level() {
    // Backgrounds
    background = App::CreateSprite(".\\TestData\\Train_bg.png", 1, 1);
    background->SetPosition(500.0f, 400.0f);
    background->SetScale(0.6f);

    window = App::CreateSprite(".\\TestData\\cityscape.png", 7, 1);
    window->SetPosition(470.0f, 450.0f);
    window->SetScale(0.6f);
    window->CreateAnimation(0, 0.5f, { 0, 1, 2, 3, 4, 5, 6 });
    window->SetAnimation(0);

    rain = App::CreateSprite(".\\TestData\\rain.png", 3, 1);
    rain->SetPosition(475.0f, 450.0f);
    rain->SetScale(0.5f);
    rain->CreateAnimation(0, 0.2f, { 0, 1, 2 });
    rain->SetAnimation(0);

    // Roaming NPC
    roamingNPC = App::CreateSprite(".\\TestData\\npc_walk.png", 4, 1);
    roamingNPC->SetScale(0.2f);
    roamingNPC->CreateAnimation(0, 0.2f, { 0,1,2,3 });
    roamingNPC->SetAnimation(0);
    roamingNPC->SetPosition(-200.0f, -200.0f);

    npcActive = false;
    npcTimer = 0.0f;
    npcSpawnDelay = 3.0f;
    npcSpeed = 2.5f;
    npcMoveRight = true;
}

Level::~Level() {
    delete background;
    delete window;
    delete rain;
    delete roamingNPC;
}

void Level::Update(float deltaTime) {
    window->Update(deltaTime);
    rain->Update(deltaTime);

    // Roaming NPC Logic
    npcTimer += deltaTime / 1000.0f;

    if (!npcActive) {
        if (npcTimer >= npcSpawnDelay) {
            npcTimer = 0.0f;
            npcActive = true;
            npcMoveRight = (rand() % 2) == 0;
            float startX = npcMoveRight ? -100.0f : 1100.0f;
            float startY = 280.0f;
            roamingNPC->SetPosition(startX, startY);
            roamingNPC->SetFlipX(npcMoveRight);
            roamingNPC->SetAnimation(0);
        }
    }
    else {
        float x, y;
        roamingNPC->GetPosition(x, y);
        x += (npcMoveRight ? npcSpeed : -npcSpeed);
        roamingNPC->SetPosition(x, y);
        roamingNPC->SetFlipX(!npcMoveRight);

        if (x > 1200.0f || x < -200.0f) {
            npcActive = false;
            npcTimer = 0.0f;
            roamingNPC->SetPosition(-200.0f, -200.0f);
        }
        roamingNPC->Update(deltaTime);
    }
}

void Level::RenderBackground(float camX) {
    float bgWidth = 1075.0f;
    float windowWidth = 1024.0f;

    float windowScrollX = fmodf(camX * 0.3f, windowWidth);
    float bgScrollX = fmodf(camX, bgWidth);

    window->SetPosition(512.0f - windowScrollX, 450.0f);
    window->Draw();
    window->SetPosition(512.0f - windowScrollX + windowWidth, 450.0f);
    window->Draw();

    rain->SetPosition(475.0f - windowScrollX, 450.0f);
    rain->Draw();
    rain->SetPosition(475.0f - windowScrollX + windowWidth, 450.0f);
    rain->Draw();

    background->SetPosition(500.0f - bgScrollX, 400.0f);
    background->Draw();
    background->SetPosition(500.0f - bgScrollX + bgWidth, 400.0f);
    background->Draw();
}

void Level::RenderForeground(float camX, float camY) {
    if (npcActive) {
        float actualX, actualY;
        roamingNPC->GetPosition(actualX, actualY);
        roamingNPC->SetPosition(actualX - camX, actualY - camY);
        roamingNPC->Draw();
        roamingNPC->SetPosition(actualX, actualY);
    }
}