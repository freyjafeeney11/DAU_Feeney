#include "stdafx.h"
#include "Campsite.h"
#include "GearManager.h"
#include <math.h>
#include <cstdio>


const DailyRequest Rooftop::ms_requests[7] = {
    {
        "You must bring me this item before nightfall..",
        "Converse with the dead but meet silence on one end.",
        { ITEM_LETTER }
    },
    {
        "I'll admit I have a big nose.",
        "Inked pages reveal the tale of a great love.. of pigeons.",
        { ITEM_BOOK }
    },
    {
        "Jucily incriminating...I'm salivating at the thought!",
        "Encrypted. Of course it is.",
        { ITEM_FLASHDRIVE }
    },
    {
        "This one's quite sad.. but I'm allergic.",
        "Jiji never came home, and never liked wearing this.",
        { ITEM_COLLAR }
    },
    {
        "Who knows what they're doing with this one.. I want it!",
        "Preserved out of love or something stranger. Whiskers intact.",
        { ITEM_RAT }
    },
    {
        "For me..? You shouldn't have!",
        "Forgive-Me-Nots! They smell lovely, thorns and all.",
        { ITEM_BOUQUET }
    },
    {
        "I must have this for my collection...",
        "An original Reign Stumpen. Its bearer would sooner let the train crash.",
        { ITEM_PAINTING }
    },
};

const DailyRequest& Rooftop::GetRequest(int index) {
    if (index < 0) index = 0;
    if (index > 6) index = 6;
    return ms_requests[index];
}


Rooftop::Rooftop() {
    m_justSlept = false;
    m_justTraded = false;
    m_showDayMessage = false;

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

    m_dialogue_bg = App::CreateSprite(".\\TestData\\dialogue.png", 1, 1);
    m_dialogue_bg->SetPosition(500.0f, 650.0f);
    m_dialogue_bg->SetScale(0.75f);

    m_request_board = App::CreateSprite(".\\TestData\\ui_screen.png", 1, 1);
    m_request_board->SetPosition(500.0f, 400.0f);
    m_request_board->SetScale(0.6f);

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
    m_ui_cursor->SetScale(0.06f);

    m_icon_letter = App::CreateSprite(".\\TestData\\letter_icon_cropped.png", 1, 1);
    m_icon_flashdrive = App::CreateSprite(".\\TestData\\flashdrive_icon_cropped.png", 1, 1);
    m_icon_picture = App::CreateSprite(".\\TestData\\picture_icon_cropped.png", 1, 1);
    m_icon_rat = App::CreateSprite(".\\TestData\\rat_icon.png", 1, 1);
    m_icon_collar = App::CreateSprite(".\\TestData\\collar_icon.png", 1, 1);
    m_icon_bouquet = App::CreateSprite(".\\TestData\\bouquet_icon.png", 1, 1);
    m_icon_painting = App::CreateSprite(".\\TestData\\painting_icon.png", 1, 1);

    m_campLetter = App::CreateSprite(".\\TestData\\letter_camp.png", 1, 1);
    m_campLetter->SetPosition(512.0f, 400.0f);
    m_campLetter->SetScale(0.6f);

    m_campBook = App::CreateSprite(".\\TestData\\diary_camp.png", 1, 1);
    m_campBook->SetPosition(512.0f, 400.0f);
    m_campBook->SetScale(0.6f);

    m_campDrive = App::CreateSprite(".\\TestData\\drive_camp.png", 1, 1);
    m_campDrive->SetPosition(512.0f, 400.0f);
    m_campDrive->SetScale(0.6f);

    m_campCollar = App::CreateSprite(".\\TestData\\collar_camp.png", 1, 1);
    m_campCollar->SetPosition(512.0f, 400.0f);
    m_campCollar->SetScale(0.6f);

    m_campRat = App::CreateSprite(".\\TestData\\rat_camp.png", 1, 1);
    m_campRat->SetPosition(508.0f, 400.0f);
    m_campRat->SetScale(0.6f);

    m_campBouquet = App::CreateSprite(".\\TestData\\bouquet_camp.png", 1, 1);
    m_campBouquet->SetPosition(512.0f, 400.0f);
    m_campBouquet->SetScale(0.6f);

    m_campPainting = App::CreateSprite(".\\TestData\\painting_camp.png", 1, 1);
    m_campPainting->SetPosition(512.0f, 400.0f);
    m_campPainting->SetScale(0.6f);

    m_watch_sprite = nullptr;
    m_cloudOffset = 0.0f;

    m_playerInTentZone = false;
    m_playerSleeping = false;
    m_enterWasDown = false;

    m_tradeState = TradeState::NONE;
    m_promptChoice = 0;
    m_confirmChoice = 0;
    m_selectedItemIndex = 0;
    m_navKeyDown = false;
    m_tradeEnterDown = false;
    m_completeTimer = 0.0f;

    m_requestBoardOpen = false;
    m_iBoardKeyDown = false;
}

