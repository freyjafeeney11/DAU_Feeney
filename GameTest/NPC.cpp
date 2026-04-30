#include "stdafx.h"
#include "NPC.h"
#include <math.h>

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
    AssignThought();

    m_bubbleSprite = App::CreateSprite(".\\TestData\\speech_bubble.png", 1, 1);
    m_bubbleSprite->SetScale(0.18f);
    m_bubbleSprite->SetFlipX(true);
    m_thoughtTimer = 0.0f;
    m_thoughtDuration = 4.0f;
    m_thoughtCooldown = 3.0f + (rand() % 5);
    m_showingThought = false;
}

NPC::~NPC() {
    delete m_sprite;
    delete m_bubbleSprite;
}

void NPC::AssignThought() {
    if (m_name == "Rosamund") m_thoughtText = "Such a beautiful\nrainy night...";
    else if (m_name == "Randy") m_thoughtText = "Did I leave\nthe stove on?";
    else if (m_name == "Granny") m_thoughtText = "I hope M. Miaou\nis doing okay...";
    else if (m_name == "Helene") m_thoughtText = "I can't wait to go back\nto the thrift store.";
    else if (m_name == "Charles") m_thoughtText = "How can I diversify\nmy portfolio...";
    else if (m_name == "Magda") m_thoughtText = "I like reading\nthe newspaper?";
    else if (m_name == "Lupine") m_thoughtText = "I hope nobody\ncan hear my thoughts...";
}

void NPC::Update(float deltaTime) {
    m_sprite->Update(deltaTime);
    float dt = deltaTime / 1000.0f;

    if (m_isAlerted) {
        m_alertTimer += dt;
        if (m_alertTimer >= ALERT_DURATION) {
            m_isAlerted = false;
            m_alertTimer = 0.0f;
        }
    }
    else {
        m_thoughtTimer += dt;
        if (m_showingThought) {
            if (m_thoughtTimer >= m_thoughtDuration) {
                m_showingThought = false;
                m_thoughtTimer = 0.0f;
                m_thoughtCooldown = 3.0f + (rand() % 5);
            }
        }
        else {
            if (m_thoughtTimer >= m_thoughtCooldown) {
                m_showingThought = true;
                m_thoughtTimer = 0.0f;
            }
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

    float bubbleX = actualX - camX + 120.0f;
    float bubbleY = actualY - camY + GetHeight() * GetScale() * 0.5f + 80.0f;
    static float bt = 0.0f;
    bt += 0.05f;
    bubbleY += sinf(bt) * 3.0f;

    if (m_isAlerted) {
        m_bubbleSprite->SetAlpha(1.0f);
        m_bubbleSprite->SetPosition(bubbleX, bubbleY);
        m_bubbleSprite->Draw();
        App::PrintTTF(bubbleX - 45.0f, bubbleY + 10.0f, "Hey!", 0.6f, 0.0f, 0.0f, 0, 1.0f);
        App::PrintTTF(bubbleX - 45.0f, bubbleY - 10.0f, "My pockets!", 0.6f, 0.0f, 0.0f, 0, 1.0f);
    } else if (m_showingThought && !m_thoughtText.empty()) {
        float alpha = 1.0f;
        if (m_thoughtTimer < 0.5f) {
            alpha = m_thoughtTimer / 0.5f;
        } else if (m_thoughtTimer > m_thoughtDuration - 0.5f) {
            alpha = (m_thoughtDuration - m_thoughtTimer) / 0.5f;
            if (alpha < 0.0f) alpha = 0.0f;
        }

        m_bubbleSprite->SetAlpha(alpha);
        m_bubbleSprite->SetPosition(bubbleX, bubbleY);
        m_bubbleSprite->Draw();
        
        std::string t = m_thoughtText;
        size_t pos = t.find('\n');
        if (pos != std::string::npos) {
            std::string line1 = t.substr(0, pos);
            std::string line2 = t.substr(pos + 1);
            App::PrintTTF(bubbleX - 60.0f, bubbleY + 10.0f, line1.c_str(), 0.5f, 0.0f, 0.0f, 0, alpha);
            App::PrintTTF(bubbleX - 60.0f, bubbleY - 10.0f, line2.c_str(), 0.5f, 0.0f, 0.0f, 0, alpha);
        } else {
            App::PrintTTF(bubbleX - 60.0f, bubbleY, t.c_str(), 0.5f, 0.0f, 0.0f, 0, alpha);
        }
    }
}

void NPC::GetPosition(float& x, float& y) {
    m_sprite->GetPosition(x, y);
}