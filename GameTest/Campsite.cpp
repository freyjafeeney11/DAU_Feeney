#include "stdafx.h"
#include "Campsite.h"
#include <math.h>

Rooftop::Rooftop() {
    m_nightsky = App::CreateSprite(".\\TestData\\night_sky.png", 1, 1);
    m_nightsky->SetPosition(512.0f, 400.0f);
    m_nightsky->SetScale(0.6f);

    m_daysky = App::CreateSprite(".\\TestData\\day_background.png", 1, 1);
    m_daysky->SetPosition(512.0f, 400.0f);
    m_daysky->SetScale(0.6f);

    m_dayclouds = App::CreateSprite(".\\TestData\\day_clouds.png", 1, 1);
    m_dayclouds->SetScale(0.6f);

    m_nightclouds = App::CreateSprite(".\\TestData\\clouds.png", 1, 1);
    m_nightclouds->SetScale(0.6f);

    m_background = App::CreateSprite(".\\TestData\\train_bg.png", 1, 1);
    m_background->SetPosition(512.0f, 400.0f);
    m_background->SetScale(0.6f);

    m_tentflaps = App::CreateSprite(".\\TestData\\tent_closed.png", 1, 1);
    m_tentflaps->SetPosition(512.0f, 400.0f);
    m_tentflaps->SetScale(0.6f);

    m_tentsleep = App::CreateSprite(".\\TestData\\tent_sleep.png", 1, 1);
    m_tentsleep->SetPosition(512.0f, 400.0f);
    m_tentsleep->SetScale(0.6f);

    m_plant = App::CreateSprite(".\\TestData\\plant.png", 1, 1);
    m_plant->SetPosition(512.0f, 400.0f);
    m_plant->SetScale(0.6f);

    m_campfire = App::CreateSprite(".\\TestData\\fire_anim.png", 5, 1);
    m_campfire->SetPosition(585.0f, 324.0f);
    m_campfire->SetScale(0.77f);
    m_campfire->CreateAnimation(0, 0.1f, { 0, 1, 2, 3, 4 });
    m_campfire->SetAnimation(0);

    m_cloudOffset = 0.0f;
    m_playerInTentZone = false;
    m_playerSleeping = false;
    m_enterWasDown = false;
}

Rooftop::~Rooftop() {
    delete m_background;
    delete m_nightsky;
    delete m_daysky;
    delete m_dayclouds;
    delete m_nightclouds;
    delete m_tentflaps;
    delete m_tentsleep;
    delete m_plant;
    delete m_campfire;
}

void Rooftop::Update(float deltaTime, float playerX) {
    float dt = deltaTime / 1000.0f;

    m_cloudOffset += CLOUD_SPEED * dt;
    if (m_cloudOffset > CLOUD_WIDTH) m_cloudOffset -= CLOUD_WIDTH;

    m_playerInTentZone = (playerX > TENT_TRIGGER_X);

    if (m_playerInTentZone) {
        bool enterDown = App::IsKeyPressed(VK_RETURN);
        if (enterDown && !m_enterWasDown) {
            m_playerSleeping = !m_playerSleeping;
        }
        m_enterWasDown = enterDown;
    }
    else {
        m_playerSleeping = false;
        m_enterWasDown = false;
    }
    m_campfire->Update(deltaTime);
}

void Rooftop::Render() {
    m_nightsky->SetPosition(512.0f, 400.0f);
    m_nightsky->Draw();

    m_nightclouds->SetPosition(512.0f - m_cloudOffset, 400.0f);
    m_nightclouds->Draw();
    m_nightclouds->SetPosition(512.0f - m_cloudOffset + CLOUD_WIDTH, 400.0f);
    m_nightclouds->Draw();

    m_background->SetPosition(512.0f, 400.0f);
    m_background->Draw();
    m_campfire->Draw();

    if (!m_playerInTentZone) {
        m_tentflaps->SetPosition(512.0f, 400.0f);
        m_tentflaps->Draw();
    }

    if (m_playerSleeping) {
        m_tentsleep->SetPosition(512.0f, 400.0f);
        m_tentsleep->Draw();
    }
}

void Rooftop::RenderPlant() {
    m_plant->SetPosition(512.0f, 400.0f);
    m_plant->Draw();
}

bool Rooftop::IsPlayerNearHatch(float px) const {
    return fabsf(px - HATCH_X) < HATCH_RADIUS;
}