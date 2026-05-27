#pragma once
#include "NPC.h"
#include "Player.h"
#include "app\app.h"
#include <vector>

class NPCManager {
public:
    NPCManager();
    ~NPCManager();

    void LoadCar(int carNumber, Player* player, float& cameraX);
    void Update(float deltaTime);
    void Render(float camX, float camY, bool isPickpocketUIOpen);

    bool IsPlayerNearNPC(Player* player);
    NPC* GetActiveNPC() const { return m_activeNPC; }
    const std::vector<NPC*>& GetAllNPCs() const { return m_allNPCs; }

    void SetAlertIcon(CSimpleSprite* icon) { m_alertIcon = icon; }
    void DrawAlertIcons(float camX);
    void ResetAlerts();

private:
    void DrawAlertIconAboveNPC(NPC* npc, float camX);

    std::vector<NPC*> m_allNPCs;
    NPC* m_activeNPC;
    CSimpleSprite* m_alertIcon;

    
    int m_rosamundLoot[6];
    int m_grannyLoot[6];
    int m_randyLoot[6];
    int m_heleneLoot[6];
    int m_magdaLoot[6];
    int m_lupineLoot[6];
    int m_charlesLoot[6];
};
