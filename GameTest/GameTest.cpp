#include "stdafx.h"
#include <windows.h> 
#include <math.h>  
#include "app\app.h"
#include <string>
#include <ctime>
#include <vector>
#include "CrowdManager.h"
#include "Player.h"
#include "NPC.h"
#include "UIManager.h"
#include "Level.h"
#include "Campsite.h"
#include "Patroller.h"
#include "MainMenu.h"
#include "Intro.h"
#include "App\SimpleSound.h"
#include "GameClock.h"
#include "PauseMenu.h"
#include "CaughtMenu.h"

#define SKIP_INTRO true

// things to fix immediate
// fix music and adding other sounds, ex gold sound effect
// not deleting saved stuff between "Retry"
// ladder draws ontop of clock ui
// hover above items with or show the difficulty of the roll needed
// idea .. at the end scene, the goblin takes all of the items you traded and swirls them around in an animation and tells a story ? 
// outro

enum class SceneState {
    MAIN_MENU,
    INTRO,
    TRAIN_INTERIOR,
    ROOFTOP
};
// font stuff
bool g_fontsLoaded = false;
// pause menu stuff
bool g_isPaused = false;
bool g_escWasDown = false;
PauseMenu* myPauseMenu;

// end menu
CaughtMenu* myCaughtMenu;
float g_caughtAnimTimer = 0.0f;
static constexpr float CAUGHT_ANIM_DURATION = 0.6f;

SceneState g_scene = SKIP_INTRO ? SceneState::TRAIN_INTERIOR : SceneState::MAIN_MENU;
bool g_nearLadder = false;
bool g_nearHatch = false;

struct Camera {
    float x = 0.0f;
    float y = 0.0f;
    float targetX = 0.0f;
    float width = 1024.0f;
} g_camera;

GameClock* g_clock;
Patroller* myPatroller;
CrowdManager* myCrowdManager;
Player* myPlayer;
UIManager* myUI;
Level* myLevel;
Rooftop* myRooftop;
MainMenu* myMainMenu;
Intro* myIntro;

CSimpleSprite* alertIcon;

std::vector<Item> playerInventory;
std::vector<NPC*> allNPCs;
NPC* activeNPC = nullptr;

int rosamundLoot[6] = { ITEM_GOLD, ITEM_LETTER,    ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE };
int grannyLoot[6] = { ITEM_GOLD, ITEM_BOOK,   ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE };
int randyLoot[6] = { ITEM_GOLD, ITEM_FLASHDRIVE, ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE };

int heleneLoot[6] = { ITEM_GOLD, ITEM_COLLAR, ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE };
int magdaLoot[6] = { ITEM_GOLD, ITEM_RAT, ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE };
int lupineLoot[6] = { ITEM_GOLD, ITEM_PAINTING, ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE };
int charlesLoot[6] = { ITEM_GOLD, ITEM_BOUQUET, ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE };

NPC* rosamund;
NPC* granny;
NPC* randy;

void DrawAlertIconAboveNPC(NPC* npc) {
    if (!npc || !alertIcon) return;
    float x, y;
    npc->GetPosition(x, y);
    float worldHeight = npc->GetHeight() * npc->GetScale();

    static float t = 0.0f;
    t += 0.05f;
    float bob = sinf(t) * 4.0f;

    alertIcon->SetPosition(x - g_camera.x, y + (worldHeight * 0.5f) + 10.0f + bob);
    alertIcon->Draw();
}

bool IsPlayerNearNPC() {
    float px, py, nx, ny;
    myPlayer->GetPosition(px, py);
    activeNPC = nullptr;
    const float playerRadius = 20.0f;
    const float npcRadius = 100.0f;
    for (NPC* npc : allNPCs) {
        npc->GetPosition(nx, ny);
        float dx = px - nx;
        float dy = py - ny;
        if (sqrtf(dx * dx + dy * dy) < (playerRadius + npcRadius)) {
            activeNPC = npc;
            return true;
        }
    }
    return false;
}

