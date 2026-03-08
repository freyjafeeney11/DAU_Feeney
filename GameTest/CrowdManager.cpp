// CrowdManager.cpp
#include "stdafx.h"
#include "CrowdManager.h"
#include <stdlib.h>

float CrowdManager::GetRandomOffset(int amplitude) {
    return (float)((rand() % (amplitude * 2 + 1)) - amplitude);
}

bool CrowdManager::IsPlayerInClump(float playerX, float playerY) {
    const float hideRadius = 150.0f;
    for (int i = 0; i < ms_NUM_CLUMPS; i++) {
        float dx = playerX - m_crowdClumps[i].m_baseX;
        float dy = playerY - m_crowdClumps[i].m_baseY;
        if (sqrtf(dx * dx + dy * dy) < hideRadius) return true;
    }
    return false;
}

CrowdManager::CrowdManager() : m_swayTime(0.0f) {
    const float baseX = 320.0f;
    const float spacing = 350.0f;

    for (int i = 0; i < ms_NUM_CLUMPS; i++) {
        m_crowdClumps[i].m_baseX = baseX + i * spacing;
        m_crowdClumps[i].m_baseY = 320.0f;
        m_crowdClumps[i].m_swayOffset = GetRandomOffset(100) / 100.0f * 6.28f;

        for (int j = 0; j < ms_MEMBERS_PER_CLUMP; j++) {
            CrowdMember& m = m_crowdClumps[i].m_members[j];

            m.m_sprite = std::unique_ptr<CSimpleSprite>(App::CreateSprite(".\\TestData\\IMG_1297.png", 1, 1));
            m.m_sprite->SetScale(0.5f);

            m.m_offsetX = GetRandomOffset(50);
            m.m_offsetY = GetRandomOffset(15);

            m.m_personalSwayOffset = GetRandomOffset(100) / 100.0f * 6.28f;
        }
    }
}

void CrowdManager::Update(float deltaTime) {

    m_swayTime += deltaTime / 1000.0f; // deltaTime is in milliseconds, convert to seconds

    for (int i = 0; i < ms_NUM_CLUMPS; i++) {
        CrowdClump& clump = m_crowdClumps[i];

        float clumpSwayX = sinf(m_swayTime + clump.m_swayOffset) * 2.0f;

        for (int j = 0; j < ms_MEMBERS_PER_CLUMP; j++) {
            CrowdMember& m = clump.m_members[j];

            float personalSway = sinf(m_swayTime * 1.5f + m.m_personalSwayOffset) * 1.5f;

            m.m_sprite->SetPosition(
                clump.m_baseX + clumpSwayX + m.m_offsetX,
                clump.m_baseY + personalSway + m.m_offsetY
            );
        }
    }
}

void CrowdManager::Render(float camX, float camY) {
    for (int i = 0; i < ms_NUM_CLUMPS; i++) {
        for (int j = 0; j < ms_MEMBERS_PER_CLUMP; j++) {
            CSimpleSprite* sprite = m_crowdClumps[i].m_members[j].m_sprite.get();
            float actualX, actualY;
            sprite->GetPosition(actualX, actualY);
            sprite->SetPosition(actualX - camX, actualY - camY);
            sprite->Draw();
            sprite->SetPosition(actualX, actualY);
        }
    }
}