#include "stdafx.h"
#include "UIManager.h"

UIManager::UIManager() {
    m_playerInventoryOpen = false;
    m_playerInventoryKeyDown = false;
    inPickpocketUI = false;
    m_currentSlot = 0;
    m_navButtonDown = false;
    m_enterButtonDown = false;
    m_showDiceResult = false;
    m_lastDiceRoll = 0;
    m_lastStealSuccess = false;
    m_diceTimer = 0.0f;
    m_diceDuration = 0.1f * 5.0f;
    m_diceLanded = false;
    m_failureTimer = 0.0f;

    float coords[6][2] = {
        { 190.0f, 600.0f }, { 310.0f, 600.0f }, { 450.0f, 600.0f },
        { 190.0f, 450.0f }, { 310.0f, 450.0f }, { 450.0f, 450.0f }
    };
    for (int i = 0; i < 6; i++) {
        m_slotCoords[i][0] = coords[i][0];
        m_slotCoords[i][1] = coords[i][1];
    }

    //float playerCoords[6][2] = {
    //    { 190.0f, 600.0f }, { 310.0f, 600.0f }, { 450.0f, 600.0f },
    //    { 190.0f, 450.0f }, { 310.0f, 450.0f }, { 450.0f, 450.0f }
    //};
    //for (int i = 0; i < 6; i++) {
    //    m_playerSlotCoords[i][0] = playerCoords[i][0];
    //    m_playerSlotCoords[i][1] = playerCoords[i][1];
    //}

    m_dice_roll = App::CreateSprite(".\\TestData\\dice_roll.png", 5, 1);
    m_dice_roll->SetPosition(946.0f, 277.0f);
    m_dice_roll->SetScale(0.1f);
    m_dice_roll->CreateAnimation(0, 0.1f, { 0,1,2,3,4 });
    m_dice_roll->SetAnimation(0);

    m_inventory_screen = App::CreateSprite(".\\TestData\\Inventory.png", 1, 1);
    m_inventory_screen->SetPosition(500.0f, 400.0f);
    m_inventory_screen->SetScale(0.6f);

    m_player_inventory_screen = App::CreateSprite(".\\TestData\\Inventory.png", 1, 1);
    m_player_inventory_screen->SetPosition(500.0f, 400.0f);
    m_player_inventory_screen->SetScale(0.6f);

    m_player_watch_sprite = App::CreateSprite(".\\TestData\\watch.png", 1, 1);
    m_player_watch_sprite->SetPosition(710.0f, 550.0f);
    m_player_watch_sprite->SetScale(0.12f);

    m_icon_gold_small_hud = App::CreateSprite(".\\TestData\\gold_icon_small.png", 1, 1);
    m_icon_gold_small_hud->SetPosition(780.0f, 520.0f);
    m_icon_gold_small_hud->SetScale(0.6f);

    m_rosamund_inv_sprite = App::CreateSprite(".\\TestData\\rosamund_portrait_2.png", 1, 1);
    m_rosamund_inv_sprite->SetPosition(500.0f, 400.0f);
    m_rosamund_inv_sprite->SetScale(0.6f);

    m_randy_inv_sprite = App::CreateSprite(".\\TestData\\randy_portrait_2.png", 1, 1);
    m_randy_inv_sprite->SetPosition(500.0f, 400.0f);
    m_randy_inv_sprite->SetScale(0.6f);

    m_granny_inv_sprite = App::CreateSprite(".\\TestData\\granny_portrait.png", 1, 1);
    m_granny_inv_sprite->SetPosition(500.0f, 400.0f);
    m_granny_inv_sprite->SetScale(0.6f);

    m_npcPortrait = App::CreateSprite(".\\TestData\\pickpocket_dialogue.png", 1, 1);
    m_npcPortrait->SetPosition(500.0f, 400.0f);
    m_npcPortrait->SetScale(0.6f);

    m_icon_gold_small = App::CreateSprite(".\\TestData\\gold_icon_small.png", 1, 1);
    m_icon_gold_small->SetPosition(500.0f, 400.0f);
    m_icon_gold_small->SetScale(0.6f);
    m_icon_flashdrive_small = App::CreateSprite(".\\TestData\\flashdrive_icon_small.png", 1, 1);
    m_icon_flashdrive_small->SetPosition(500.0f, 400.0f);
    m_icon_flashdrive_small->SetScale(0.6f);
    m_icon_letter_small = App::CreateSprite(".\\TestData\\Letter_Icon_small.png", 1, 1);
    m_icon_letter_small->SetPosition(500.0f, 400.0f);
    m_icon_letter_small->SetScale(0.6f);
    m_icon_picture_small = App::CreateSprite(".\\TestData\\picture_icon.png", 1, 1);
    m_icon_picture_small->SetPosition(500.0f, 400.0f);
    m_icon_picture_small->SetScale(0.6f);

    m_player_icon_gold_small = App::CreateSprite(".\\TestData\\gold_icon_small.png", 1, 1);
    m_player_icon_gold_small->SetScale(0.6f);
    m_player_icon_flashdrive_small = App::CreateSprite(".\\TestData\\flashdrive_icon_small.png", 1, 1);
    m_player_icon_flashdrive_small->SetScale(0.6f);
    m_player_icon_letter_small = App::CreateSprite(".\\TestData\\Letter_Icon_small.png", 1, 1);
    m_player_icon_letter_small->SetScale(0.6f);
    m_player_icon_picture_small = App::CreateSprite(".\\TestData\\picture_icon.png", 1, 1);
    m_player_icon_picture_small->SetScale(0.6f);

    m_icon_gold = App::CreateSprite(".\\TestData\\gold_icon.png", 1, 1);
    m_icon_gold->SetPosition(500.0f, 400.0f);
    m_icon_gold->SetScale(0.6f);
    m_icon_flashdrive = App::CreateSprite(".\\TestData\\flashdrive_icon.png", 1, 1);
    m_icon_flashdrive->SetPosition(500.0f, 400.0f);
    m_icon_flashdrive->SetScale(0.6f);
    m_icon_letter = App::CreateSprite(".\\TestData\\Letter_Icon.png", 1, 1);
    m_icon_letter->SetPosition(500.0f, 400.0f);
    m_icon_letter->SetScale(0.6f);
    m_icon_picture = App::CreateSprite(".\\TestData\\picture.png", 1, 1);
    m_icon_picture->SetPosition(500.0f, 400.0f);
    m_icon_picture->SetScale(0.6f);

    m_ui_cursor = App::CreateSprite(".\\TestData\\mask_temp.png", 1, 1);
    m_ui_cursor->SetScale(0.4f);
}