bool IsPlayerNearLadder() {
    float px, py;
    myPlayer->GetPosition(px, py);
    return fabsf(px - myLevel->GetLadderX()) < 120.0f;
}
void LoadCar(int carNumber) {
    for (NPC* npc : allNPCs) {
        delete npc;
    }
    allNPCs.clear();
    activeNPC = nullptr;

    if (carNumber == 1) {
        allNPCs.push_back(new NPC(".\\TestData\\rosamund_idle.png", "Rosamund", 11, rosamundLoot, 790.0f, 330.0f, 0.19f));
        allNPCs.push_back(new NPC(".\\TestData\\granny_idle.png", "Granny", 8, grannyLoot, 1750.0f, 310.0f, 0.16f));
        allNPCs.push_back(new NPC(".\\TestData\\randy_idle.png", "Randy", 14, randyLoot, 320.0f, 330.0f, 0.2f));
    }
    else if (carNumber == 2) {
        allNPCs.push_back(new NPC(".\\TestData\\npc_sprites.png", "Helene", 10, heleneLoot, 400.0f, 330.0f, 0.2f, 2, 4, 0, 2));
        allNPCs.push_back(new NPC(".\\TestData\\npc_sprites.png", "Magda", 12, magdaLoot, 900.0f, 330.0f, 0.2f, 2, 4, 1, 2));
        allNPCs.push_back(new NPC(".\\TestData\\npc_sprites.png", "Charles", 13, charlesLoot, 1900.0f, 330.0f, 0.2f, 2, 4, 3, 2));
    }
    else if (carNumber == 3) {
        allNPCs.push_back(new NPC(".\\TestData\\npc_sprites.png", "Lupine", 17, lupineLoot, 1400.0f, 330.0f, 0.2f, 2, 4, 2, 2));
    }

    myPlayer->SetPosition(200.0f, 250.0f);
    g_camera.x = 0.0f;
}


void Init() {
    App::PlaySound(".\\TestData\\audio\\jazz.wav", DSBPLAY_LOOPING);
    App::PlaySound(".\\TestData\\audio\\train_sounds.wav", DSBPLAY_LOOPING);
    App::PlaySound(".\\TestData\\audio\\rain.wav", DSBPLAY_LOOPING);
    App::PlaySound(".\\TestData\\goldsteal.wav", true);

    myCaughtMenu = new CaughtMenu();
    myPauseMenu = new PauseMenu();
    myPatroller = new Patroller();
    srand((unsigned int)time(nullptr));
    alertIcon = App::CreateSprite(".\\TestData\\exclamation.png", 1, 1);
    alertIcon->SetScale(0.1f);
    g_clock = new GameClock();
    myCrowdManager = new CrowdManager();
    myPlayer = new Player();
    myUI = new UIManager();
    myLevel = new Level();
    myRooftop = new Rooftop();
    myMainMenu = new MainMenu();
    myIntro = new Intro();
    // load the first car ! move this to own class eventually ?
    LoadCar(1);

    // IMFellEnglish-Regular
    // UncialAntiqua-Regular

    //App::InitFont(".\\TestData\\IMFellEnglish-Regular.ttf", 18.0f, 0);  // dialogue = 0
    //App::InitFont(".\\TestData\\UncialAntiqua-Regular.ttf", 28.0f, 1);  // titles = 1
}

