#pragma once
#include "app\app.h"

enum class GuardState {
    NONE,
    PROMPT,
    NO_FUNDS,
    NOT_READY,
    OUTRO
};

class Level {
private:

    CSimpleSprite* m_city;
    CSimpleSprite* m_background;
    CSimpleSprite* m_window;
    CSimpleSprite* m_rain;
    CSimpleSprite* m_roamingNPC;
    CSimpleSprite* m_roamingNPC2;
    CSimpleSprite* m_guardSprite;
    CSimpleSprite* m_ladderSprite;
    CSimpleSprite* m_dialogueBox;

    float m_cityScrollOffset;
    bool m_npcActive;
    float m_npcTimer;
    float m_npcSpawnDelay;
    float m_npcSpeed;
    bool m_npcMoveRight;

    bool m_npcActive2;
    float m_npcTimer2;
    bool m_npcMoveRight2;

    int m_heatLevel;

 
    int m_currentCar;
    GuardState m_guardState;
    int m_guardChoice;
    bool m_enterWasDown;
    bool m_navDown;
    float m_msgTimer;

    static constexpr float BG_WIDTH = 1075.0f;
    static constexpr float GUARD_WORLD_X = 2.0f * BG_WIDTH + 716.0f;
    static constexpr float GUARD_WORLD_Y = 370.0f;
    static constexpr float LADDER_WORLD_X = 2750.0f;
    static constexpr float LADDER_WORLD_Y = 480.0f;
    static constexpr float VISION_RANGE = 320.0f;
    static constexpr float VISION_CONE_HALF = 0.7f;

    // alert cone
    CSimpleSprite* m_questionIcon;
    CSimpleSprite* m_alertIcon;
    bool m_npcAlerted;

public:
    Level();
    ~Level();

    void Update(float deltaTime);
    void RenderBackground(float camX, float brightness = 1.0f);
    void RenderForeground(float camX, float camY, bool isColliding);

    //walking npc scope
    bool IsPlayerInWalkingNPCVision(float playerX, float playerY) const;

    void UpdateGuard(float playerX, int& playerGold, bool& outChangeCar, float deltaTime);
    void RenderGuardUI();
    bool IsGuardUIOpen() const { return m_guardState != GuardState::NONE; }
    bool IsPlayerNearGuard(float px) const;
    int GetCurrentCar() const { return m_currentCar; }

    float GetLadderX() const { return LADDER_WORLD_X; }

    void SetNPCAlerted(bool alerted) { m_npcAlerted = alerted; }
    void RenderWalkingNPCVision(float camX, float camY, bool isColliding) const;

    void IncreaseHeat() { m_heatLevel++; }
    void ResetHeat() { m_heatLevel = 0; }
    int GetHeatLevel() const { return m_heatLevel; }
    void Reset() { 
        m_heatLevel = 0; 
        m_currentCar = 1; 
        m_guardState = GuardState::NONE; 
        m_npcAlerted = false; 
    }
};