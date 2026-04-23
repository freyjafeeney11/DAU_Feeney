#pragma once
#include "app\app.h"

enum class GuardState {
    NONE,
    PROMPT,
    NO_FUNDS
};

class Level {
private:
    CSimpleSprite* m_city;
    CSimpleSprite* m_background;
    CSimpleSprite* m_window;
    CSimpleSprite* m_rain;
    CSimpleSprite* m_roamingNPC;
    CSimpleSprite* m_guardSprite;
    CSimpleSprite* m_ladderSprite;
    CSimpleSprite* m_dialogueBox;

    float m_cityScrollOffset;
    bool m_npcActive;
    float m_npcTimer;
    float m_npcSpawnDelay;
    float m_npcSpeed;
    bool m_npcMoveRight;

 
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

public:
    Level();
    ~Level();

    void Update(float deltaTime);
    void RenderBackground(float camX);
    void RenderForeground(float camX, float camY);

    void UpdateGuard(float playerX, int& playerGold, bool& outChangeCar, float deltaTime);
    void RenderGuardUI();
    bool IsGuardUIOpen() const { return m_guardState != GuardState::NONE; }
    bool IsPlayerNearGuard(float px) const;
    int GetCurrentCar() const { return m_currentCar; }

    float GetLadderX() const { return LADDER_WORLD_X; }
};