// UPDATE!! ***********************************************************************
void Update(float deltaTime) {

    // load fonts

    if (!g_fontsLoaded) {
        App::InitFont(".\\TestData\\fonts\\IMFellEnglish-Regular.ttf", 24.0f, 0);
        App::InitFont(".\\TestData\\fonts\\UncialAntiqua-Regular.ttf", 25.0f, 1);
        g_fontsLoaded = true;
    }

    // main menu
    if (g_scene == SceneState::MAIN_MENU) {
        myMainMenu->Update(deltaTime);
        if (myMainMenu->ShouldStart()) {
            g_scene = SceneState::INTRO;
        }
        return;
    }

    // intro
    if (g_scene == SceneState::INTRO) {
        myIntro->Update(deltaTime);
        if (myIntro->IsDone()) {
            g_scene = SceneState::TRAIN_INTERIOR;
        }
        return;
    }

    // pause menu logic
    bool escDown = App::IsKeyPressed(VK_ESCAPE);
    if (escDown && !g_escWasDown) {
        // prevent double opening pause and ui
        if (!myUI->IsAnyUIOpen() && !myRooftop->IsTrading()) {
            g_isPaused = !g_isPaused;
        }
    }
    g_escWasDown = escDown;
    if (g_isPaused) {
        myPauseMenu->Update(g_isPaused);
        return;
    }

    // rooftop
    if (g_scene == SceneState::ROOFTOP) {
        g_clock->Update(deltaTime);
        float px, py;
        myPlayer->GetPosition(px, py);

        myRooftop->Update(deltaTime, px, playerInventory, g_clock->IsDay());

        if (myRooftop->JustSlept()) {
            g_clock->AdvanceToMorning();
        }

        g_nearHatch = myRooftop->IsPlayerNearHatch(px);
        if (g_nearHatch && App::IsKeyPressed(VK_DOWN)) {
            myPlayer->SetPosition(myLevel->GetLadderX(), 250.0f);
            g_scene = SceneState::TRAIN_INTERIOR;
        }

        if (!myRooftop->IsTrading()) {
            myPlayer->Update(deltaTime);
        }
        return;
    }

    g_clock->Update(deltaTime);
    
    if (myPatroller->IsPlayerCaught()) {
        g_caughtAnimTimer += deltaTime / 1000.0f;

        if (g_caughtAnimTimer < CAUGHT_ANIM_DURATION) {
            float flash = fmodf(g_caughtAnimTimer, 0.2f) < 0.1f ? 1.0f : 0.3f;
            myPlayer->SetColor(1.0f, flash * 0.45f, flash * 0.45f);

            // camera shake? or should i do sprite shake
            float shake = sinf(g_caughtAnimTimer * 40.0f) * 9.0f;
            g_camera.x += shake;
            return;
        }

        myPlayer->SetColor(1.0f, 1.0f, 1.0f);
        bool retry = false;
        myCaughtMenu->Update(retry);
        if (retry) {
            myPatroller->Reset();
            g_caughtAnimTimer = 0.0f;
            LoadCar(1);
        }
        return;
    }

    myUI->Update(deltaTime, activeNPC, playerInventory);
    if (!myUI->IsAnyUIOpen()) {
        float px, py;
        myPlayer->GetPosition(px, py);

        // ticketman logic
        bool changeCar = false;
        int currentGold = myUI->GetGoldAmount();
        myLevel->UpdateGuard(px, currentGold, changeCar, deltaTime);
        myUI->SetGoldAmount(currentGold);

        if (changeCar) {
            int nextCar = myLevel->GetCurrentCar();
            LoadCar(nextCar);
            myPlayer->GetPosition(px, py);
        }

        if (!myLevel->IsGuardUIOpen()) {
            g_camera.x = px - 512.0f;
            if (g_camera.x < 0.0f) g_camera.x = 0.0f;
            if (g_camera.x > 3225.0f - 1024.0f) g_camera.x = 3225.0f - 1024.0f;

            bool playerInClump = myCrowdManager->IsPlayerInClump(px, py) && myPlayer->IsHiding();
            myPatroller->Update(deltaTime, px, py, playerInClump, g_camera.x);
            myLevel->Update(deltaTime);
            myCrowdManager->Update(deltaTime);
            myPlayer->Update(deltaTime);

            for (NPC* npc : allNPCs) {
                npc->Update(deltaTime);
            }

            g_nearLadder = IsPlayerNearLadder() && !myUI->inPickpocketUI;
            if (g_nearLadder && App::IsKeyPressed(VK_UP)) {
                myPlayer->SetPosition(myRooftop->GetSpawnX(), myRooftop->GetSpawnY());
                g_scene = SceneState::ROOFTOP;
                return;
            }

            bool nearNPC = IsPlayerNearNPC();

            if (nearNPC && !myUI->inPickpocketUI) {
                if (activeNPC && activeNPC->GetIsAlerted()) {
                    App::PrintTTF(10, 140, "Can't steal from an alert NPC", 1.0f, 0.0f, 0.0f, 0);
                    if (myPatroller->IsInactive()) myPatroller->Activate();
                }
                else {
                    if (App::IsKeyPressed(VK_RETURN)) {
                        if (myLevel->IsPlayerInWalkingNPCVision(px, py)) {
                            // she saw you attempt to steal
                            activeNPC->SetAlerted(true);
                            if (myPatroller->IsInactive()) myPatroller->Activate();
                        }
                        else {
                            myUI->OpenUI();
                        }
                    }
                }
            }

            if (!nearNPC && myUI->inPickpocketUI) {
                myUI->CloseUI();
            }

            myUI->Update(deltaTime, activeNPC, playerInventory);

            g_camera.x = px - 512.0f;
            if (g_camera.x < 0.0f) g_camera.x = 0.0f;
            if (g_camera.x > 3225.0f - 1024.0f) g_camera.x = 3225.0f - 1024.0f;
            if (px > 3225.0f) myPlayer->SetPosition(3225.0f, py);

        }
    }
}

