#pragma once
#include "app\app.h"
#include <string>
#include "Item.h"

class NPC {
private:
    CSimpleSprite* m_sprite;
    std::string m_name;
    Item m_lootTable[6];
    int m_difficulty;
    bool m_isAlerted;
    float m_alertTimer;
    static constexpr float ALERT_DURATION = 30.0f;

public:
    NPC(const char* spritePath, const char* npcName, int npcDifficulty, int* npcLoot,
        float startX, float startY, float scale = 0.2f,
        int sheetCols = 4, int sheetRows = 1, int animRow = 0, int animFrameCount = 5);    
    ~NPC();

    void Update(float deltaTime);
    void Render(float camX, float camY, bool targeted);
    void GetPosition(float& x, float& y);

    std::string GetName()      const { return m_name; }
    int GetDifficulty()        const { return m_difficulty; }
    Item* GetLootTable() { return m_lootTable; }
    bool GetIsAlerted()        const { return m_isAlerted; }
    void SetAlerted(bool alerted) {
        m_isAlerted = alerted;
        if (alerted) m_alertTimer = 0.0f;
    }
    float GetHeight()          const { return m_sprite->GetHeight(); }
    float GetScale()           const { return m_sprite->GetScale(); }
};