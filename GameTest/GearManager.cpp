#include "stdafx.h"
#include "GearManager.h"
#include <cstdio>
#include <string>

GearManager::GearManager() {
    m_gearCount = 0;
    m_watchSprite = nullptr;
}

GearManager::~GearManager() {
    if (m_watchSprite) {
        delete m_watchSprite;
    }
}

void GearManager::AddGear() {
    m_gearCount++;

    if (m_watchSprite) {
        delete m_watchSprite;
    }
       // add conditions for finished clock
    std::string spritePath = ".\\TestData\\watch_phase_" + std::to_string(m_gearCount) + ".png";
    m_watchSprite = App::CreateSprite(spritePath.c_str(), 1, 1);
}

void GearManager::RenderUI(float x, float y) {
    if (m_watchSprite) {
        m_watchSprite->SetPosition(x, y);
        m_watchSprite->SetScale(0.12f);
        m_watchSprite->Draw();
    }
}