// RENDER!! *************************************************************************

void Render() {
    if (g_scene == SceneState::MAIN_MENU) {
        myMainMenu->Render();
        return;
    }

    if (g_scene == SceneState::INTRO) {
        myIntro->Render();
        return;
    }

    if (g_scene == SceneState::ROOFTOP) {
        myRooftop->Render(g_clock->IsDay());

        if (!myRooftop->IsSleeping()) {
            myPlayer->Render(0.0f, 0.0f, false);
        }
        g_clock->Render();
        float fade = myRooftop->GetFadeBrightness();
        char timeBuf[32];
        sprintf(timeBuf, "Day %d  %02d:00", g_clock->GetDay(), g_clock->GetHour());
        App::PrintTTF(805, 730, timeBuf, fade, fade, fade, 1);

        if (g_nearHatch && !myRooftop->IsTrading() && !myRooftop->IsSleeping()) {
            App::PrintTTF(10, 60, "Press Down to climb back down", fade, fade, 0.0f, 0);
        }

        myRooftop->RenderPlant();
        myRooftop->RenderTradeUI(playerInventory);

        if (g_isPaused) {
            myPauseMenu->Render();
        }
        return;
    }

    myLevel->RenderBackground(g_camera.x);
    g_clock->Render();
    char timeBuf[32];
    sprintf(timeBuf, "Day %d  %02d:00", g_clock->GetDay(), g_clock->GetHour());
    App::PrintTTF(805, 730, timeBuf, 1.0f, 1.0f, 1.0f, 1);


    if (activeNPC && !myUI->inPickpocketUI) {
        App::PrintTTF(10, 60, "Press Enter to check their pockets...", 1.0f, 1.0f, 0.0f, 0);
    }

    if (g_nearLadder) {
        App::PrintTTF(10, 60, "Press Up to climb the ladder", 1.0f, 1.0f, 0.0f, 0);
    }

    myCrowdManager->Render(g_camera.x, g_camera.y);

    for (NPC* npc : allNPCs) {
        bool targeted = (npc == activeNPC && !myUI->inPickpocketUI);
        npc->Render(g_camera.x, g_camera.y, targeted);
    }

    for (NPC* npc : allNPCs) {
        if (npc->GetIsAlerted()) {
            DrawAlertIconAboveNPC(npc);
        }
    }

    float px, py;
    myPlayer->GetPosition(px, py);
    bool inClump = myCrowdManager->IsPlayerInClump(px, py);
    bool hidden = inClump && myPlayer->IsHiding() && !(myLevel->IsPlayerInWalkingNPCVision(px, py));
    myPlayer->Render(g_camera.x, g_camera.y, hidden);

    myPatroller->Render(g_camera.x, g_camera.y);

    myLevel->RenderForeground(g_camera.x, g_camera.y);
    myLevel->RenderWalkingNPCVision(g_camera.x, g_camera.y);

    // new ticketman logic
    myLevel->RenderGuardUI();
    myPlayer->GetPosition(px, py);
    if (myLevel->IsPlayerNearGuard(px) && !myLevel->IsGuardUIOpen()) {
        App::PrintTTF(10, 60, "Press Enter to speak to the Ticketman", 1.0f, 1.0f, 0.0f, 0);
    }

    if (myPatroller->IsPlayerCaught()) {
        if (g_caughtAnimTimer < CAUGHT_ANIM_DURATION) {
            myPlayer->Render(g_camera.x, g_camera.y, false);
        }
        else {
            myCaughtMenu->Render();
        }
        return;
    }

    myUI->Render(activeNPC, playerInventory);
    if (g_isPaused) {
        myPauseMenu->Render();
    }
}

void Shutdown() {
    delete alertIcon;
    delete g_clock;
    delete myCrowdManager;
    delete myPlayer;
    delete myUI;
    delete myLevel;
    delete myRooftop;
    delete myPatroller;
    delete myMainMenu;
    delete myIntro;
    delete myPauseMenu;
    delete myCaughtMenu;
    for (NPC* npc : allNPCs) delete npc;
    allNPCs.clear();
}