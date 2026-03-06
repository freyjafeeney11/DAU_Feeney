// NPC.h
#pragma once
#include "app\app.h"
#include <string>

class NPC {
private:
    CSimpleSprite* m_sprite;
    std::string m_name;
    int m_lootTable[6];
    int m_difficulty;
    bool m_isAlerted;

public:
    NPC(const char* spritePath, const char* npcName, int npcDifficulty, int* npcLoot, float startX, float startY, float scale = 0.2f);
    ~NPC();

    void Update(float deltaTime);
    void Render(float camX, float camY);
    void GetPosition(float& x, float& y);
    std::string GetName();
    int GetDifficulty();
    int* GetLootTable();
    bool GetIsAlerted();

    void SetAlerted(bool alerted);
    float GetHeight();
    float GetScale();
};