UIManager::~UIManager() {
    delete m_dice_roll;
    delete m_inventory_screen;
    delete m_player_inventory_screen;
    delete m_player_watch_sprite;
    delete m_rosamund_inv_sprite;
    delete m_randy_inv_sprite;
    delete m_granny_inv_sprite;
    delete m_npcPortrait;
    delete m_icon_gold_small;
    delete m_icon_flashdrive_small;
    delete m_icon_letter_small;
    delete m_icon_picture_small;
    delete m_icon_gold_small_hud;
    delete m_player_icon_gold_small;
    delete m_player_icon_flashdrive_small;
    delete m_player_icon_letter_small;
    delete m_player_icon_picture_small;
    delete m_icon_gold;
    delete m_icon_flashdrive;
    delete m_icon_letter;
    delete m_icon_picture;
    delete m_ui_cursor;
}

Item UIManager::GetItemFromLibrary(int itemId) {
    switch (itemId) {
    case ITEM_GOLD:       return { ITEM_GOLD,       "A sack of gold",    "It's gold.",                              50 };
    case ITEM_LETTER:     return { ITEM_LETTER,     "Perfumed Letter",   "It says: idk ill come up with something",  0 };
    case ITEM_FLASHDRIVE: return { ITEM_FLASHDRIVE, "FlashDrive",        "I wonder what's on this...",             100 };
    case ITEM_PICTURE:    return { ITEM_PICTURE,    "Old Photograph",    "It's a photo of two people on a beach.", 100 };
    default:              return { ITEM_NONE,       "Empty Slot",        "Put something here..",                     0 };
    }
}

