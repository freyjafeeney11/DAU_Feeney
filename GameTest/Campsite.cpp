#include "stdafx.h"
#include "Campsite.h"
#include <math.h>
#include <cstdio>

Rooftop::Rooftop() {
    m_justSlept = false;
    m_nightsky = App::CreateSprite(".\\TestData\\night_sky.png", 1, 1);
    m_nightsky->SetPosition(512.0f, 400.0f);
    m_nightsky->SetScale(0.6f);

    m_daysky = App::CreateSprite(".\\TestData\\day_background.png", 1, 1);
    m_daysky->SetPosition(512.0f, 420.0f);
    m_daysky->SetScale(0.6f);

    m_dayclouds = App::CreateSprite(".\\TestData\\day_clouds.png", 1, 1);
    m_dayclouds->SetScale(0.6f);

    m_nightclouds = App::CreateSprite(".\\TestData\\clouds.png", 1, 1);
    m_nightclouds->SetScale(0.6f);

    m_background = App::CreateSprite(".\\TestData\\train_bg.png", 1, 1);
    m_background->SetPosition(512.0f, 400.0f);
    m_background->SetScale(0.6f);

    m_tentflaps = App::CreateSprite(".\\TestData\\tent_closed.png", 1, 1);
    m_tentflaps->SetPosition(512.0f, 400.0f);
    m_tentflaps->SetScale(0.6f);

    m_tentsleep = App::CreateSprite(".\\TestData\\tent_sleep.png", 1, 1);
    m_tentsleep->SetPosition(512.0f, 400.0f);
    m_tentsleep->SetScale(0.6f);

    m_plant = App::CreateSprite(".\\TestData\\plant.png", 1, 1);
    m_plant->SetPosition(512.0f, 400.0f);
    m_plant->SetScale(0.6f);

    m_goblin = App::CreateSprite(".\\TestData\\goblin.png", 1, 1);
    m_goblin->SetPosition(512.0f, 400.0f);
    m_goblin->SetScale(0.6f);

    m_campfire = App::CreateSprite(".\\TestData\\fire_anim.png", 5, 1);
    m_campfire->SetPosition(585.0f, 324.0f);
    m_campfire->SetScale(0.77f);
    m_campfire->CreateAnimation(0, 0.1f, { 0, 1, 2, 3, 4 });
    m_campfire->SetAnimation(0);

    m_trade_inventory_screen = App::CreateSprite(".\\TestData\\Inventory.png", 1, 1);
    m_trade_inventory_screen->SetPosition(500.0f, 400.0f);
    m_trade_inventory_screen->SetScale(0.6f);

    m_inventory_title = App::CreateSprite(".\\TestData\\inventory_title.png", 1, 1);
    m_inventory_title->SetPosition(500.0f, 400.0f);
    m_inventory_title->SetScale(0.6f);

    m_watch_base = App::CreateSprite(".\\TestData\\watch.png", 1, 1);
    m_watch_base->SetPosition(700.0f, 520.0f);
    m_watch_base->SetScale(0.12f);

    m_ui_cursor = App::CreateSprite(".\\TestData\\mask_temp.png", 1, 1);
    m_ui_cursor->SetScale(0.4f);

    m_icon_letter = App::CreateSprite(".\\TestData\\letter_icon_cropped.png", 1, 1);
    m_icon_flashdrive = App::CreateSprite(".\\TestData\\flashdrive_icon_cropped.png", 1, 1);
    m_icon_picture = App::CreateSprite(".\\TestData\\picture_icon_cropped.png", 1, 1);

    m_watch_sprite = nullptr;

    m_cloudOffset = 0.0f;
    m_playerInTentZone = false;
    m_playerSleeping = false;
    m_enterWasDown = false;

    m_tradeState = TradeState::NONE;
    m_promptChoice = 0;
    m_confirmChoice = 0;
    m_selectedItemIndex = 0;
    m_gearCount = 0;
    m_justTraded = false;
    m_navKeyDown = false;
    m_tradeEnterDown = false;
    m_completeTimer = 0.0f;
}

Rooftop::~Rooftop() {
    delete m_background;
    delete m_nightsky;
    delete m_daysky;
    delete m_dayclouds;
    delete m_nightclouds;
    delete m_tentflaps;
    delete m_goblin;
    delete m_tentsleep;
    delete m_plant;
    delete m_campfire;
    delete m_trade_inventory_screen;
    delete m_inventory_title;
    delete m_watch_base;
    delete m_ui_cursor;
    delete m_icon_letter;
    delete m_icon_flashdrive;
    delete m_icon_picture;
    if (m_watch_sprite) delete m_watch_sprite;
}

