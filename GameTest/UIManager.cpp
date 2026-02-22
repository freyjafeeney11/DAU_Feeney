// UIManager.cpp
#include "stdafx.h"
#include "UIManager.h"

UIManager::UIManager() {
    inPickpocketUI = false;
    currentSlot = 0;
    navButtonDown = false;
    enterButtonDown = false;
    showDiceResult = false;
    lastDiceRoll = 0;
    lastStealSuccess = false;

    float coords[6][2] = {
        { 190.0f, 600.0f }, { 310.0f, 600.0f }, { 450.0f, 600.0f },
        { 190.0f, 450.0f }, { 310.0f, 450.0f }, { 450.0f, 450.0f }
    };
    for (int i = 0; i < 6; i++) {
        slotCoords[i][0] = coords[i][0];
        slotCoords[i][1] = coords[i][1];
    }

    inventory_screen = App::CreateSprite(".\\TestData\\Inventory.png", 1, 1);
    inventory_screen->SetPosition(500.0f, 400.0f);
    inventory_screen->SetScale(0.6f);

    rosamund_inv_sprite = App::CreateSprite(".\\TestData\\rosamund_portrait_2.png", 1, 1);
    rosamund_inv_sprite->SetPosition(500.0f, 400.0f);
    rosamund_inv_sprite->SetScale(0.6f);

    randy_inv_sprite = App::CreateSprite(".\\TestData\\randy_portrait_2.png", 1, 1);
    randy_inv_sprite->SetPosition(500.0f, 400.0f);
    randy_inv_sprite->SetScale(0.6f);

    granny_inv_sprite = App::CreateSprite(".\\TestData\\granny_portrait.png", 1, 1);
    granny_inv_sprite->SetPosition(500.0f, 400.0f);
    granny_inv_sprite->SetScale(0.6f);

    npcPortrait = App::CreateSprite(".\\TestData\\pickpocket_dialogue.png", 1, 1);
    npcPortrait->SetPosition(500.0f, 400.0f);
    npcPortrait->SetScale(0.6f);

    icon_gold_small = App::CreateSprite(".\\TestData\\gold_icon_small.png", 1, 1);
    icon_gold_small->SetPosition(500.0f, 400.0f);
    icon_gold_small->SetScale(0.6f);
    icon_flashdrive_small = App::CreateSprite(".\\TestData\\flashdrive_icon_small.png", 1, 1);
    icon_flashdrive_small->SetPosition(500.0f, 400.0f);
    icon_flashdrive_small->SetScale(0.6f);
    icon_letter_small = App::CreateSprite(".\\TestData\\Letter_Icon_small.png", 1, 1);
    icon_letter_small->SetPosition(500.0f, 400.0f);
    icon_letter_small->SetScale(0.6f);
    icon_picture_small = App::CreateSprite(".\\TestData\\picture_icon.png", 1, 1);
    icon_picture_small->SetPosition(500.0f, 400.0f);
    icon_picture_small->SetScale(0.6f);

    icon_gold = App::CreateSprite(".\\TestData\\gold_icon.png", 1, 1);
    icon_gold->SetPosition(500.0f, 400.0f);
    icon_gold->SetScale(0.6f);
    icon_flashdrive = App::CreateSprite(".\\TestData\\flashdrive_icon.png", 1, 1);
    icon_flashdrive->SetPosition(500.0f, 400.0f);
    icon_flashdrive->SetScale(0.6f);
    icon_letter = App::CreateSprite(".\\TestData\\Letter_Icon.png", 1, 1);
    icon_letter->SetPosition(500.0f, 400.0f);
    icon_letter->SetScale(0.6f);
    icon_picture = App::CreateSprite(".\\TestData\\picture.png", 1, 1);
    icon_picture->SetPosition(500.0f, 400.0f);
    icon_picture->SetScale(0.6f);

    ui_cursor = App::CreateSprite(".\\TestData\\mask_temp.png", 1, 1);
    ui_cursor->SetScale(0.4f);
}

UIManager::~UIManager() {
    delete inventory_screen;
    delete rosamund_inv_sprite;
    delete randy_inv_sprite;
    delete granny_inv_sprite;
    delete npcPortrait;
    delete icon_gold_small;
    delete icon_flashdrive_small;
    delete icon_letter_small;
    delete icon_picture_small;
    delete icon_gold;
    delete icon_flashdrive;
    delete icon_letter;
    delete icon_picture;
    delete ui_cursor;
}

Item UIManager::GetItemFromLibrary(int itemId) {
    switch (itemId) {
    case ITEM_GOLD: return { ITEM_GOLD, "A sack of gold", "It's gold.", 50 };
    case ITEM_LETTER: return { ITEM_LETTER, "Perfumed Letter", "It says: idk ill come up with something", 0 };
    case ITEM_FLASHDRIVE: return { ITEM_FLASHDRIVE, "FlashDrive", "I wonder what's on this...", 100 };
    case ITEM_PICTURE: return { ITEM_PICTURE, "Old Photograph", "It's a photo of two people on a beach.", 100 };
    default: return { ITEM_NONE, "Empty Slot", "Put something here..", 0 };
    }
}

