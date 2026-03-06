// UIManager.h
#pragma once
#include "app\app.h"
#include "NPC.h"
#include <vector>
#include <string>

struct Item {
    int id;
    std::string name;
    std::string flavorText;
    int value;
};

enum { ITEM_NONE, ITEM_GOLD, ITEM_FLASHDRIVE, ITEM_LETTER, ITEM_PICTURE };

class UIManager {
private:
    CSimpleSprite* m_inventory_screen;
    CSimpleSprite* m_rosamund_inv_sprite;
    CSimpleSprite* m_randy_inv_sprite;
    CSimpleSprite* m_granny_inv_sprite;
    CSimpleSprite* m_npcPortrait;

    CSimpleSprite* m_icon_gold;
    CSimpleSprite* m_icon_letter;
    CSimpleSprite* m_icon_flashdrive;
    CSimpleSprite* m_icon_picture;
    CSimpleSprite* m_icon_gold_small;
    CSimpleSprite* m_icon_letter_small;
    CSimpleSprite* m_icon_flashdrive_small;
    CSimpleSprite* m_icon_picture_small;

    CSimpleSprite* m_ui_cursor;
    int m_currentSlot;
    bool m_navButtonDown;
    bool m_enterButtonDown;
    int m_lastDiceRoll;
    bool m_showDiceResult;
    bool m_lastStealSuccess;

    float m_slotCoords[6][2];

    Item GetItemFromLibrary(int itemId);

public:
    bool inPickpocketUI;

    UIManager();
    ~UIManager();
    void Update(float deltaTime, NPC* activeNPC, std::vector<Item>& playerInventory);
    void Render(NPC* activeNPC, std::vector<Item>& playerInventory);

    void OpenUI();
    void CloseUI();
};