std::vector<int> Rooftop::GetTradeableIndices(const std::vector<Item>& playerInventory) const {
    std::vector<int> result;
    for (int i = 0; i < (int)playerInventory.size(); i++) {
        int id = playerInventory[i].id;
        if (id != ITEM_NONE && id != ITEM_GOLD) {
            result.push_back(i);
        }
    }
    return result;
}

void Rooftop::DrawItemIcon(int itemId, float x, float y) {
    CSimpleSprite* spriteToDraw = nullptr;
    if (itemId == ITEM_LETTER) spriteToDraw = m_icon_letter;
    else if (itemId == ITEM_FLASHDRIVE) spriteToDraw = m_icon_flashdrive;
    else if (itemId == ITEM_PICTURE) spriteToDraw = m_icon_picture;

    if (spriteToDraw) {
        spriteToDraw->SetPosition(x, y);
        spriteToDraw->SetScale(0.6f);
        spriteToDraw->Draw();
    }
}

float Rooftop::GetFadeBrightness() const {
    if (m_tradeState == TradeState::SLEEP_TRANSITION) {
        float b = 1.0f - (m_completeTimer / COMPLETE_DISPLAY_TIME);
        if (b < 0.0f) b = 0.0f;
        return b;
    }
    return 1.0f;
}

void Rooftop::Update(float deltaTime, float playerX, std::vector<Item>& playerInventory) {
    float dt = deltaTime / 1000.0f;

    m_cloudOffset += CLOUD_SPEED * dt;
    if (m_cloudOffset > CLOUD_WIDTH) m_cloudOffset -= CLOUD_WIDTH;

    m_playerInTentZone = (playerX > TENT_TRIGGER_X);
    m_campfire->Update(deltaTime);

    if (m_tradeState == TradeState::SLEEP_TRANSITION) {
        m_completeTimer += dt;
        m_playerSleeping = true;
        if (m_completeTimer >= COMPLETE_DISPLAY_TIME) {
            m_tradeState = TradeState::NONE;
            m_completeTimer = 0.0f;
            m_playerSleeping = false;
            m_justSlept = true;
        }
        return;
    }

    if (m_tradeState == TradeState::COMPLETE) {
        m_completeTimer += dt;
        if (m_completeTimer >= COMPLETE_DISPLAY_TIME) {
            m_tradeState = TradeState::SLEEP_TRANSITION;
            m_completeTimer = 0.0f;
        }
        return;
    }

    bool enterDown = App::IsKeyPressed(VK_RETURN);
    bool backDown = App::IsKeyPressed(VK_BACK);

    if (m_tradeState == TradeState::NONE) {
        if (m_playerInTentZone && enterDown && !m_enterWasDown) {
            m_tradeState = TradeState::PROMPT;
            m_promptChoice = 0;
            m_navKeyDown = false;
            m_tradeEnterDown = true;
        }
    }
    else if (m_tradeState == TradeState::PROMPT) {
        if (!m_navKeyDown) {
            if (App::IsKeyPressed(VK_LEFT) || App::IsKeyPressed(VK_RIGHT)) {
                m_promptChoice = 1 - m_promptChoice;
                m_navKeyDown = true;
            }
        }
        if (!App::IsKeyPressed(VK_LEFT) && !App::IsKeyPressed(VK_RIGHT)) {
            m_navKeyDown = false;
        }

        if (enterDown && !m_tradeEnterDown) {
            m_tradeEnterDown = true;
            if (m_promptChoice == 0) {
                m_tradeState = TradeState::SELECTING;
                m_selectedItemIndex = 0;
            }
            else {
                m_tradeState = TradeState::SLEEP_TRANSITION;
                m_completeTimer = 0.0f;
                m_playerSleeping = true;
            }
        }
    }
    else if (m_tradeState == TradeState::SELECTING) {
        if (backDown) {
            m_tradeState = TradeState::NONE;
        }
        else {
            std::vector<int> tradeable = GetTradeableIndices(playerInventory);
            if (!m_navKeyDown) {
                if (App::IsKeyPressed(VK_LEFT)) {
                    if (m_selectedItemIndex > 0) m_selectedItemIndex--;
                    m_navKeyDown = true;
                }
                if (App::IsKeyPressed(VK_RIGHT)) {
                    if (m_selectedItemIndex < (int)tradeable.size() - 1) m_selectedItemIndex++;
                    m_navKeyDown = true;
                }
                if (App::IsKeyPressed(VK_UP)) {
                    if (m_selectedItemIndex - 3 >= 0) m_selectedItemIndex -= 3;
                    m_navKeyDown = true;
                }
                if (App::IsKeyPressed(VK_DOWN)) {
                    if (m_selectedItemIndex + 3 < (int)tradeable.size()) m_selectedItemIndex += 3;
                    m_navKeyDown = true;
                }
            }
            if (!App::IsKeyPressed(VK_UP) && !App::IsKeyPressed(VK_DOWN) && !App::IsKeyPressed(VK_LEFT) && !App::IsKeyPressed(VK_RIGHT)) {
                m_navKeyDown = false;
            }

            if (enterDown && !m_tradeEnterDown) {
                m_tradeEnterDown = true;
                if (!tradeable.empty()) {
                    m_tradeState = TradeState::CONFIRMING;
                    m_confirmChoice = 0;
                }
            }
        }
    }
    else if (m_tradeState == TradeState::CONFIRMING) {
        std::vector<int> tradeable = GetTradeableIndices(playerInventory);
        if (backDown) {
            m_tradeState = TradeState::SELECTING;
        }
        else {
            if (!m_navKeyDown) {
                if (App::IsKeyPressed(VK_LEFT) || App::IsKeyPressed(VK_RIGHT)) {
                    m_confirmChoice = 1 - m_confirmChoice;
                    m_navKeyDown = true;
                }
            }
            if (!App::IsKeyPressed(VK_LEFT) && !App::IsKeyPressed(VK_RIGHT)) {
                m_navKeyDown = false;
            }

            if (enterDown && !m_tradeEnterDown) {
                m_tradeEnterDown = true;
                if (m_confirmChoice == 0 && !tradeable.empty()) {
                    int invIndex = tradeable[m_selectedItemIndex];
                    playerInventory.erase(playerInventory.begin() + invIndex);
                    m_gearCount++;
                    m_justTraded = true;

                    if (m_watch_sprite) delete m_watch_sprite;
                    char spritePath[64];
                    sprintf(spritePath, ".\\TestData\\watch_phase (%d).png", m_gearCount);
                    m_watch_sprite = App::CreateSprite(spritePath, 1, 1);

                    m_watch_sprite->SetPosition(700.0f, 520.0f);
                    m_watch_sprite->SetScale(0.12f);

                    m_tradeState = TradeState::COMPLETE;
                    m_completeTimer = 0.0f;
                    m_selectedItemIndex = 0;
                }
                else {
                    m_tradeState = TradeState::SELECTING;
                }
            }
        }
    }

    if (!enterDown) m_tradeEnterDown = false;
    m_enterWasDown = enterDown;

    if (!m_playerInTentZone && m_tradeState == TradeState::NONE) {
        m_playerSleeping = false;
        m_enterWasDown = false;
    }
}

