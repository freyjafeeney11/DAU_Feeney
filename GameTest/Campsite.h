#pragma once
#include "app\app.h"
#include "UIManager.h"
#include <vector>

enum class TradeState {
    NONE,
    PROMPT,
    SELECTING,
    CONFIRMING,
    COMPLETE,
    SLEEP_TRANSITION
};

class Rooftop {
private:
    CSimpleSprite* m_background;
    CSimpleSprite* m_nightsky;
    CSimpleSprite* m_daysky;
    CSimpleSprite* m_dayclouds;
    CSimpleSprite* m_nightclouds;
    CSimpleSprite* m_tentflaps;
    CSimpleSprite* m_tentsleep;
    CSimpleSprite* m_plant;
    CSimpleSprite* m_campfire;
    CSimpleSprite* m_goblin;
    CSimpleSprite* m_trade_inventory_screen;
    CSimpleSprite* m_inventory_title;
    CSimpleSprite* m_watch_base;
    CSimpleSprite* m_ui_cursor;
    CSimpleSprite* m_watch_sprite;
    CSimpleSprite* m_icon_letter;
    CSimpleSprite* m_icon_flashdrive;
    CSimpleSprite* m_icon_picture;

    float m_cloudOffset;
    bool  m_playerInTentZone;
    bool  m_playerSleeping;
    bool  m_enterWasDown;
    bool  m_justSlept;

    TradeState m_tradeState;
    int   m_promptChoice;
    int   m_confirmChoice;
    int   m_selectedItemIndex;
    int   m_gearCount;
    bool  m_justTraded;
    bool  m_navKeyDown;
    bool  m_tradeEnterDown;
    float m_completeTimer;

    static constexpr float COMPLETE_DISPLAY_TIME = 1.5f;
    static constexpr float HATCH_X = 200.0f;
    static constexpr float HATCH_Y = 250.0f;
    static constexpr float HATCH_RADIUS = 120.0f;
    static constexpr float PLAYER_SPAWN_X = 200.0f;
    static constexpr float PLAYER_SPAWN_Y = 250.0f;
    static constexpr float TENT_TRIGGER_X = 700.0f;
    static constexpr float CLOUD_WIDTH = 1024.0f;
    static constexpr float CLOUD_SPEED = 80.0f;

    std::vector<int> GetTradeableIndices(const std::vector<Item>& playerInventory) const;
    void DrawItemIcon(int itemId, float x, float y);

public:
    Rooftop();
    ~Rooftop();

    void Update(float deltaTime, float playerX, std::vector<Item>& playerInventory);
    void Render(bool isDay);
    void RenderTradeUI(const std::vector<Item>& playerInventory);
    void RenderPlant();

    bool IsPlayerNearHatch(float px) const;
    bool JustSlept();

    bool JustTraded();
    bool IsSleeping()       const { return m_playerSleeping; }
    bool IsTrading()        const { return (m_tradeState != TradeState::NONE && m_tradeState != TradeState::SLEEP_TRANSITION); }
    int  GetGearCount()     const { return m_gearCount; }
    float GetSpawnX()       const { return PLAYER_SPAWN_X; }
    float GetSpawnY()       const { return PLAYER_SPAWN_Y; }
    float GetFadeBrightness() const;
};