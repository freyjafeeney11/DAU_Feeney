#pragma once
#include "app\app.h"
#include "NPC.h"
#include "Item.h"
#include <vector>
#include <string>

class UIManager {
private:
    CSimpleSprite* m_inventory_screen;
    CSimpleSprite* m_player_inventory_screen;
    CSimpleSprite* m_player_watch_sprite;
    CSimpleSprite* m_rosamund_inv_sprite;
    CSimpleSprite* m_overlay_sprite;
    CSimpleSprite* m_randy_inv_sprite;
    CSimpleSprite* m_granny_inv_sprite;
    CSimpleSprite* m_npcPortrait;
    CSimpleSprite* m_dice_roll;
    CSimpleSprite* m_icon_gold;
    CSimpleSprite* m_icon_letter;
    CSimpleSprite* m_icon_flashdrive;
    CSimpleSprite* m_icon_picture;
    CSimpleSprite* m_icon_gold_small;
    CSimpleSprite* m_icon_letter_small;
    CSimpleSprite* m_icon_flashdrive_small;
    CSimpleSprite* m_icon_picture_small;
    CSimpleSprite* m_icon_gold_small_hud;
    CSimpleSprite* m_player_icon_gold_small;
    CSimpleSprite* m_player_icon_letter_small;
    CSimpleSprite* m_player_icon_flashdrive_small;
    CSimpleSprite* m_player_icon_picture_small;
    CSimpleSprite* m_ui_cursor;
    CSimpleSprite* m_player_inventory_screen_title;

    float m_diceTimer;
    int m_goldAmount = 0;
    float m_failureTimer;
    float m_diceDuration;
    bool m_diceLanded;
    int m_currentSlot;
    bool m_navButtonDown;
    bool m_enterButtonDown;
    int m_lastDiceRoll;
    bool m_showDiceResult;
    bool m_lastStealSuccess;
    bool m_playerInventoryOpen;
    bool m_playerInventoryKeyDown;

    float m_slotCoords[6][2];
    float m_playerSlotCoords[6][2];

    int ComputeGoldCount(const std::vector<Item>& playerInventory) const;
    void DrawItemIconSmall(int itemId, float x, float y);

public:
    bool inPickpocketUI;

    UIManager();
    ~UIManager();
    bool IsAnyUIOpen() const;
    void Update(float deltaTime, NPC* activeNPC, std::vector<Item>& playerInventory);
    void Render(NPC* activeNPC, std::vector<Item>& playerInventory);
    void OpenUI();
    void CloseUI();
};