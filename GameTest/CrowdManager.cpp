// CrowdManager.cpp
#include "stdafx.h"
#include "CrowdManager.h"
#include <stdlib.h>

CrowdManager::CrowdManager() {
    swayTime = 0.0f;

    float baseX = 300.0f;
    float spacing = 120.0f;

    for (int i = 0; i < NUM_CLUMPS; i++) {
        crowdClumps[i].baseX = baseX + i * spacing;
        crowdClumps[i].baseY = 350.0f;
        crowdClumps[i].swayOffset = (rand() % 100) / 100.0f * 6.28f;

        for (int j = 0; j < MEMBERS_PER_CLUMP; j++) {
            CrowdMember& m = crowdClumps[i].members[j];

            m.sprite = App::CreateSprite(".\\TestData\\IMG_1297.png", 1, 1);
            m.sprite->SetScale(0.35f);

            // spread people inside the clump
            m.offsetX = (float)((rand() % 41) - 20);   // -20 to +20
            m.offsetY = (float)((rand() % 31) - 15);

            m.personalSwayOffset = (rand() % 100) / 100.0f * 6.28f;
        }
    }
}

CrowdManager::~CrowdManager() {
    for (int i = 0; i < NUM_CLUMPS; i++) {
        for (int j = 0; j < MEMBERS_PER_CLUMP; j++) {
            delete crowdClumps[i].members[j].sprite;
        }
    }
}

void CrowdManager::Update(float deltaTime) {
    swayTime += deltaTime / 1000.0f;

    for (int i = 0; i < NUM_CLUMPS; i++) {
        CrowdClump& clump = crowdClumps[i];

        float clumpSwayX = sinf(swayTime + clump.swayOffset) * 2.0f;

        for (int j = 0; j < MEMBERS_PER_CLUMP; j++) {
            CrowdMember& m = clump.members[j];

            float personalSway = sinf(swayTime * 1.5f + m.personalSwayOffset) * 1.5f;

            m.sprite->SetPosition(
                clump.baseX + clumpSwayX + m.offsetX,
                clump.baseY + personalSway + m.offsetY
            );
        }
    }
}

void CrowdManager::Render(float camX, float camY) {
    for (int i = 0; i < NUM_CLUMPS; i++) {
        for (int j = 0; j < MEMBERS_PER_CLUMP; j++) {
            CSimpleSprite* sprite = crowdClumps[i].members[j].sprite;
            float actualX, actualY;
            sprite->GetPosition(actualX, actualY);
            sprite->SetPosition(actualX - camX, actualY - camY);
            sprite->Draw();
            sprite->SetPosition(actualX, actualY);
        }
    }
}