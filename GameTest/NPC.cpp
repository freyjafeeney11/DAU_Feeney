#include "stdafx.h"
#include "NPC.h"

NPC::NPC(const char* spritePath, const char* npcName, int npcDifficulty, int* npcLoot,
    float startX, float startY, float scale,
    int sheetCols, int sheetRows, int animRow, int animFrameCount)
{
    m_name = npcName;
    m_difficulty = npcDifficulty;
    m_isAlerted = false;

    for (int i = 0; i < 6; i++) {
        m_lootTable[i] = GenerateItem(npcLoot[i]);
    }

    m_sprite = App::CreateSprite(spritePath, sheetCols, sheetRows);
    m_sprite->SetPosition(startX, startY);
    m_sprite->SetScale(scale);

    std::vector<int> frames;
    int startFrame = animRow * sheetCols;
    for (int i = 0; i < animFrameCount; i++) {
        frames.push_back(startFrame + i);
    }
    m_sprite->CreateAnimation(0, 0.4f, frames);
    m_sprite->SetAnimation(0);
    m_alertTimer = 0.0f;
}

NPC::~NPC() {
    delete m_sprite;
}

void NPC::Update(float deltaTime) {
    m_sprite->Update(deltaTime);
    if (m_isAlerted) {
        m_alertTimer += deltaTime / 1000.0f;
        if (m_alertTimer >= ALERT_DURATION) {
            m_isAlerted = false;
            m_alertTimer = 0.0f;
        }
    }
}

void NPC::Render(float camX, float camY, bool targeted=false) {
    if (targeted) {
        m_sprite->SetColor(1.0f, 0.45f, 0.45f);
    }
    else {
        m_sprite->SetColor(1.0f, 1.0f, 1.0f);
    }
    float actualX, actualY;
    m_sprite->GetPosition(actualX, actualY);
    m_sprite->SetPosition(actualX - camX, actualY - camY);
    m_sprite->Draw();
    m_sprite->SetPosition(actualX, actualY);
}

void NPC::GetPosition(float& x, float& y) {
    m_sprite->GetPosition(x, y);
}