void Rooftop::Render(bool isDay) {
    float b = GetFadeBrightness();

    if (isDay) {
        m_daysky->SetColor(b, b, b);
        m_daysky->SetPosition(512.0f, 400.0f);
        m_daysky->Draw();

        m_dayclouds->SetColor(b, b, b);
        m_dayclouds->SetPosition(512.0f - m_cloudOffset, 400.0f);
        m_dayclouds->Draw();
        m_dayclouds->SetPosition(512.0f - m_cloudOffset + CLOUD_WIDTH, 400.0f);
        m_dayclouds->Draw();
    }
    else {
        m_nightsky->SetColor(b, b, b);
        m_nightsky->SetPosition(512.0f, 400.0f);
        m_nightsky->Draw();

        m_nightclouds->SetColor(b, b, b);
        m_nightclouds->SetPosition(512.0f - m_cloudOffset, 400.0f);
        m_nightclouds->Draw();
        m_nightclouds->SetPosition(512.0f - m_cloudOffset + CLOUD_WIDTH, 400.0f);
        m_nightclouds->Draw();
    }

    m_background->SetColor(b, b, b);
    m_background->SetPosition(512.0f, 400.0f);
    m_background->Draw();

    m_campfire->SetColor(b, b, b);
    m_campfire->Draw();

    if (!m_playerInTentZone) {
        m_tentflaps->SetColor(b, b, b);
        m_tentflaps->SetPosition(512.0f, 400.0f);
        m_tentflaps->Draw();
    }
    if (m_playerSleeping) {
        m_tentsleep->SetColor(b, b, b);
        m_tentsleep->SetPosition(512.0f, 400.0f);
        m_tentsleep->Draw();
    }

    m_goblin->SetColor(b, b, b);
    m_goblin->Draw();

    if (m_playerInTentZone && m_tradeState == TradeState::NONE) {
        App::Print(750, 150, "Press Enter to trade or sleep", 1.0f, 1.0f, 1.0f);
    }
}