int UIManager::ComputeGoldCount(const std::vector<Item>& playerInventory) const {
    int total = 0;
    for (const Item& item : playerInventory) {
        if (item.id == ITEM_GOLD) total += 10;
    }
    return total;
}

void UIManager::DrawPlayerItemIconSmall(int itemId, float x, float y) {
    CSimpleSprite* icon = nullptr;
    if (itemId == ITEM_GOLD)       icon = m_player_icon_gold_small;
    else if (itemId == ITEM_FLASHDRIVE) icon = m_player_icon_flashdrive_small;
    else if (itemId == ITEM_LETTER)     icon = m_player_icon_letter_small;
    else if (itemId == ITEM_PICTURE)    icon = m_player_icon_picture_small;

    if (icon) {
        icon->SetPosition(x, y);
        icon->Draw();
    }
}

void UIManager::OpenUI() {
    inPickpocketUI = true;
    m_showDiceResult = false;
    m_diceLanded = false;
    m_diceTimer = 0.0f;
    m_failureTimer = 0.0f;
    m_currentSlot = 0;
}

void UIManager::CloseUI() {
    inPickpocketUI = false;
    m_showDiceResult = false;
    m_diceLanded = false;
    m_diceTimer = 0.0f;
    m_failureTimer = 0.0f;
}

void UIManager::Update(float deltaTime, NPC* activeNPC, std::vector<Item>& playerInventory) {
    if (App::IsKeyPressed('I') && !m_playerInventoryKeyDown) {
        m_playerInventoryKeyDown = true;
        m_playerInventoryOpen = !m_playerInventoryOpen;
    }
    if (!App::IsKeyPressed('I')) {
        m_playerInventoryKeyDown = false;
    }


    if (!inPickpocketUI) return;

    float dt = deltaTime / 1000.0f;

    if (App::IsKeyPressed(VK_CONTROL)) {
        CloseUI();
        return;
    }

    if (m_diceLanded && !m_lastStealSuccess) {
        m_failureTimer += dt;
        if (m_failureTimer >= 1.0f) {
            CloseUI();
        }
        return;
    }

    if (!m_navButtonDown) {
        if (App::IsKeyPressed(VK_RIGHT)) {
            if (m_currentSlot == 0) m_currentSlot = 1; else if (m_currentSlot == 1) m_currentSlot = 2; else if (m_currentSlot == 2) m_currentSlot = 0;
            else if (m_currentSlot == 3) m_currentSlot = 4; else if (m_currentSlot == 4) m_currentSlot = 5; else if (m_currentSlot == 5) m_currentSlot = 3;
            m_navButtonDown = true;
        }
        if (App::IsKeyPressed(VK_LEFT)) {
            if (m_currentSlot == 1) m_currentSlot = 0; else if (m_currentSlot == 2) m_currentSlot = 1; else if (m_currentSlot == 0) m_currentSlot = 2;
            else if (m_currentSlot == 4) m_currentSlot = 3; else if (m_currentSlot == 5) m_currentSlot = 4; else if (m_currentSlot == 3) m_currentSlot = 5;
            m_navButtonDown = true;
        }
        if (App::IsKeyPressed(VK_DOWN)) {
            if (m_currentSlot >= 0 && m_currentSlot <= 2) m_currentSlot += 3;
            m_navButtonDown = true;
        }
        if (App::IsKeyPressed(VK_UP)) {
            if (m_currentSlot >= 3 && m_currentSlot <= 5) m_currentSlot -= 3;
            m_navButtonDown = true;
        }
    }

    if (!App::IsKeyPressed(VK_RIGHT) && !App::IsKeyPressed(VK_LEFT) &&
        !App::IsKeyPressed(VK_DOWN) && !App::IsKeyPressed(VK_UP)) {
        m_navButtonDown = false;
    }

    if (App::IsKeyPressed(VK_RETURN) && !m_enterButtonDown) {
        m_enterButtonDown = true;

        if (!activeNPC) return;

        int* currentTable = activeNPC->GetLootTable();
        int difficulty = activeNPC->GetDifficulty();

        if (currentTable[m_currentSlot] != ITEM_NONE) {
            m_lastDiceRoll = (rand() % 20) + 1;

            if (m_lastDiceRoll >= difficulty) {
                m_lastStealSuccess = true;
                int stolenId = currentTable[m_currentSlot];
                playerInventory.push_back(GetItemFromLibrary(stolenId));
                currentTable[m_currentSlot] = ITEM_NONE;
                App::PlaySound(".\\TestData\\gold_steal.wav", false);
            }
            else {
                m_lastStealSuccess = false;
                activeNPC->SetAlerted(true);
            }

            m_showDiceResult = true;
            m_diceLanded = false;
            m_diceTimer = 0.0f;
            m_failureTimer = 0.0f;
            m_dice_roll->SetAnimation(0);
        }
    }

    if (!App::IsKeyPressed(VK_RETURN)) {
        m_enterButtonDown = false;
    }

    if (m_showDiceResult && !m_diceLanded) {
        m_diceTimer += dt;
        m_dice_roll->Update(deltaTime);
        if (m_diceTimer >= m_diceDuration) {
            m_diceLanded = true;
        }
    }
}

