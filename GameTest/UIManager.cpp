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
    m_diceDuration = 0.5f;
    m_diceLanded = false;
    m_failureTimer = 0.0f;

    float coords[6][2] = {
        { 190.0f, 600.0f }, { 310.0f, 600.0f }, { 450.0f, 600.0f },
        { 190.0f, 450.0f }, { 310.0f, 450.0f }, { 450.0f, 450.0f }
    };
    for (int i = 0; i < 6; i++) {
        m_slotCoords[i][0] = coords[i][0];
        m_slotCoords[i][1] = coords[i][1];
        m_playerSlotCoords[i][0] = coords[i][0];
        m_playerSlotCoords[i][1] = coords[i][1];
    }

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
    m_player_watch_sprite->SetPosition(700.0f, 520.0f);
    m_player_watch_sprite->SetScale(0.12f);

    m_icon_gold_small_hud = App::CreateSprite(".\\TestData\\gold_icon_cropped.png", 1, 1);
    m_icon_gold_small_hud->SetPosition(940.0f, 490.0f);
    m_icon_gold_small_hud->SetScale(0.7f);

    m_rosamund_inv_sprite = App::CreateSprite(".\\TestData\\rosamund_portrait_2.png", 1, 1);
    m_rosamund_inv_sprite->SetPosition(500.0f, 400.0f);
    m_rosamund_inv_sprite->SetScale(0.6f);

    m_randy_inv_sprite = App::CreateSprite(".\\TestData\\randy_portrait_2.png", 1, 1);
    m_randy_inv_sprite->SetPosition(500.0f, 400.0f);
    m_randy_inv_sprite->SetScale(0.6f);

    m_granny_inv_sprite = App::CreateSprite(".\\TestData\\granny_portrait.png", 1, 1);
    m_granny_inv_sprite->SetPosition(500.0f, 400.0f);
    m_granny_inv_sprite->SetScale(0.6f);

    m_player_icon_gold_small = App::CreateSprite(".\\TestData\\gold_icon_cropped.png", 1, 1);
    m_player_icon_gold_small->SetScale(0.6f);
    m_player_icon_flashdrive_small = App::CreateSprite(".\\TestData\\flashdrive_icon_cropped.png", 1, 1);
    m_player_icon_flashdrive_small->SetScale(0.6f);
    m_player_icon_letter_small = App::CreateSprite(".\\TestData\\letter_icon_cropped.png", 1, 1);
    m_player_icon_letter_small->SetScale(0.6f);
    m_player_icon_picture_small = App::CreateSprite(".\\TestData\\picture_icon_cropped.png", 1, 1);
    m_player_icon_picture_small->SetScale(0.6f);

    m_icon_gold_small = m_player_icon_gold_small;
    m_icon_flashdrive_small = m_player_icon_flashdrive_small;
    m_icon_letter_small = m_player_icon_letter_small;
    m_icon_picture_small = m_player_icon_picture_small;

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
    delete m_player_icon_gold_small;
    delete m_player_icon_flashdrive_small;
    delete m_player_icon_letter_small;
    delete m_player_icon_picture_small;
    delete m_icon_gold_small_hud;
    delete m_icon_gold;
    delete m_icon_flashdrive;
    delete m_icon_letter;
    delete m_icon_picture;
    delete m_ui_cursor;
}

void UIManager::DrawItemIconSmall(int itemId, float x, float y) {
    CSimpleSprite* icon = nullptr;
    if (itemId == ITEM_GOLD)            icon = m_player_icon_gold_small;
    else if (itemId == ITEM_FLASHDRIVE) icon = m_player_icon_flashdrive_small;
    else if (itemId == ITEM_LETTER)     icon = m_player_icon_letter_small;
    else if (itemId == ITEM_PICTURE)    icon = m_player_icon_picture_small;

    if (icon) {
        icon->SetPosition(x, y);
        icon->Draw();
    }
}

int UIManager::ComputeGoldCount(const std::vector<Item>& playerInventory) const {
    int total = 0;
    for (const auto& item : playerInventory) {
        if (item.id == ITEM_GOLD) total += 50;
    }
    return total;
}

bool UIManager::IsAnyUIOpen() const {
    return m_playerInventoryOpen || inPickpocketUI;
}

void UIManager::OpenUI() {
    m_enterButtonDown = true;
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
}