Rooftop::~Rooftop() {
    delete m_background;
    delete m_nightsky;
    delete m_request_board;
    delete m_daysky;
    delete m_dayclouds;
    delete m_nightclouds;
    delete m_tentflaps;
    delete m_goblin;
    delete m_tentsleep;
    delete m_plant;
    delete m_campfire;
    delete m_dialogue_bg;
    delete m_trade_inventory_screen;
    delete m_inventory_title;
    delete m_watch_base;
    delete m_ui_cursor;
    delete m_icon_letter;
    delete m_icon_flashdrive;
    delete m_icon_picture;
    delete m_icon_rat;
    delete m_icon_collar;
    delete m_icon_bouquet;
    delete m_icon_painting;
    delete m_campLetter;
    delete m_campBook;
    delete m_campDrive;
    delete m_campCollar;
    delete m_campRat;
    delete m_campBouquet;
    delete m_campPainting;
    if (m_watch_sprite) delete m_watch_sprite;
}


void Rooftop::NotifyNewDay() {
    m_requestBoardOpen = true;
    m_iBoardKeyDown = true;
    App::PlaySound(".\\TestData\\audio\\menu_open.wav", false);
    App::SetSoundVolume(".\\TestData\\audio\\menu_open.wav", 0.4f);
}

void Rooftop::Reset() {
    m_tradeState = TradeState::NONE;
    m_requestBoardOpen = false;
    m_iBoardKeyDown = false;
    m_selectedItemIndex = 0;
    m_promptChoice = 0;
    m_confirmChoice = 0;
    m_completeTimer = 0.0f;
    m_playerSleeping = false;
    m_justSlept = false;
    m_justTraded = false;
    m_justSatByFire = false;
    m_playerNearFire = false;
    m_showDayMessage = false;
}

void Rooftop::CloseUI() {
    m_requestBoardOpen = false;
    if (m_tradeState != TradeState::NONE && m_tradeState != TradeState::SLEEP_TRANSITION) {
        m_tradeState = TradeState::NONE;
    }
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
    CSimpleSprite* s = nullptr;
    if (itemId == ITEM_LETTER)     s = m_icon_letter;
    else if (itemId == ITEM_FLASHDRIVE) s = m_icon_flashdrive;
    else if (itemId == ITEM_BOOK)       s = m_icon_picture;
    else if (itemId == ITEM_RAT)        s = m_icon_rat;
    else if (itemId == ITEM_PAINTING)   s = m_icon_painting;
    else if (itemId == ITEM_COLLAR)     s = m_icon_collar;
    else if (itemId == ITEM_BOUQUET)    s = m_icon_bouquet;
    if (s) {
        s->SetPosition(x, y);
        s->SetScale(0.6f);
        s->Draw();
    }
}