void UIManager::Render(NPC* activeNPC, std::vector<Item>& playerInventory) {
if (m_playerInventoryOpen && !inPickpocketUI) {
    m_player_inventory_screen->Draw();
    m_player_watch_sprite->Draw();

    App::Print(100, 700, "--- YOUR INVENTORY ---", 1.0f, 1.0f, 1.0f);
    for (int i = 0; i < (int)playerInventory.size(); i++) {
        float yOffset = 650.0f - (i * 30.0f);
        App::Print(100, yOffset, playerInventory[i].name.c_str(), 1.0f, 1.0f, 1.0f);
    }

    int goldCount = ComputeGoldCount(playerInventory);
    char goldText[32];
    sprintf(goldText, "%d", goldCount);
    m_icon_gold_small_hud->Draw();
    App::Print(810, 513, goldText, 1.0f, 1.0f, 1.0f);
}

    if (!inPickpocketUI) return;

    m_inventory_screen->Draw();

    if (activeNPC) {
        if (activeNPC->GetName() == "Rosamund") m_rosamund_inv_sprite->Draw();
        else if (activeNPC->GetName() == "Randy")    m_randy_inv_sprite->Draw();
        else if (activeNPC->GetName() == "Granny")   m_granny_inv_sprite->Draw();

        int* currentTable = activeNPC->GetLootTable();
        for (int i = 0; i < 6; i++) {
            int itemID = currentTable[i];
            if (itemID == ITEM_GOLD)       m_icon_gold_small->Draw();
            else if (itemID == ITEM_FLASHDRIVE) m_icon_flashdrive_small->Draw();
            else if (itemID == ITEM_LETTER)     m_icon_letter_small->Draw();
            else if (itemID == ITEM_PICTURE)    m_icon_picture_small->Draw();
        }

        m_ui_cursor->SetPosition(m_slotCoords[m_currentSlot][0], m_slotCoords[m_currentSlot][1]);
        m_ui_cursor->Draw();

        int selectedItem = currentTable[m_currentSlot];
        if (selectedItem == ITEM_GOLD)       m_icon_gold->Draw();
        else if (selectedItem == ITEM_FLASHDRIVE) m_icon_flashdrive->Draw();
        else if (selectedItem == ITEM_LETTER)     m_icon_letter->Draw();
        else if (selectedItem == ITEM_PICTURE)    m_icon_picture->Draw();
    }

    if (m_showDiceResult) {
        m_dice_roll->Draw();
        if (m_diceLanded) {
            char resultText[100];
            char resultText1[100];
            sprintf(resultText, "%d", m_lastDiceRoll);
            if (m_lastStealSuccess) {
                sprintf(resultText1, "Success!");
                App::Print(925, 275, resultText, 0.0f, 0.0f, 0.0f);
                App::Print(890, 190, resultText1, 0.0f, 1.0f, 0.0f);
            }
            else {
                sprintf(resultText1, "Failure");
                App::Print(925, 275, resultText, 0.0f, 0.0f, 0.0f);
                App::Print(890, 190, resultText1, 1.0f, 0.0f, 0.0f);
            }
        }
    }
    else {
        App::Print(10, 100, "Press Enter to Steal", 0.0f, 0.0f, 0.0f);
    }
}