void UIManager::Update(float deltaTime, NPC* activeNPC, std::vector<Item>& playerInventory) {
    if (App::IsKeyPressed('I') && !m_playerInventoryKeyDown) {
        m_playerInventoryKeyDown = true;
        m_playerInventoryOpen = !m_playerInventoryOpen;
    }
    if (!App::IsKeyPressed('I')) m_playerInventoryKeyDown = false;

    if (!inPickpocketUI) return;

    float dt = deltaTime / 1000.0f;

    if (App::IsKeyPressed(VK_BACK)) {
        CloseUI();
        return;
    }

    if (m_diceLanded && !m_lastStealSuccess) {
        m_failureTimer += dt;
        if (m_failureTimer >= 1.0f) CloseUI();
        return;
    }

    if (!m_navButtonDown) {
        if (App::IsKeyPressed(VK_RIGHT)) {
            if (m_currentSlot % 3 < 2) m_currentSlot++;
            m_navButtonDown = true;
        }
        if (App::IsKeyPressed(VK_LEFT)) {
            if (m_currentSlot % 3 > 0) m_currentSlot--;
            m_navButtonDown = true;
        }
        if (App::IsKeyPressed(VK_DOWN)) {
            if (m_currentSlot < 3) m_currentSlot += 3;
            m_navButtonDown = true;
        }
        if (App::IsKeyPressed(VK_UP)) {
            if (m_currentSlot >= 3) m_currentSlot -= 3;
            m_navButtonDown = true;
        }
    }

    if (!App::IsKeyPressed(VK_RIGHT) && !App::IsKeyPressed(VK_LEFT) &&
        !App::IsKeyPressed(VK_DOWN) && !App::IsKeyPressed(VK_UP)) m_navButtonDown = false;
    if (App::IsKeyPressed(VK_RETURN) && !m_enterButtonDown) {
        m_enterButtonDown = true;
        if (!activeNPC) return;

        Item* currentTable = activeNPC->GetLootTable();
        int difficulty = activeNPC->GetDifficulty();

        if (currentTable[m_currentSlot].id != ITEM_NONE) {
            m_lastDiceRoll = (rand() % 20) + 1;
            if (m_lastDiceRoll >= difficulty) {
                m_lastStealSuccess = true;
                if (currentTable[m_currentSlot].id == ITEM_GOLD) {
                    m_goldAmount += 10; // dont move the gold icon over just increment counter
                    currentTable[m_currentSlot] = Item();
                }
                else {
                    playerInventory.push_back(std::move(currentTable[m_currentSlot]));
                }

                App::PlaySound(".\\TestData\\gold_steal.wav", false);
            }
            else {
                m_lastStealSuccess = false;
                activeNPC->SetAlerted(true);
            }
            m_showDiceResult = true;
            m_diceLanded = false;
            m_diceTimer = 0.0f;
            m_dice_roll->SetAnimation(0);
        }
    }

    if (!App::IsKeyPressed(VK_RETURN)) m_enterButtonDown = false;

    if (m_showDiceResult && !m_diceLanded) {
        m_diceTimer += dt;
        m_dice_roll->Update(deltaTime);
        if (m_diceTimer >= m_diceDuration) m_diceLanded = true;
    }
}

void UIManager::Render(NPC* activeNPC, std::vector<Item>& playerInventory) {
    if (m_playerInventoryOpen && !inPickpocketUI) {
        m_player_inventory_screen->Draw();
        m_player_watch_sprite->Draw();

        int maxSlots = playerInventory.size() < 6 ? (int)playerInventory.size() : 6;
        for (int i = 0; i < maxSlots; i++) {
            DrawItemIconSmall(playerInventory[i].id, m_playerSlotCoords[i][0], m_playerSlotCoords[i][1]);
        }

        char goldText[32];
        sprintf(goldText, "%d", m_goldAmount);
        m_icon_gold_small_hud->Draw();
        App::Print(920, 423, goldText, 1.0f, 1.0f, 1.0f);
    }

    if (!inPickpocketUI) return;

    m_inventory_screen->Draw();

    if (activeNPC) {
        if (activeNPC->GetName() == "Rosamund") m_rosamund_inv_sprite->Draw();
        else if (activeNPC->GetName() == "Randy") m_randy_inv_sprite->Draw();
        else if (activeNPC->GetName() == "Granny") m_granny_inv_sprite->Draw();

        Item* currentTable = activeNPC->GetLootTable();
        for (int i = 0; i < 6; i++) {
            if (currentTable[i].id != ITEM_NONE) {
                DrawItemIconSmall(currentTable[i].id, m_slotCoords[i][0], m_slotCoords[i][1]);
            }
        }

        m_ui_cursor->SetPosition(m_slotCoords[m_currentSlot][0], m_slotCoords[m_currentSlot][1]);
        m_ui_cursor->Draw();
        Item& selectedItem = currentTable[m_currentSlot];

        if (selectedItem.id != ITEM_NONE) {
            App::Print(120, 310, selectedItem.name.c_str(), 1.0f, 1.0f, 0.0f);
            App::Print(120, 280, selectedItem.flavorText.c_str(), 1.0f, 1.0f, 1.0f);
        }
    }

    if (m_showDiceResult) {
        m_dice_roll->Draw();
        if (m_diceLanded) {
            char res[32];
            sprintf(res, "%d", m_lastDiceRoll);
            App::Print(925, 275, res, 0.0f, 0.0f, 0.0f);
            App::Print(890, 190, m_lastStealSuccess ? "Success!" : "Failure",
                m_lastStealSuccess ? 0.0f : 1.0f,
                m_lastStealSuccess ? 1.0f : 0.0f, 0.0f);
        }
    }
}