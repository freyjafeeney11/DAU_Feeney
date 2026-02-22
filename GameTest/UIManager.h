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
    CSimpleSprite* inventory_screen;
    CSimpleSprite* rosamund_inv_sprite;
    CSimpleSprite* randy_inv_sprite;
    CSimpleSprite* granny_inv_sprite;
    CSimpleSprite* npcPortrait;

    CSimpleSprite* icon_gold;
    CSimpleSprite* icon_letter;
    CSimpleSprite* icon_flashdrive;
    CSimpleSprite* icon_picture;
    CSimpleSprite* icon_gold_small;
    CSimpleSprite* icon_letter_small;
    CSimpleSprite* icon_flashdrive_small;
    CSimpleSprite* icon_picture_small;

    CSimpleSprite* ui_cursor;
    int currentSlot;
    bool navButtonDown;
    bool enterButtonDown;
    int lastDiceRoll;
    bool showDiceResult;
    bool lastStealSuccess;

    float slotCoords[6][2];

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