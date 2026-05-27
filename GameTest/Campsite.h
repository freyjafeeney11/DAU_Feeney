#pragma once
#include "app\app.h"
#include "UIManager.h"
#include <vector>
#include <string>

enum class TradeState {
    NONE,
    PROMPT,
    SELECTING,
    CONFIRMING,
    COMPLETE,
    FAILED,
    SLEEP_TRANSITION,
    FIRE_SIT
};

struct DailyRequest {
    std::string goblinLine;
    std::string riddle;
    std::vector<int> requiredItems;
};

class Rooftop {
private:
    CSimpleSprite* m_background;
    CSimpleSprite* m_request_board;
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
    CSimpleSprite* m_icon_rat;
    CSimpleSprite* m_icon_collar;
    CSimpleSprite* m_icon_bouquet;
    CSimpleSprite* m_icon_painting;
    CSimpleSprite* m_dialogue_bg;

    CSimpleSprite* m_campLetter;
    CSimpleSprite* m_campBook;
    CSimpleSprite* m_campDrive;
    CSimpleSprite* m_campCollar;
    CSimpleSprite* m_campRat;
    CSimpleSprite* m_campBouquet;
    CSimpleSprite* m_campPainting;

    float m_cloudOffset;
    bool  m_playerInTentZone;
    bool  m_playerSleeping;
    bool  m_enterWasDown;
    bool  m_justSlept;
    bool  m_justTraded;
    bool  m_justSatByFire;
    bool  m_playerNearFire;
    bool  m_navKeyDown;
    bool  m_tradeEnterDown;
    float m_completeTimer;
    bool  m_showDayMessage;

    TradeState m_tradeState;
    int   m_promptChoice;
    int   m_confirmChoice;
    int   m_selectedItemIndex;

    
    bool m_requestBoardOpen;
    bool m_iBoardKeyDown;

public:
    static const DailyRequest& GetRequest(int index);
private:
    static const DailyRequest ms_requests[7];

    static constexpr float COMPLETE_DISPLAY_TIME = 3.5f;
    static constexpr float FIRE_SIT_DURATION     = 2.0f;
    static constexpr float FIRE_X                = 585.0f;
    static constexpr float FIRE_TRIGGER_RADIUS   = 250.0f;
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
    void RenderRequestBoard() const;

public:
    Rooftop();
    ~Rooftop();

    void Update(float deltaTime, float playerX, std::vector<Item>& playerInventory, bool isDay);
    void Render(bool isDay);
    void RenderTradeUI(const std::vector<Item>& playerInventory);
    void RenderOverlayText(bool nearHatch, bool isDay);
    void RenderPlant();

    void NotifyNewDay();
    void Reset();
    void CloseUI();

    bool IsPlayerNearHatch(float px) const;
    bool JustSlept();
    bool JustTraded();
    bool JustSatByFire();

    bool IsSleeping()         const { return m_playerSleeping; }
    bool IsTrading()          const { return (m_tradeState != TradeState::NONE && m_tradeState != TradeState::SLEEP_TRANSITION); }
    bool IsSittingByFire()    const { return m_tradeState == TradeState::FIRE_SIT; }
    bool IsSleepTransition()  const { return m_tradeState == TradeState::SLEEP_TRANSITION; }
    bool IsRequestBoardOpen() const { return m_requestBoardOpen; }
    float GetSpawnX()         const { return PLAYER_SPAWN_X; }
    float GetSpawnY()         const { return PLAYER_SPAWN_Y; }
    float GetFadeBrightness() const;
};