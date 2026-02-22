#pragma once
#include "app\app.h"
#include <string>

class NPC {
private:
    CSimpleSprite* sprite;
    std::string name;
    int lootTable[6];
    int difficulty;
    bool isAlerted;

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