void UIManager::OpenUI() {
    inPickpocketUI = true;
    showDiceResult = false;
    currentSlot = 0;
}

void UIManager::CloseUI() {
    inPickpocketUI = false;
}

void UIManager::Update(float deltaTime, NPC* activeNPC, std::vector<Item>& playerInventory) {
    if (!inPickpocketUI) return;

    if (App::IsKeyPressed(VK_CONTROL)) {
        CloseUI();
        return;
    }

    if (!navButtonDown) {
        if (App::IsKeyPressed(VK_RIGHT)) {
            if (currentSlot == 0) currentSlot = 1; else if (currentSlot == 1) currentSlot = 2; else if (currentSlot == 2) currentSlot = 0;
            else if (currentSlot == 3) currentSlot = 4; else if (currentSlot == 4) currentSlot = 5; else if (currentSlot == 5) currentSlot = 3;
            navButtonDown = true;
        }
        if (App::IsKeyPressed(VK_LEFT)) {
            if (currentSlot == 1) currentSlot = 0; else if (currentSlot == 2) currentSlot = 1; else if (currentSlot == 0) currentSlot = 2;
            else if (currentSlot == 4) currentSlot = 3; else if (currentSlot == 5) currentSlot = 4; else if (currentSlot == 3) currentSlot = 5;
            navButtonDown = true;
        }
        if (App::IsKeyPressed(VK_DOWN)) {
            if (currentSlot >= 0 && currentSlot <= 2) currentSlot += 3;
            navButtonDown = true;
        }
        if (App::IsKeyPressed(VK_UP)) {
            if (currentSlot >= 3 && currentSlot <= 5) currentSlot -= 3;
            navButtonDown = true;
        }
    }

    if (!App::IsKeyPressed(VK_RIGHT) && !App::IsKeyPressed(VK_LEFT) && !App::IsKeyPressed(VK_DOWN) && !App::IsKeyPressed(VK_UP)) {
        navButtonDown = false;
    }

    if (App::IsKeyPressed(VK_RETURN) && !enterButtonDown) {
        enterButtonDown = true;

        if (!activeNPC) return;

        int* currentTable = activeNPC->GetLootTable();
        int difficulty = activeNPC->GetDifficulty();

        if (currentTable[currentSlot] != ITEM_NONE) {
            lastDiceRoll = (rand() % 20) + 1;

            if (lastDiceRoll >= difficulty) {
                lastStealSuccess = true;
                int stolenId = currentTable[currentSlot];
                playerInventory.push_back(GetItemFromLibrary(stolenId));
                currentTable[currentSlot] = ITEM_NONE;
            }
            else {
                lastStealSuccess = false;
                activeNPC->SetAlerted(true);
                CloseUI();
            }
            showDiceResult = true;
        }
    }

    if (!App::IsKeyPressed(VK_RETURN)) {
        enterButtonDown = false;
    }
}

void UIManager::Render(NPC* activeNPC, std::vector<Item>& playerInventory) {
    // players inventory - really basic for now
    if (App::IsKeyPressed('I')) {
        App::Print(100, 700, "--- YOUR INVENTORY ---", 1.0f, 1.0f, 1.0f);
        for (int i = 0; i < (int)playerInventory.size(); i++) {
            float yOffset = 650.0f - (i * 30.0f);
            App::Print(100, yOffset, playerInventory[i].name.c_str(), 1.0f, 1.0f, 1.0f);
        }
    }

    if (!inPickpocketUI) return;

    inventory_screen->Draw();

    if (activeNPC) {
        if (activeNPC->GetName() == "Rosamund") rosamund_inv_sprite->Draw();
        else if (activeNPC->GetName() == "Randy") randy_inv_sprite->Draw();
        else if (activeNPC->GetName() == "Granny") granny_inv_sprite->Draw();

        int* currentTable = activeNPC->GetLootTable();
        for (int i = 0; i < 6; i++) {
            int itemID = currentTable[i];
            if (itemID == ITEM_GOLD) icon_gold_small->Draw();
            else if (itemID == ITEM_FLASHDRIVE) icon_flashdrive_small->Draw();
            else if (itemID == ITEM_LETTER) icon_letter_small->Draw();
            else if (itemID == ITEM_PICTURE) icon_picture_small->Draw();
        }

        ui_cursor->SetPosition(slotCoords[currentSlot][0], slotCoords[currentSlot][1]);
        ui_cursor->Draw();

        int selectedItem = currentTable[currentSlot];
        if (selectedItem == ITEM_GOLD) icon_gold->Draw();
        else if (selectedItem == ITEM_FLASHDRIVE) icon_flashdrive->Draw();
        else if (selectedItem == ITEM_LETTER) icon_letter->Draw();
        else if (selectedItem == ITEM_PICTURE) icon_picture->Draw();
    }

    if (showDiceResult) {
        char resultText[100];
        char resultText1[100];
        sprintf(resultText, "%d", lastDiceRoll);
        if (lastStealSuccess) {
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
    else {
        App::Print(10, 100, "Press Enter to Steal", 0.0f, 0.0f, 0.0f);
    }
}