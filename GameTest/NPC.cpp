// NPC.cpp
#include "stdafx.h"
#include "NPC.h"

NPC::NPC(const char* spritePath, const char* npcName, int npcDifficulty, int* npcLoot, float startX, float startY, float scale) {
    m_name = npcName;
    m_difficulty = npcDifficulty;
    m_isAlerted = false;

    for (int i = 0; i < 6; i++) {
        m_lootTable[i] = npcLoot[i];
    }

    m_sprite = App::CreateSprite(spritePath, 4, 1);
    m_sprite->SetPosition(startX, startY);
    m_sprite->SetScale(scale);
    m_sprite->CreateAnimation(0, 0.4f, { 0,1,2,3,4 });
    m_sprite->SetAnimation(0);
}

NPC::~NPC() {
    delete m_sprite;
}

void NPC::Update(float deltaTime) {
    m_sprite->Update(deltaTime);
}

void NPC::Render(float camX, float camY) {
    float actualX, actualY;
    m_sprite->GetPosition(actualX, actualY);

    m_sprite->SetPosition(actualX - camX, actualY - camY);
    m_sprite->Draw();

    m_sprite->SetPosition(actualX, actualY);
}

void NPC::GetPosition(float& x, float& y) {
    m_sprite->GetPosition(x, y);
}