void Rooftop::RenderTradeUI(const std::vector<Item>& playerInventory) {
    if (m_tradeState == TradeState::NONE || m_tradeState == TradeState::SLEEP_TRANSITION) return;

    m_trade_inventory_screen->Draw();
    m_inventory_title->Draw();
    m_watch_base->Draw();

    if (m_watch_sprite) {
        m_watch_sprite->SetPosition(700.0f, 520.0f);
        m_watch_sprite->SetScale(0.12f);
        m_watch_sprite->Draw();
    }

    if (m_tradeState == TradeState::COMPLETE) {
        App::Print(150, 280, "A deal is struck! Sleep well...", 1.0f, 0.9f, 0.4f);
        return;
    }

    if (m_tradeState == TradeState::PROMPT) {
        App::Print(150, 310, "The goblin eyes your pockets...", 1.0f, 1.0f, 0.0f);
        App::Print(150, 280, "Care to make a deal, or head to sleep?", 1.0f, 1.0f, 1.0f);

        float tradeR = (m_promptChoice == 0) ? 1.0f : 0.4f;
        float sleepR = (m_promptChoice == 1) ? 1.0f : 0.4f;
        App::Print(250, 220, "TRADE", tradeR, (m_promptChoice == 0) ? 0.9f : 0.4f, 0.0f);
        App::Print(400, 220, "SLEEP", sleepR, (m_promptChoice == 1) ? 0.9f : 0.4f, 0.0f);
        return;
    }

    std::vector<int> tradeable = GetTradeableIndices(playerInventory);

    const float SLOT_X[6] = { 190.0f, 310.0f, 450.0f, 190.0f, 310.0f, 450.0f };
    const float SLOT_Y[6] = { 600.0f, 600.0f, 600.0f, 450.0f, 450.0f, 450.0f };

    if (m_tradeState == TradeState::SELECTING || m_tradeState == TradeState::CONFIRMING) {
        for (int i = 0; i < (int)tradeable.size() && i < 6; i++) {
            DrawItemIcon(playerInventory[tradeable[i]].id, SLOT_X[i], SLOT_Y[i]);
        }

        if (tradeable.empty()) {
            App::Print(150, 310, "You have nothing to trade.", 0.5f, 0.5f, 0.5f);
            return;
        }

        m_ui_cursor->SetPosition(SLOT_X[m_selectedItemIndex], SLOT_Y[m_selectedItemIndex]);
        m_ui_cursor->Draw();

        const Item& item = playerInventory[tradeable[m_selectedItemIndex]];
        App::Print(120, 310, item.name.c_str(), 1.0f, 1.0f, 0.0f);
        App::Print(120, 280, item.flavorText.c_str(), 1.0f, 1.0f, 1.0f);

        if (m_tradeState == TradeState::CONFIRMING) {
            char line[128];
            sprintf(line, "Give \"%s\" to the goblin?", item.name.c_str());
            App::Print(120, 220, line, 1.0f, 0.5f, 0.0f);

            float yesR = (m_confirmChoice == 0) ? 1.0f : 0.4f;
            float noR = (m_confirmChoice == 1) ? 1.0f : 0.4f;
            App::Print(300, 180, "YES", yesR, (m_confirmChoice == 0) ? 0.9f : 0.4f, 0.0f);
            App::Print(400, 180, "NO", noR, 0.4f, 0.4f);
        }
    }
}

void Rooftop::RenderPlant() {
    float b = GetFadeBrightness();
    m_plant->SetColor(b, b, b);
    m_plant->SetPosition(512.0f, 400.0f);
    m_plant->Draw();
}

bool Rooftop::JustSlept() {
    bool v = m_justSlept;
    m_justSlept = false;
    return v;
}

bool Rooftop::JustTraded() {
    bool v = m_justTraded;
    m_justTraded = false;
    return v;
}

bool Rooftop::IsPlayerNearHatch(float px) const {
    return fabsf(px - HATCH_X) < HATCH_RADIUS;
}