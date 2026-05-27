#include "stdafx.h"
#include "NPCManager.h"
#include <math.h>

NPCManager::NPCManager() {
    m_activeNPC = nullptr;
    m_alertIcon = nullptr;

    int rosamund[6] = { ITEM_GOLD, ITEM_LETTER,    ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE };
    int granny[6]   = { ITEM_GOLD, ITEM_BOOK,      ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE };
    int randy[6]    = { ITEM_GOLD, ITEM_FLASHDRIVE,ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE };
    int helene[6]   = { ITEM_GOLD, ITEM_COLLAR,    ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE };
    int magda[6]    = { ITEM_GOLD, ITEM_RAT,       ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE };
    int lupine[6]   = { ITEM_GOLD, ITEM_PAINTING,  ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE };
    int charles[6]  = { ITEM_GOLD, ITEM_BOUQUET,   ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE };

    for (int i = 0; i < 6; i++) {
        m_rosamundLoot[i] = rosamund[i];
        m_grannyLoot[i]   = granny[i];
        m_randyLoot[i]    = randy[i];
        m_heleneLoot[i]   = helene[i];
        m_magdaLoot[i]    = magda[i];
        m_lupineLoot[i]   = lupine[i];
        m_charlesLoot[i]  = charles[i];
    }
}

NPCManager::~NPCManager() {
    for (NPC* npc : m_allNPCs) {
        delete npc;
    }
    m_allNPCs.clear();
}

void NPCManager::LoadCar(int carNumber, Player* player, float& cameraX) {
    for (NPC* npc : m_allNPCs) {
        delete npc;
    }
    m_allNPCs.clear();
    m_activeNPC = nullptr;

    if (carNumber == 1) {
        m_allNPCs.push_back(new NPC(".\\TestData\\rosamund_idle.png", "Rosamund", 11, m_rosamundLoot, 790.0f, 330.0f, 0.19f));
        m_allNPCs.push_back(new NPC(".\\TestData\\granny_idle.png", "Granny", 8, m_grannyLoot, 1750.0f, 310.0f, 0.16f));
        m_allNPCs.push_back(new NPC(".\\TestData\\randy_idle.png", "Randy", 14, m_randyLoot, 320.0f, 330.0f, 0.2f));
    }
    else if (carNumber == 2) {
        m_allNPCs.push_back(new NPC(".\\TestData\\npc_sprites.png", "Helene", 10, m_heleneLoot, 400.0f, 330.0f, 0.2f, 2, 4, 0, 2));
        m_allNPCs.push_back(new NPC(".\\TestData\\npc_sprites.png", "Magda", 12, m_magdaLoot, 900.0f, 330.0f, 0.2f, 2, 4, 1, 2));
        m_allNPCs.push_back(new NPC(".\\TestData\\npc_sprites.png", "Charles", 13, m_charlesLoot, 1900.0f, 330.0f, 0.2f, 2, 4, 3, 2));
    }
    else if (carNumber == 3) {
        m_allNPCs.push_back(new NPC(".\\TestData\\npc_sprites.png", "Lupine", 17, m_lupineLoot, 1400.0f, 330.0f, 0.2f, 2, 4, 2, 2));
    }

    if (player) {
        player->SetPosition(200.0f, 250.0f);
        cameraX = 0.0f;
    }
}

void NPCManager::Update(float deltaTime) {
    for (NPC* npc : m_allNPCs) {
        npc->Update(deltaTime);
    }
}

void NPCManager::Render(float camX, float camY, bool isPickpocketUIOpen) {
    for (NPC* npc : m_allNPCs) {
        bool targeted = (npc == m_activeNPC && !isPickpocketUIOpen);
        npc->Render(camX, camY, targeted);
    }
}

bool NPCManager::IsPlayerNearNPC(Player* player) {
    if (!player) return false;
    float px, py, nx, ny;
    player->GetPosition(px, py);
    m_activeNPC = nullptr;
    const float playerRadius = 20.0f;
    const float npcRadius = 100.0f;
    for (NPC* npc : m_allNPCs) {
        npc->GetPosition(nx, ny);
        float dx = px - nx;
        float dy = py - ny;
        if (sqrtf(dx * dx + dy * dy) < (playerRadius + npcRadius)) {
            m_activeNPC = npc;
            return true;
        }
    }
    return false;
}

void NPCManager::DrawAlertIconAboveNPC(NPC* npc, float camX) {
    if (!npc || !m_alertIcon) return;
    float x, y;
    npc->GetPosition(x, y);
    float worldHeight = npc->GetHeight() * npc->GetScale();

    static float t = 0.0f;
    t += 0.05f;
    float bob = sinf(t) * 4.0f;

    m_alertIcon->SetPosition(x - camX, y + (worldHeight * 0.5f) + 22.0f + bob);
    m_alertIcon->Draw();
}

void NPCManager::DrawAlertIcons(float camX) {
    for (NPC* npc : m_allNPCs) {
        if (npc->GetIsAlerted()) {
            DrawAlertIconAboveNPC(npc, camX);
        }
    }
}

void NPCManager::ResetAlerts() {
    for (NPC* npc : m_allNPCs) {
        npc->SetAlerted(false);
    }
}
