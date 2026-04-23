#pragma once
#include "app\app.h"
#include "NPC.h"
#include <vector>
#include <string>

class UIManager {
private:
    CSimpleSprite* m_inventory_screen;
    CSimpleSprite* m_player_inventory_screen;
    CSimpleSprite* m_player_inventory_screen_title;
    CSimpleSprite* m_player_watch_sprite;
    CSimpleSprite* m_icon_gold_small_hud;

    CSimpleSprite* m_rosamund_inv_sprite;
    CSimpleSprite* m_randy_inv_sprite;
    CSimpleSprite* m_granny_inv_sprite;

    CSimpleSprite* m_dice_roll;
    CSimpleSprite* m_ui_cursor;

    // one set of icons used everywhere
    CSimpleSprite* m_icon_gold_small;
    CSimpleSprite* m_icon_flashdrive_small;
    CSimpleSprite* m_icon_letter_small;
    CSimpleSprite* m_icon_picture_small;
    CSimpleSprite* m_icon_bouquet;
    CSimpleSprite* m_icon_collar;
    CSimpleSprite* m_icon_rat;
    CSimpleSprite* m_icon_painting;
    CSimpleSprite* m_charles_inv_sprite;
    CSimpleSprite* m_helene_inv_sprite;
    CSimpleSprite* m_magda_inv_sprite;
    CSimpleSprite* m_lupine_inv_sprite;
    CSimpleSprite* m_overlay;

    float m_diceTimer;
    float m_failureTimer;
    float m_diceDuration;
    bool  m_diceLanded;
    int   m_currentSlot;
    bool  m_navButtonDown;
    bool  m_enterButtonDown;
    int   m_lastDiceRoll;
    bool  m_showDiceResult;
    bool  m_lastStealSuccess;
    bool  m_playerInventoryOpen;
    bool  m_playerInventoryKeyDown;
    int   m_goldAmount;

    float m_slotCoords[6][2];
    float m_playerSlotCoords[6][2];

    int  ComputeGoldCount(const std::vector<Item>& playerInventory) const;
    void DrawItemIcon(int itemId, float x, float y);

public:
    bool inPickpocketUI;

    UIManager();
    ~UIManager();

    void Update(float deltaTime, NPC* activeNPC, std::vector<Item>& playerInventory);
    void Render(NPC* activeNPC, std::vector<Item>& playerInventory);
    void OpenUI();
    void CloseUI();

    bool IsAnyUIOpen()   const { return m_playerInventoryOpen || inPickpocketUI; }
    int  GetGoldAmount() const { return m_goldAmount; }
    void SetGoldAmount(int g) { m_goldAmount = g; }
};