float Rooftop::GetFadeBrightness() const {
    if (m_tradeState == TradeState::SLEEP_TRANSITION || m_tradeState == TradeState::FIRE_SIT) {
        float b = 1.0f - (m_completeTimer / COMPLETE_DISPLAY_TIME);
        if (b < 0.0f) b = 0.0f;
        return b;
    }
    return 1.0f;
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

bool Rooftop::JustSatByFire() {
    bool v = m_justSatByFire;
    m_justSatByFire = false;
    return v;
}

bool Rooftop::IsPlayerNearHatch(float px) const {
    return fabsf(px - HATCH_X) < HATCH_RADIUS;
}


void Rooftop::Update(float deltaTime, float playerX, std::vector<Item>& playerInventory, bool isDay) {
    float dt = deltaTime / 1000.0f;

    m_cloudOffset += CLOUD_SPEED * dt;
    if (m_cloudOffset > CLOUD_WIDTH) m_cloudOffset -= CLOUD_WIDTH;

    m_playerInTentZone = (playerX > TENT_TRIGGER_X);
    m_campfire->Update(deltaTime);

    bool iKeyDown = App::IsKeyPressed('I');
    if (iKeyDown && !m_iBoardKeyDown && m_tradeState == TradeState::NONE) {
        m_requestBoardOpen = !m_requestBoardOpen;
        m_iBoardKeyDown = true;
    }
    if (!iKeyDown) m_iBoardKeyDown = false;

    if (App::IsKeyPressed(VK_ESCAPE) && m_requestBoardOpen) {
        m_requestBoardOpen = false;
    }

    if (m_tradeState == TradeState::FIRE_SIT) {
        m_completeTimer += dt;
        if (m_completeTimer >= FIRE_SIT_DURATION) {
            m_tradeState = TradeState::NONE;
            m_completeTimer = 0.0f;
            m_justSatByFire = true;
        }
        return;
    }

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
            m_playerSleeping = true;
        }
        return;
    }

    if (m_tradeState == TradeState::FAILED) {
        m_completeTimer += dt;
        if (m_completeTimer >= COMPLETE_DISPLAY_TIME) {
            m_tradeState = TradeState::PROMPT;
            m_promptChoice = 0;
            m_completeTimer = 0.0f;
        }
        return;
    }

    m_playerNearFire = isDay && fabsf(playerX - FIRE_X) < FIRE_TRIGGER_RADIUS
                       && m_tradeState == TradeState::NONE;

    bool enterDown = App::IsKeyPressed(VK_RETURN);
    bool backDown  = App::IsKeyPressed(VK_BACK);

    if (m_playerNearFire && enterDown && !m_enterWasDown) {
        m_tradeState   = TradeState::FIRE_SIT;
        m_completeTimer = 0.0f;
        m_enterWasDown = true;
        return;
    }

    if (m_tradeState == TradeState::NONE) {
        if (!m_playerInTentZone) m_showDayMessage = false;

        if (m_playerInTentZone && enterDown && !m_enterWasDown) {
            if (isDay) {
                m_showDayMessage = true;
            }
            else {
                m_tradeState = TradeState::PROMPT;
                App::PlaySound(".\\TestData\\audio\\menu_open.wav", false);
                App::SetSoundVolume(".\\TestData\\audio\\menu_open.wav", 0.4f);
                m_promptChoice = 0;
                m_navKeyDown = false;
                m_tradeEnterDown = true;
                m_showDayMessage = false;
                m_requestBoardOpen = false;
            }
        }
    }
    else if (m_tradeState == TradeState::PROMPT) {
        if (backDown) {
            m_tradeState = TradeState::NONE;
            m_promptChoice = 0;
            return;
        }
        if (!m_navKeyDown) {
            if (App::IsKeyPressed(VK_LEFT) || App::IsKeyPressed(VK_RIGHT)) {
                m_promptChoice = 1 - m_promptChoice;
                m_navKeyDown = true;
            }
        }
        if (!App::IsKeyPressed(VK_LEFT) && !App::IsKeyPressed(VK_RIGHT)) m_navKeyDown = false;

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
            int maxIndex = (int)tradeable.size() - 1;

            if (!m_navKeyDown) {
                if (App::IsKeyPressed(VK_LEFT)) {
                    App::PlaySound(".\\TestData\\audio\\confirm.wav", false);
                    App::SetSoundVolume(".\\TestData\\audio\\confirm.wav", 0.4f);
                    if (m_selectedItemIndex > 0) m_selectedItemIndex--;
                    m_navKeyDown = true;
                }
                if (App::IsKeyPressed(VK_RIGHT)) {
                    App::PlaySound(".\\TestData\\audio\\confirm.wav", false);
                    App::SetSoundVolume(".\\TestData\\audio\\confirm.wav", 0.4f);
                    if (m_selectedItemIndex < maxIndex) m_selectedItemIndex++;
                    m_navKeyDown = true;
                }
                if (App::IsKeyPressed(VK_UP)) {
                    App::PlaySound(".\\TestData\\audio\\confirm.wav", false);
                    App::SetSoundVolume(".\\TestData\\audio\\confirm.wav", 0.4f);
                    if (m_selectedItemIndex - 3 >= 0) m_selectedItemIndex -= 3;
                    m_navKeyDown = true;
                }
                if (App::IsKeyPressed(VK_DOWN)) {
                    App::PlaySound(".\\TestData\\audio\\confirm.wav", false);
                    App::SetSoundVolume(".\\TestData\\audio\\confirm.wav", 0.4f);
                    if (m_selectedItemIndex + 3 <= maxIndex) m_selectedItemIndex += 3;
                    m_navKeyDown = true;
                }
            }
            if (!App::IsKeyPressed(VK_UP) && !App::IsKeyPressed(VK_DOWN) &&
                !App::IsKeyPressed(VK_LEFT) && !App::IsKeyPressed(VK_RIGHT))
                m_navKeyDown = false;

            if (maxIndex < 0) m_selectedItemIndex = 0;
            else if (m_selectedItemIndex > maxIndex) m_selectedItemIndex = maxIndex;

            if (enterDown && !m_tradeEnterDown) {
                m_tradeEnterDown = true;
                if (!tradeable.empty() && m_selectedItemIndex <= maxIndex) {
                    App::PlaySound(".\\TestData\\audio\\confirm.wav", false);
                    App::SetSoundVolume(".\\TestData\\audio\\confirm.wav", 0.4f);
                    m_tradeState = TradeState::CONFIRMING;
                    m_confirmChoice = 0;
                }
            }
        }
    }
    else if (m_tradeState == TradeState::CONFIRMING) {
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
            }
            if (!App::IsKeyPressed(VK_LEFT) && !App::IsKeyPressed(VK_RIGHT)) m_navKeyDown = false;

            if (enterDown && !m_tradeEnterDown) {
                m_tradeEnterDown = true;
                if (m_confirmChoice == 0) {
                    std::vector<int> tradeable = GetTradeableIndices(playerInventory);
                    if (!tradeable.empty() && m_selectedItemIndex < (int)tradeable.size()) {
                        int gearCount = GearManager::GetInstance().GetGearCount();
                        if (gearCount < 7) {
                            const DailyRequest& req = ms_requests[gearCount];
                            int selectedInvIdx = tradeable[m_selectedItemIndex];
                            
                            if (playerInventory[selectedInvIdx].id == req.requiredItems[0]) {
                                playerInventory.erase(playerInventory.begin() + selectedInvIdx);
                                GearManager::GetInstance().AddGear();
                                m_justTraded = true;
                                m_tradeState = TradeState::COMPLETE;
                                m_completeTimer = 0.0f;
                                m_selectedItemIndex = 0;
                            }
                            else {
                                m_tradeState = TradeState::FAILED;
                                m_completeTimer = 0.0f;
                            }
                        }
                    }
                }
                else {
                    m_tradeState = TradeState::SELECTING;
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

    int gear = GearManager::GetInstance().GetGearCount();
    if (gear >= 1) { m_campLetter->SetColor(b, b, b); m_campLetter->Draw(); }
    if (gear >= 2) { m_campBook->SetColor(b, b, b); m_campBook->Draw(); }
    if (gear >= 3) { m_campDrive->SetColor(b, b, b); m_campDrive->Draw(); }
    if (gear >= 5) { m_campRat->SetColor(b, b, b); m_campRat->Draw(); }
    if (gear >= 7) { m_campPainting->SetColor(b, b, b); m_campPainting->Draw(); }

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
    if (!isDay) {
        m_goblin->SetColor(b, b, b);
        m_goblin->Draw();
        if (GearManager::GetInstance().GetGearCount() >= 4) {
            m_campCollar->SetColor(b, b, b);
            m_campCollar->Draw();
        }
    }
    if (m_requestBoardOpen && m_tradeState == TradeState::NONE) {
    }
}

void Rooftop::RenderRequestBoard() const {
    int gearCount = GearManager::GetInstance().GetGearCount();
    if (gearCount >= 8) {
        App::PrintTTF(200, 500, "The watch is complete. All is well.", 0.239f, 0.0f, 0.0f, 1);
        return;
    }

    const DailyRequest& req = ms_requests[gearCount];

    m_request_board->Draw();

    App::PrintTTF(200, 500, "The goblin speaks...", 0.239f, 0.0f, 0.0f, 1);
    App::PrintTTF(200, 470, req.goblinLine.c_str(), 1.0f, 1.0f, 1.0f, 0);

    App::PrintTTF(200, 440, "Today's riddle:", 0.239f, 0.0f, 0.0f, 1);
    App::PrintTTF(200, 410, req.riddle.c_str(), 1.0f, 1.0f, 1.0f, 0);
}

void Rooftop::RenderTradeUI(const std::vector<Item>& playerInventory) {
    if (m_tradeState == TradeState::NONE) {
        if (m_playerNearFire) {
            App::PrintTTF(10, 60, "Sit by the fire until nightfall?", 1.0f, 1.0f, 1.0f, 0);
            App::PrintTTF(10, 45, "Press Enter", 1.0f, 1.0f, 1.0f, 0);
        }
        if (m_requestBoardOpen) RenderRequestBoard();
        return;
    }
    if (m_tradeState == TradeState::SLEEP_TRANSITION || m_tradeState == TradeState::FIRE_SIT) return;

    if (m_tradeState == TradeState::PROMPT ||
        m_tradeState == TradeState::COMPLETE ||
        m_tradeState == TradeState::FAILED) {
        m_dialogue_bg->Draw();
    }
    else {
        m_trade_inventory_screen->Draw();
        m_inventory_title->Draw();
        GearManager::GetInstance().RenderUI(700.0f, 520.0f);
    }

    if (m_tradeState == TradeState::COMPLETE) {
        App::PrintTTF(150, 340, "let me look at this... Yes!!", 0.239f, 0.0f, 0.0f, 1);
        App::PrintTTF(150, 305, "A deal is struck!", 1.0f, 1.0f, 1.0f, 0);
        return;
    }

    if (m_tradeState == TradeState::FAILED) {
        App::PrintTTF(150, 340, "What is this.. it is not what I requested.", 0.239f, 0.0f, 0.0f, 1);
        App::PrintTTF(150, 305, "The goblin shakes his head in disappointment.", 1.0f, 1.0f, 1.0f, 0);
        return;
    }

    if (m_tradeState == TradeState::PROMPT) {
        App::PrintTTF(135, 325, "The goblin eyes your pockets...", 0.239f, 0.0f, 0.0f, 1);
        App::PrintTTF(135, 305, "Would you like to make a trade?", 1.0f, 1.0f, 1.0f, 0);
        
        App::PrintTTF(235, 275, "TRADE", 
            m_promptChoice == 0 ? 0.403f : 1.0f,
            m_promptChoice == 0 ? 1.0f   : 1.0f,
            m_promptChoice == 0 ? 0.372f : 1.0f, 0);
            
        App::PrintTTF(385, 275, "SLEEP", 
            m_promptChoice == 1 ? 0.403f : 1.0f,
            m_promptChoice == 1 ? 1.0f   : 1.0f,
            m_promptChoice == 1 ? 0.372f : 1.0f, 0);
        return;
    }

    std::vector<int> tradeable = GetTradeableIndices(playerInventory);

    const float SLOT_X[6] = { 180.0f, 310.0f, 450.0f, 180.0f, 310.0f, 450.0f };
    const float SLOT_Y[6] = { 600.0f, 600.0f, 600.0f, 450.0f, 450.0f, 450.0f };

    for (int i = 0; i < (int)tradeable.size() && i < 6; i++) {
        DrawItemIcon(playerInventory[tradeable[i]].id, SLOT_X[i], SLOT_Y[i]);

    }

    if (tradeable.empty()) {
        App::PrintTTF(150, 310, "You have nothing to trade.", 1.0f, 1.0f, 1.0f, 0);
        return;
    }

    
    if (m_tradeState == TradeState::SELECTING) {
        if (m_selectedItemIndex < (int)tradeable.size()) {
            m_ui_cursor->SetPosition(SLOT_X[m_selectedItemIndex], SLOT_Y[m_selectedItemIndex]);
            m_ui_cursor->Draw();
            const Item& item = playerInventory[tradeable[m_selectedItemIndex]];
            App::PrintTTF(120, 310, item.name.c_str(), 0.239f, 0.0f, 0.0f, 0);
            App::PrintTTF(120, 280, item.flavorText.c_str(), 1.0f, 1.0f, 1.0f, 0);
        }
    }

    if (m_tradeState == TradeState::CONFIRMING) {
        App::PrintTTF(120, 320, "Give this to the goblin?", 1.0f, 1.0f, 1.0f, 1);

        std::vector<int> tradeable = GetTradeableIndices(playerInventory);
        if (!tradeable.empty() && m_selectedItemIndex < (int)tradeable.size()) {
            int si = tradeable[m_selectedItemIndex];
            DrawItemIcon(playerInventory[si].id, 160, 280.0f);
            App::PrintTTF(120, 210.0f, playerInventory[si].name.c_str(), 0.239f, 0.0f, 0.0f, 0);
        }

        App::PrintTTF(300, 200, "YES", 
            m_confirmChoice == 0 ? 1.0f   : 1.0f,
            m_confirmChoice == 0 ? 1.0f   : 1.0f,
            m_confirmChoice == 0 ? 0.0f   : 1.0f,
            0);
                    
        App::PrintTTF(400, 200, "NO", 
            m_confirmChoice == 1 ? 1.0f   : 1.0f,
            m_confirmChoice == 1 ? 1.0f   : 1.0f,
            m_confirmChoice == 1 ? 0.0f   : 1.0f,
            0);
    }
}

void Rooftop::RenderPlant() {
    float b = GetFadeBrightness();
    m_plant->SetColor(b, b, b);
    m_plant->SetPosition(512.0f, 400.0f);
    m_plant->Draw();
    
    if (GearManager::GetInstance().GetGearCount() >= 6) {
        m_campBouquet->SetColor(b, b, b);
        m_campBouquet->Draw();
    }
}

void Rooftop::RenderOverlayText(bool nearHatch, bool isDay) {
    if (m_tradeState != TradeState::NONE) return;

    if (m_playerInTentZone) {
        if (isDay) {
            App::PrintTTF(750, 170, "The goblin is out...", 0.8f, 0.8f, 0.8f, 0);
            if (m_showDayMessage)
                App::PrintTTF(750, 145, "You can only trade at night.", 0.239f, 0.0f, 0.0f, 0);
        } else {
            App::PrintTTF(750, 170, "Press Enter to trade or sleep", 1.0f, 1.0f, 1.0f, 0);
        }
    }

    if (!m_playerNearFire && !nearHatch) {
        App::PrintTTF(10, 70, "Press I to check today's request", 1.0f, 1.0f, 1.0f, 0);
    }
}