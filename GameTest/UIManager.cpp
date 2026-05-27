#include "stdafx.h"
#include "UIManager.h"
#include "GearManager.h"
#include "PlayerArchetype.h"
#include <math.h>

UIManager::UIManager() {
    m_playerInventoryOpen    = false;
    m_playerInventoryKeyDown = false;
    inPickpocketUI           = false;
    m_currentSlot            = 0;
    m_navButtonDown          = false;
    m_enterButtonDown        = false;
    m_showDiceResult         = false;
    m_lastDiceRoll           = 0;
    m_lastStealSuccess       = false;
    m_diceTimer              = 0.0f;
    m_diceDuration           = 0.5f;
    m_diceLanded             = false;
    m_failureTimer           = 0.0f;
    m_goldAmount             = 0;
    m_pendingSlot            = -1;
    m_luckyNotifTimer        = 0.0f;
    m_showLuckyNotif         = false;
    m_rawDiceRoll            = 0;
    m_luckyPhase             = 0;
    m_luckyPhaseTimer        = 0.0f;
    m_playerInvSlot          = 0;
    m_playerInvNavDown       = false;

    float coords[6][2] = {
        { 180.0f, 600.0f }, { 310.0f, 600.0f }, { 450.0f, 600.0f },
        { 180.0f, 450.0f }, { 310.0f, 450.0f }, { 450.0f, 450.0f }
    };
    for (int i = 0; i < 6; i++) {
        m_slotCoords[i][0]       = coords[i][0];
        m_slotCoords[i][1]       = coords[i][1];
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
    m_inventory_screen->SetScale(0.607f);

    m_player_inventory_screen = App::CreateSprite(".\\TestData\\Inventory.png", 1, 1);
    m_player_inventory_screen->SetPosition(500.0f, 400.0f);
    m_player_inventory_screen->SetScale(0.6f);

    m_overlay = App::CreateSprite(".\\TestData\\overlay.png", 1, 1);
    m_overlay->SetPosition(500.0f, 400.0f);
    m_overlay->SetScale(0.6f);

    m_player_inventory_screen_title = App::CreateSprite(".\\TestData\\inventory_title.png", 1, 1);
    m_player_inventory_screen_title->SetPosition(500.0f, 400.0f);
    m_player_inventory_screen_title->SetScale(0.6f);

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

    
    m_charles_inv_sprite = App::CreateSprite(".\\TestData\\charles_portrait.png", 1, 1);
    m_charles_inv_sprite->SetPosition(707.0f, 515.0f);
    m_charles_inv_sprite->SetScale(0.4f);

    m_helene_inv_sprite = App::CreateSprite(".\\TestData\\helene_portrait.png", 1, 1);
    m_helene_inv_sprite->SetPosition(700.0f, 518.0f);
    m_helene_inv_sprite->SetScale(0.42f);

    m_lupine_inv_sprite = App::CreateSprite(".\\TestData\\lupine_portrait.png", 1, 1);
    m_lupine_inv_sprite->SetPosition(700.0f, 510.0f);
    m_lupine_inv_sprite->SetScale(0.4f);

    m_magda_inv_sprite = App::CreateSprite(".\\TestData\\magda_portrait.png", 1, 1);
    m_magda_inv_sprite->SetPosition(700.0f, 510.0f);
    m_magda_inv_sprite->SetScale(0.35f);

    
    m_icon_gold_small = App::CreateSprite(".\\TestData\\gold_icon_cropped.png", 1, 1);
    m_icon_gold_small->SetScale(0.6f);

    m_icon_flashdrive_small = App::CreateSprite(".\\TestData\\flashdrive_icon_cropped.png", 1, 1);
    m_icon_flashdrive_small->SetScale(0.6f);

    m_icon_letter_small = App::CreateSprite(".\\TestData\\letter_icon_cropped.png", 1, 1);
    m_icon_letter_small->SetScale(0.6f);

    m_icon_picture_small = App::CreateSprite(".\\TestData\\picture_icon_cropped.png", 1, 1);
    m_icon_picture_small->SetScale(0.6f);

    m_icon_bouquet = App::CreateSprite(".\\TestData\\bouquet_icon.png", 1, 1);
    m_icon_bouquet->SetScale(0.6f);

    m_icon_collar = App::CreateSprite(".\\TestData\\collar_icon.png", 1, 1);
    m_icon_collar->SetScale(0.6f);

    m_icon_rat = App::CreateSprite(".\\TestData\\rat_icon.png", 1, 1);
    m_icon_rat->SetScale(0.6f);

    m_icon_painting = App::CreateSprite(".\\TestData\\painting_icon.png", 1, 1);
    m_icon_painting->SetScale(0.6f);

    m_ui_cursor = App::CreateSprite(".\\TestData\\mask_temp.png", 1, 1);
    m_ui_cursor->SetScale(0.05f);
}

UIManager::~UIManager() {
    delete m_dice_roll;
    delete m_inventory_screen;
    delete m_player_inventory_screen;
    delete m_player_inventory_screen_title;
    delete m_player_watch_sprite;
    delete m_icon_gold_small_hud;
    delete m_rosamund_inv_sprite;
    delete m_randy_inv_sprite;
    delete m_granny_inv_sprite;
    delete m_charles_inv_sprite;
    delete m_helene_inv_sprite;
    delete m_lupine_inv_sprite;
    delete m_magda_inv_sprite;
    delete m_icon_gold_small;
    delete m_icon_flashdrive_small;
    delete m_icon_letter_small;
    delete m_icon_picture_small;
    delete m_icon_bouquet;
    delete m_icon_collar;
    delete m_icon_rat;
    delete m_icon_painting;
    delete m_ui_cursor;
    delete m_overlay;
}

void UIManager::DrawItemIcon(int itemId, float x, float y) {
    CSimpleSprite* icon = nullptr;
    switch (itemId) {
        case ITEM_GOLD:       icon = m_icon_gold_small;      break;
        case ITEM_FLASHDRIVE: icon = m_icon_flashdrive_small; break;
        case ITEM_LETTER:     icon = m_icon_letter_small;    break;
        case ITEM_BOOK:    icon = m_icon_picture_small;   break;
        case ITEM_BOUQUET:    icon = m_icon_bouquet;         break;
        case ITEM_COLLAR:     icon = m_icon_collar;          break;
        case ITEM_RAT:        icon = m_icon_rat;             break;
        case ITEM_PAINTING:   icon = m_icon_painting;        break;
        default: break;
    }
    if (icon) {
        icon->SetPosition(x, y);
        icon->Draw();
    }
}

void UIManager::OpenUI() {
    m_enterButtonDown = true;
    inPickpocketUI    = true;
    m_showDiceResult  = false;
    m_diceLanded      = false;
    m_diceTimer       = 0.0f;
    m_failureTimer    = 0.0f;
    m_currentSlot     = 0;
    m_lastDiceRoll    = 0;
    App::PlaySound(".\\TestData\\audio\\menu_open.wav", false);
    App::SetSoundVolume(".\\TestData\\audio\\menu_open.wav", 0.4f);

}

void UIManager::CloseUI() {
    inPickpocketUI   = false;
    m_showDiceResult = false;
    m_playerInventoryOpen = false;
}

void UIManager::Update(float deltaTime, NPC* activeNPC, std::vector<Item>& playerInventory) {
    if (App::IsKeyPressed('I') && !m_playerInventoryKeyDown) {
        m_playerInventoryKeyDown = true;

        App::PlaySound(".\\TestData\\audio\\menu_open.wav", false);
        App::SetSoundVolume(".\\TestData\\audio\\menu_open.wav", 0.4f);

        m_playerInventoryOpen    = !m_playerInventoryOpen;
    }
    if (!App::IsKeyPressed('I')) m_playerInventoryKeyDown = false;

    if (m_playerInventoryOpen && !inPickpocketUI) {
        if (!m_playerInvNavDown) {
            if (App::IsKeyPressed(VK_RIGHT)) {
                if (m_playerInvSlot % 3 < 2) m_playerInvSlot++;
                m_playerInvNavDown = true;
            }
            if (App::IsKeyPressed(VK_LEFT)) {
                if (m_playerInvSlot % 3 > 0) m_playerInvSlot--;
                m_playerInvNavDown = true;
            }
            if (App::IsKeyPressed(VK_DOWN)) {
                if (m_playerInvSlot < 3) m_playerInvSlot += 3;
                m_playerInvNavDown = true;
            }
            if (App::IsKeyPressed(VK_UP)) {
                if (m_playerInvSlot >= 3) m_playerInvSlot -= 3;
                m_playerInvNavDown = true;
            }
        }
        if (!App::IsKeyPressed(VK_RIGHT) && !App::IsKeyPressed(VK_LEFT) &&
            !App::IsKeyPressed(VK_DOWN)  && !App::IsKeyPressed(VK_UP))
            m_playerInvNavDown = false;
    }

    if (!inPickpocketUI) return;

    float dt = deltaTime / 1000.0f;

    if (App::IsKeyPressed(VK_ESCAPE)) {
        CloseUI();
        App::PlaySound(".\\TestData\\audio\\menu_open.wav", false);
        App::SetSoundVolume(".\\TestData\\audio\\menu_open.wav", 0.4f);
        return;
    }

    bool sequenceComplete = m_diceLanded && (PlayerArchetype::current != Archetype::LUCKY || m_luckyPhase >= 3);

    if (sequenceComplete && !m_lastStealSuccess) {
        m_failureTimer += dt;
        if (m_failureTimer >= 1.0f) CloseUI();
        return;
    }

    if (!m_showDiceResult || sequenceComplete) {
        if (!m_navButtonDown) {
            if (App::IsKeyPressed(VK_RIGHT)) {
                App::PlaySound(".\\TestData\\audio\\confirm.wav", false);
                App::SetSoundVolume(".\\TestData\\audio\\confirm.wav", 0.4f);
                if (m_currentSlot % 3 < 2) m_currentSlot++;
                m_navButtonDown = true;
                m_showDiceResult = false;
                m_lastDiceRoll = 0;
            }
            if (App::IsKeyPressed(VK_LEFT)) {
                App::PlaySound(".\\TestData\\audio\\confirm.wav", false);
                App::SetSoundVolume(".\\TestData\\audio\\confirm.wav", 0.4f);
                if (m_currentSlot % 3 > 0) m_currentSlot--;
                m_navButtonDown = true;
                m_showDiceResult = false;
                m_lastDiceRoll = 0;
            }
            if (App::IsKeyPressed(VK_DOWN)) {
                App::PlaySound(".\\TestData\\audio\\confirm.wav", false);
                App::SetSoundVolume(".\\TestData\\audio\\confirm.wav", 0.4f);
                if (m_currentSlot < 3) m_currentSlot += 3;
                m_navButtonDown = true;
                m_showDiceResult = false;
                m_lastDiceRoll = 0;
            }
            if (App::IsKeyPressed(VK_UP)) {
                App::PlaySound(".\\TestData\\audio\\confirm.wav", false);
                App::SetSoundVolume(".\\TestData\\audio\\confirm.wav", 0.4f);
                if (m_currentSlot >= 3) m_currentSlot -= 3;
                m_navButtonDown = true;
                m_showDiceResult = false;
                m_lastDiceRoll = 0;
            }
        }

        if (App::IsKeyPressed(VK_RETURN) && !m_enterButtonDown) {
            m_enterButtonDown = true;
            if (!activeNPC) return;

            Item* currentTable = activeNPC->GetLootTable();
            int   difficulty   = activeNPC->GetDifficulty();

            if (currentTable[m_currentSlot].id != ITEM_NONE) {
                m_showDiceResult  = false;
                m_showLuckyNotif  = false;
                m_luckyPhase      = 0;
                m_luckyPhaseTimer = 0.0f;
                App::PlaySound(".\\TestData\\audio\\confirm.wav", false);
                App::SetSoundVolume(".\\TestData\\audio\\confirm.wav", 0.4f);

                int raw = (rand() % 20) + 1;
                m_rawDiceRoll  = raw;
                m_lastDiceRoll = raw;

                m_showDiceResult = true;
                m_diceLanded     = false;
                m_diceTimer      = 0.0f;
                m_dice_roll->SetAnimation(0);
                m_pendingSlot = m_currentSlot;

                int finalRoll = (PlayerArchetype::current == Archetype::LUCKY)
                                 ? min(raw + 2, 20) : raw;
                m_lastStealSuccess = (finalRoll >= difficulty);
            }
        }
    }

    if (!App::IsKeyPressed(VK_RIGHT) && !App::IsKeyPressed(VK_LEFT) &&
        !App::IsKeyPressed(VK_DOWN)  && !App::IsKeyPressed(VK_UP))
        m_navButtonDown = false;

    if (!App::IsKeyPressed(VK_RETURN)) m_enterButtonDown = false;

    if (m_showLuckyNotif) {
        m_luckyNotifTimer += dt;
        if (m_luckyNotifTimer >= 1.5f) m_showLuckyNotif = false;
    }

    
    if (m_showDiceResult && !m_diceLanded) {
        m_diceTimer += dt;
        m_dice_roll->Update(deltaTime);
        if (m_diceTimer >= m_diceDuration) {
            m_diceLanded = true;
            m_lastDiceRoll = m_rawDiceRoll;   

            if (PlayerArchetype::current != Archetype::LUCKY) {
                
                if (!activeNPC) return;
                Item* currentTable = activeNPC->GetLootTable();
                int   difficulty   = activeNPC->GetDifficulty();
                if (!m_lastStealSuccess) {
                    activeNPC->SetAlerted(true);
                    App::PlaySound(".\\TestData\\audio\\whistle_blow.wav", false);
                    App::SetSoundVolume(".\\TestData\\audio\\whistle_blow.wav", 0.7f);
                } else {
                    if (m_pendingSlot >= 0 && currentTable[m_pendingSlot].id != ITEM_NONE) {
                        if (currentTable[m_pendingSlot].id == ITEM_GOLD) {
                            m_goldAmount += 10;
                            currentTable[m_pendingSlot] = Item();
                        } else {
                            playerInventory.push_back(std::move(currentTable[m_pendingSlot]));
                        }
                    }
                    App::PlaySound(".\\TestData\\audio\\gold_steal.wav", false);
                    App::SetSoundVolume(".\\TestData\\audio\\gold_steal.wav", 0.4f);
                }
            } else {
                m_luckyPhase      = 1;
                m_luckyPhaseTimer = 0.0f;
            }
        }
    }
    if (m_diceLanded && PlayerArchetype::current == Archetype::LUCKY && m_luckyPhase > 0) {
        m_luckyPhaseTimer += dt;

        if (m_luckyPhase == 1 && m_luckyPhaseTimer >= 0.8f) {
            m_luckyPhase      = 2;
            m_luckyPhaseTimer = 0.0f;
            m_showLuckyNotif  = true;
            m_luckyNotifTimer = 0.0f;
            m_lastDiceRoll    = min(m_rawDiceRoll + 2, 20);
        }
        else if (m_luckyPhase == 2 && m_luckyPhaseTimer >= 1.0f) {
            m_luckyPhase      = 3;
            m_luckyPhaseTimer = 0.0f;
            m_showLuckyNotif  = false;
            if (!activeNPC) return;
            Item* currentTable = activeNPC->GetLootTable();
            if (!m_lastStealSuccess) {
                activeNPC->SetAlerted(true);
                App::PlaySound(".\\TestData\\audio\\whistle_blow.wav", false);
                App::SetSoundVolume(".\\TestData\\audio\\whistle_blow.wav", 0.7f);
            } else {
                if (m_pendingSlot >= 0 && currentTable[m_pendingSlot].id != ITEM_NONE) {
                    if (currentTable[m_pendingSlot].id == ITEM_GOLD) {
                        m_goldAmount += 10;
                        currentTable[m_pendingSlot] = Item();
                    } else {
                        playerInventory.push_back(std::move(currentTable[m_pendingSlot]));
                    }
                }
                App::PlaySound(".\\TestData\\audio\\gold_steal.wav", false);
                App::SetSoundVolume(".\\TestData\\audio\\gold_steal.wav", 0.4f);
            }
        }
    }
}

void UIManager::Render(NPC* activeNPC, std::vector<Item>& playerInventory) {
    if (m_playerInventoryOpen && !inPickpocketUI) {
        m_player_inventory_screen->Draw();
        m_player_inventory_screen_title->Draw();
        GearManager::GetInstance().RenderUI(700.0f, 520.0f);

        int maxSlots = (int)playerInventory.size() < 6 ? (int)playerInventory.size() : 6;
        for (int i = 0; i < maxSlots; i++) {
            DrawItemIcon(playerInventory[i].id, m_playerSlotCoords[i][0], m_playerSlotCoords[i][1]);
        }

        int clampedSlot = m_playerInvSlot;
        if (maxSlots > 0 && clampedSlot >= maxSlots) clampedSlot = maxSlots - 1;

        if (maxSlots > 0) {
            m_ui_cursor->SetPosition(m_playerSlotCoords[clampedSlot][0], m_playerSlotCoords[clampedSlot][1]);
            m_ui_cursor->Draw();
        }
        if (maxSlots > 0 && clampedSlot < (int)playerInventory.size()) {
            const Item& sel = playerInventory[clampedSlot];
            App::PrintTTF(120, 310, sel.name.c_str(),       0.239f, 0.0f, 0.0f, 1);
            App::PrintTTF(120, 280, sel.flavorText.c_str(), 1.0f,   1.0f, 1.0f, 0);
        } else {
            switch (PlayerArchetype::current) {
                case Archetype::CHARISMATIC:
                    App::PrintTTF(120, 310, "Charismatic", 0.239f, 0.0f, 0.0f, 1);
                    App::PrintTTF(120, 280, "Silver tongued and slippery...you find yourself with more slack and more lives.", 1.0f, 1.0f, 1.0f, 0);
                    App::PrintTTF(120, 260, "+2 extra lives", 1.0f, 1.0f, 1.0f, 0);
                    break;
                case Archetype::DEXTEROUS:
                    App::PrintTTF(120, 310, "Dexterous", 0.239f, 0.0f, 0.0f, 1);
                    App::PrintTTF(120, 280, "Being quick on your feet is a plus in this business", 1.0f, 1.0f, 1.0f, 0);
                    App::PrintTTF(120, 260, "You are much faster.", 1.0f, 1.0f, 1.0f, 0);
                    break;
                case Archetype::LUCKY:
                    App::PrintTTF(120, 310, "Lucky", 0.239f, 0.0f, 0.0f, 1);
                    App::PrintTTF(120, 280, "Play with weighted dice to even the odds.", 1.0f, 1.0f, 1.0f, 0);
                    App::PrintTTF(120, 260, "+2 to all dice rolls.", 1.0f, 1.0f, 1.0f, 0);
                    break;
                default: break;
            }
        }

        char goldText[32];
        sprintf(goldText, "%d", m_goldAmount);
        m_icon_gold_small_hud->Draw();
        App::PrintTTF(930, 423, goldText, 1.0f, 1.0f, 1.0f, 0);
    }

    if (!inPickpocketUI) return;
    m_overlay->Draw();
    m_inventory_screen->Draw();

    if (activeNPC) {
        if (activeNPC->GetName() == "Rosamund") {
            m_rosamund_inv_sprite->Draw();
            App::PrintTTF(875, 620, "Rosamund", 1.0f, 1.0f, 1.0f, 1);
            App::PrintTTF(885, 550, "Nurse", 1.0f, 1.0f, 1.0f, 0);
            App::PrintTTF(885, 530, "34 y.o.", 1.0f, 1.0f, 1.0f, 0);
        }
        else if (activeNPC->GetName() == "Randy") {
            m_randy_inv_sprite->Draw();
            App::PrintTTF(875, 620, "Randy", 1.0f, 1.0f, 1.0f, 1);
            App::PrintTTF(885, 550, "Engineer", 1.0f, 1.0f, 1.0f, 0);
            App::PrintTTF(885, 530, "31 y.o.", 1.0f, 1.0f, 1.0f, 0);
        }
        else if (activeNPC->GetName() == "Granny") {
            m_granny_inv_sprite->Draw();
            App::PrintTTF(875, 620, "Granny", 1.0f, 1.0f, 1.0f, 1);
            App::PrintTTF(885, 550, "Cat-Sitter", 1.0f, 1.0f, 1.0f, 0);
            App::PrintTTF(885, 530, "84 y.o.", 1.0f, 1.0f, 1.0f, 0);
        }
        else if (activeNPC->GetName() == "Helene") {
            m_helene_inv_sprite->Draw();
            App::PrintTTF(875, 620, "Helene", 1.0f, 1.0f, 1.0f, 1);
            App::PrintTTF(885, 550, "Diplomat", 1.0f, 1.0f, 1.0f, 0);
            App::PrintTTF(885, 530, "41 y.o.", 1.0f, 1.0f, 1.0f, 0);
        }
        else if (activeNPC->GetName() == "Lupine") {
            m_lupine_inv_sprite->Draw();
            App::PrintTTF(875, 620, "Lupine", 1.0f, 1.0f, 1.0f, 1);
            App::PrintTTF(885, 550, "Museum Curator", 1.0f, 1.0f, 1.0f, 0);
            App::PrintTTF(885, 530, "27 y.o.", 1.0f, 1.0f, 1.0f, 0);
        }
        else if (activeNPC->GetName() == "Charles") {
            m_charles_inv_sprite->Draw();
            App::PrintTTF(875, 620, "Charles", 1.0f, 1.0f, 1.0f, 1);
            App::PrintTTF(885, 550, "Business Man", 1.0f, 1.0f, 1.0f, 0);
            App::PrintTTF(885, 530, "47 y.o.", 1.0f, 1.0f, 1.0f, 0);
        }
        else if (activeNPC->GetName() == "Magda") {
            m_magda_inv_sprite->Draw();
            App::PrintTTF(875, 620, "Magda", 1.0f, 1.0f, 1.0f, 1);
            App::PrintTTF(885, 550, "Student", 1.0f, 1.0f, 1.0f, 0);
            App::PrintTTF(885, 530, "19 y.o.", 1.0f, 1.0f, 1.0f, 0);
        }

        Item* currentTable = activeNPC->GetLootTable();
        for (int i = 0; i < 6; i++) {
            if (currentTable[i].id != ITEM_NONE) {
                DrawItemIcon(currentTable[i].id, m_slotCoords[i][0], m_slotCoords[i][1]);
            }
        }

        m_ui_cursor->SetPosition(m_slotCoords[m_currentSlot][0], m_slotCoords[m_currentSlot][1]);
        m_ui_cursor->Draw();

        Item& selected = currentTable[m_currentSlot];
        if (selected.id != ITEM_NONE) {
            App::PrintTTF(120, 310, selected.name.c_str(),       0.239f, 0.0f, 0.0f, 1);
            App::PrintTTF(120, 280, selected.flavorText.c_str(), 1.0f, 1.0f, 1.0f, 0);

            if (!m_showDiceResult) {
                char req[32];
                sprintf(req, "Need: %d+", activeNPC->GetDifficulty());
                App::PrintTTF(890, 190, req, 1.0f, 1.0f, 1.0f, 1);
            }
        }
    }


    if (m_showDiceResult) {
        m_dice_roll->Draw();

        if (m_showLuckyNotif) {
            float pulse = 0.7f + 0.3f * sinf(m_luckyNotifTimer * 10.0f);
            App::PrintTTF(900, 300, "+2 for luck!", 0.403f, 1.0f * pulse, 0.372f, 1);
        }

        if (m_diceLanded) {
            char res[32];
            sprintf(res, "%d", m_lastDiceRoll);
            App::PrintTTF(927, 275, res, 1.0f, 1.0f, 1.0f, 1);

            bool showOutcome = (PlayerArchetype::current != Archetype::LUCKY)
                                || (m_luckyPhase >= 3);

            if (showOutcome) {
                if (m_lastStealSuccess) {
                    App::PrintTTF(890, 190, "Success!", 0.403f, 1.0f, 0.372f, 1);
                } else {
                    App::PrintTTF(890, 190, "Failure", 0.239f, 0.0f, 0.0f, 1);
                }
            }
        }
    }
}