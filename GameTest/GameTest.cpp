#include "stdafx.h"
#include <windows.h> 
#include <math.h>  
#include "app\app.h"
#include <string>
#include <cstring>
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
#include "GearManager.h"
#include "Outro.h"
#include "NPCManager.h"
#include "StatSelect.h"
#include "PlayerArchetype.h"
#include "CharismaticMenu.h"

#define SKIP_INTRO    false
#define SKIP_TO_OUTRO false



#include "BadOutro.h"
BadOutro* myBadOutro = nullptr;
static constexpr int MAX_DAYS = 12;
bool g_firstRequestShown = false;
bool g_firstRequestDismissed = false;

enum class SceneState {
    MAIN_MENU,
    INTRO,
    STAT_SELECT,
    TRAIN_INTERIOR,
    CAR_TRANSITION,
    ROOFTOP,
    OUTRO,
    BAD_OUTRO,
    LADDER_TRANSITION,
    OUTRO_INTRO
};

const char* LORE_STRINGS[13][3] = {
    {"ADAM TILER:", "A pickpocket's associate, who receives the stolen goods, and runs off with them.", ""},
    {"ANABAPTIST:", "A pickpocket caught in the fact, and punished with the discipline of the pump or horse-pond.", ""},
    {"BULK AND FILE:", "Two pickpockets; the bulk jostles the party to be robbed, and the file does the business.", ""},
    {"KNUCKLES:", "Pickpockets who attend the avenues to public places to steal pocket-books, watches, &c.", ""},
    {"CANNON:", "In pickpocket parlance it signifies a pickpocket of indefinite order. A pickpocket of the highest level.", ""},
    {"JERVE:", "A vest pocket; the \"tool\"; the \"wire\"; the \"claw\" in a pickpocket mob.", ""},
    {"KICK:", "The front pocket of pants.", ""},
    {"NICK:", "To surreptitiously extract something from the person; to purloin by stealth in personal presence of a victim.", ""},
    {"PROP:", "A diamond stud originally, now comprehending diamonds in any sense.", ""},
    {"REEF:", "To lift a pocket lining so it may be slowly turned inside out without detection; it is done in cases", "where the pocket is too deep, tight or where extraordinary caution is expedient."},
    {"TOG:", "An overcoat used for a shield. From Latin \"Toga,\" a cloak.", ""},
    {"TWEEZER:", "A small pocketbook with knob clasps.", ""},
    {"WIRE:", "The principal craftsman in a group of pickpockets.", ""}
};
int g_loreIndex = 0;
float g_ladderFadeTimer = 0.0f;
bool g_ladderGoingUp = false;

bool g_fontsLoaded = false;

CSimpleSprite* g_transitionFrames[4];
float g_transitionTimer = 0.0f;
int g_transitionCurrentFrame = 0;
int g_nextCarToLoad = 1;

bool g_isPaused = false;
bool g_escWasDown = false;
PauseMenu* myPauseMenu;
Outro* myOutro = nullptr;

float  g_outroIntroTimer   = 0.0f;
int    g_outroIntroPhase   = 0;
std::string g_outroIntroText;
const char* g_outroIntroFull[2] = { "", "" };
int    g_outroIntroIdx     = 0;
float  g_outroTypeTimer    = 0.0f;
int    g_outroTypeIndex    = 0;
static constexpr float OUTRO_INTRO_FADE   = 1.2f;
static constexpr float OUTRO_INTRO_PAUSE  = 2.5f;
static constexpr float OUTRO_TYPE_SPEED   = 0.035f;




CaughtMenu* myCaughtMenu;
CharismaticMenu* myCharismaticMenu = nullptr;
float g_caughtAnimTimer = 0.0f;
static constexpr float CAUGHT_ANIM_DURATION = 0.6f;

SceneState g_scene = SKIP_TO_OUTRO ? SceneState::OUTRO_INTRO
                   : SKIP_INTRO    ? SceneState::TRAIN_INTERIOR
                   : SceneState::MAIN_MENU;
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
StatSelect* myStatSelect = nullptr;

CSimpleSprite* alertIcon;
CSimpleSprite* m_dialogue_bg = nullptr;

std::vector<Item> playerInventory;
NPCManager* myNPCManager;


void Init() {
    App::PlaySound(".\\TestData\\audio\\jazz.wav", DSBPLAY_LOOPING);
    App::SetSoundVolume(".\\TestData\\audio\\jazz.wav", 0.2f);  

    App::PlaySound(".\\TestData\\audio\\train_sounds.wav", DSBPLAY_LOOPING);
    App::SetSoundVolume(".\\TestData\\audio\\train_sounds.wav", 0.7f);

    App::PlaySound(".\\TestData\\audio\\rain.wav", DSBPLAY_LOOPING);
    App::SetSoundVolume(".\\TestData\\audio\\rain.wav", 0.5f);

    myCaughtMenu = new CaughtMenu();
    myPauseMenu = new PauseMenu();
    myPatroller = new Patroller();
    srand((unsigned int)time(nullptr));
    alertIcon = App::CreateSprite(".\\TestData\\exclamation.png", 1, 1);
    m_dialogue_bg = App::CreateSprite(".\\TestData\\ui_screen.png", 1, 1);
    m_dialogue_bg->SetScale(0.6f);
    alertIcon->SetScale(0.2f);
    g_clock = new GameClock();
    myCrowdManager = new CrowdManager();
    myPlayer = new Player();
    myUI = new UIManager();
    myLevel = new Level();
    myRooftop = new Rooftop();
    myMainMenu = new MainMenu();
    myIntro = new Intro();
    myNPCManager = new NPCManager();
    myNPCManager->SetAlertIcon(alertIcon);
    myNPCManager->LoadCar(1, myPlayer, g_camera.x);

    for (int i = 0; i < 4; i++) {
        std::string path = ".\\TestData\\transition_" + std::to_string(i + 1) + ".png";
        g_transitionFrames[i] = App::CreateSprite(path.c_str(), 1, 1);
        g_transitionFrames[i]->SetPosition(512.0f, 415.0f);
        g_transitionFrames[i]->SetScale(0.7f);
    }

    
    

    
    

    if (SKIP_TO_OUTRO && !myOutro) myOutro = new Outro();
}

void ResetGameState() {
    myPatroller->Reset();
    g_caughtAnimTimer = 0.0f;
    playerInventory.clear();
    myUI->ResetGold();
    myRooftop->Reset();
    GearManager::GetInstance().Reset();
    g_clock->Reset();
    myLevel->Reset();
    myNPCManager->LoadCar(1, myPlayer, g_camera.x);
    g_firstRequestDismissed = false;
}


void Update(float deltaTime) {
    if (GearManager::GetInstance().IsComplete() && g_scene != SceneState::OUTRO && g_scene != SceneState::OUTRO_INTRO) {
        g_scene = SceneState::OUTRO_INTRO;
        g_outroIntroTimer   = 0.0f;
        g_outroIntroPhase   = 0;
        g_outroIntroText    = "";
        g_outroIntroFull[0] = "You have brought the goblin his final request...";
        g_outroIntroFull[1] = "You are awoken in the night...";
        g_outroIntroIdx     = 0;
        g_outroTypeTimer    = 0.0f;
        g_outroTypeIndex    = 0;
    }

    if (g_scene == SceneState::OUTRO_INTRO) {
        float dt = deltaTime / 1000.0f;
        g_outroIntroTimer += dt;

        
        const char* full = g_outroIntroFull[g_outroIntroIdx];
        int fullLen = (int)strlen(full);
        if (g_outroTypeIndex < fullLen) {
            g_outroTypeTimer += dt;
            if (g_outroTypeTimer >= OUTRO_TYPE_SPEED) {
                g_outroTypeTimer = 0.0f;
                g_outroIntroText += full[g_outroTypeIndex];
                g_outroTypeIndex++;
            }
        }

        
        bool typingDone = (g_outroTypeIndex >= fullLen);
        if (typingDone && g_outroIntroTimer > OUTRO_INTRO_PAUSE) {
            if (g_outroIntroIdx < 1) {
                g_outroIntroIdx++;
                g_outroIntroTimer = 0.0f;
                g_outroIntroText  = "";
                g_outroTypeTimer  = 0.0f;
                g_outroTypeIndex  = 0;
            } else {
                
                if (!myOutro) myOutro = new Outro();
                g_scene = SceneState::OUTRO;
            }
        }
        return;
    }

    if (g_scene == SceneState::OUTRO) {
        myOutro->Update(deltaTime);
        if (myOutro->ShouldReturnToMenu()) {
            g_scene = SceneState::MAIN_MENU;
            delete myOutro;
            myOutro = nullptr;
            ResetGameState();
        }
        return;
    }

    if (g_scene == SceneState::BAD_OUTRO) {
        myBadOutro->Update(deltaTime);
        if (myBadOutro->ShouldReturnToMenu()) {
            g_scene = SceneState::MAIN_MENU;
            delete myBadOutro;
            myBadOutro = nullptr;
            ResetGameState();
        }
        return;
    }

    

    if (!g_fontsLoaded) {
        App::InitFont(".\\TestData\\fonts\\IMFellEnglish-Regular.ttf", 24.0f, 0);
        App::InitFont(".\\TestData\\fonts\\UncialAntiqua-Regular.ttf", 25.0f, 1);
        g_fontsLoaded = true;
    }

    
    if (g_scene == SceneState::MAIN_MENU) {
        myMainMenu->Update(deltaTime);
        if (myMainMenu->ShouldStart()) {
            g_scene = SceneState::INTRO;
        } else if (myMainMenu->ShouldExit()) {
            PostQuitMessage(0);
        }
        return;
    }

    if (g_scene == SceneState::INTRO) {
        myIntro->Update(deltaTime);
        if (myIntro->IsDone()) {
            g_scene = SceneState::STAT_SELECT;
            if (!myStatSelect) myStatSelect = new StatSelect();
        }
        return;
    }

    if (g_scene == SceneState::STAT_SELECT) {
        myStatSelect->Update(deltaTime);
        if (myStatSelect->IsDone()) {
            g_scene = SceneState::TRAIN_INTERIOR;
            delete myStatSelect;
            myStatSelect = nullptr;
        }
        return;
    }


    bool escDown = App::IsKeyPressed(VK_ESCAPE);
    if (escDown && !g_escWasDown) {
        if (!g_firstRequestDismissed) {
            g_firstRequestDismissed = true;
        }
        else if (myUI->IsAnyUIOpen()) {
            myUI->CloseUI();
        }
        else if (myRooftop->IsTrading() || myRooftop->IsRequestBoardOpen()) {
            myRooftop->CloseUI();
        }
        else {
            if (g_isPaused && myPauseMenu->IsControlsTabOpen()) {
                myPauseMenu->CloseControlsTab();
            } else {
                g_isPaused = !g_isPaused;
            }
        }
    }
    g_escWasDown = escDown;

    if (g_isPaused) {
        myPauseMenu->Update(g_isPaused);
        return;
    }

    
    if (g_scene == SceneState::ROOFTOP) {
        g_clock->Update(deltaTime);
        float px, py;
        myPlayer->GetPosition(px, py);

        myRooftop->Update(deltaTime, px, playerInventory, g_clock->IsDay());

        if (myRooftop->JustSlept()) {
            g_clock->AdvanceToMorning();
            myRooftop->NotifyNewDay();
            myLevel->ResetHeat();
            myLevel->SetNPCAlerted(false);
            myNPCManager->ResetAlerts();
            myPatroller->Reset();

            
            if (g_clock->GetDay() > MAX_DAYS && !GearManager::GetInstance().IsComplete()) {
                g_scene = SceneState::BAD_OUTRO;
                if (!myBadOutro) myBadOutro = new BadOutro();
            }
        }

        if (myRooftop->JustSatByFire()) {
            g_clock->SetHour(17); 
        }

        g_nearHatch = myRooftop->IsPlayerNearHatch(px);
        if (g_nearHatch && App::IsKeyPressed(VK_DOWN)) {
            myPlayer->SetPosition(myLevel->GetLadderX(), 250.0f);
            g_scene = SceneState::TRAIN_INTERIOR;
            return;
        }

        if (!myRooftop->IsTrading()) {
            myPlayer->Update(deltaTime);
        }
        return;
    }

    if (g_scene == SceneState::CAR_TRANSITION) {
        g_transitionTimer += deltaTime / 1000.0f;
        if (g_transitionTimer >= 0.1f) {
            g_transitionTimer = 0.0f;
            g_transitionCurrentFrame++;
            
            if (g_transitionCurrentFrame == 4) {
                myNPCManager->LoadCar(g_nextCarToLoad, myPlayer, g_camera.x);
            }
            if (g_transitionCurrentFrame >= 8) {
                g_scene = SceneState::TRAIN_INTERIOR;
            }
        }
        return;
    }



    g_clock->Update(deltaTime);
    
    if (myPatroller->IsPlayerCaught()) {
        g_caughtAnimTimer += deltaTime / 1000.0f;

        if (g_caughtAnimTimer < CAUGHT_ANIM_DURATION) {
            float flash = fmodf(g_caughtAnimTimer, 0.2f) < 0.1f ? 1.0f : 0.3f;
            myPlayer->SetColor(1.0f, flash * 0.45f, flash * 0.45f);

            float shake = sinf(g_caughtAnimTimer * 40.0f) * 9.0f;
            g_camera.x += shake;
            return;
        }

        myPlayer->SetColor(1.0f, 1.0f, 1.0f);
        bool retry = false;
        myCaughtMenu->Update(retry);
        if (retry) {
            ResetGameState();
        }
        return;
    }

    if (myCharismaticMenu) {
        bool dismissed = false;
        myCharismaticMenu->Update(dismissed);
        if (dismissed) {
            delete myCharismaticMenu;
            myCharismaticMenu = nullptr;
        }
        return;
    }

    if (myPatroller->JustHadEscape()) {
        myCharismaticMenu = new CharismaticMenu();
        myCharismaticMenu->Show(myPatroller->GetLivesRemaining());
    }

            size_t oldInvSize = playerInventory.size();
            myUI->Update(deltaTime, myNPCManager->GetActiveNPC(), playerInventory);
            if (playerInventory.size() > oldInvSize) {
                myLevel->IncreaseHeat();
            }

            if (!myUI->IsAnyUIOpen()) {
        float px, py;
        myPlayer->GetPosition(px, py);

        
        bool changeCar = false;
        int currentGold = myUI->GetGoldAmount();
        myLevel->UpdateGuard(px, currentGold, changeCar, deltaTime);
        myUI->SetGoldAmount(currentGold);

        if (changeCar) {
            g_nextCarToLoad = myLevel->GetCurrentCar();
            g_scene = SceneState::CAR_TRANSITION;
            g_transitionTimer = 0.0f;
            g_transitionCurrentFrame = 0;
            myPlayer->GetPosition(px, py);
        }

        if (!myLevel->IsGuardUIOpen()) {
            g_camera.x = px - 512.0f;
            if (g_camera.x < 0.0f) g_camera.x = 0.0f;
            if (g_camera.x > 3225.0f - 1024.0f) g_camera.x = 3225.0f - 1024.0f;

            
            bool inWalkingNPCVision = myLevel->IsPlayerInWalkingNPCVision(px, py);
            bool playerInClump = myCrowdManager->IsPlayerInClump(px, py) && myPlayer->IsHiding() && !inWalkingNPCVision;

            App::SetSoundVolume(".\\TestData\\audio\\jazz.wav", playerInClump ? 0.08f : 0.3f);
            App::SetSoundVolume(".\\TestData\\audio\\train_sounds.wav", playerInClump ? 0.1f : 0.5f);
            App::SetSoundVolume(".\\TestData\\audio\\rain.wav", playerInClump ? 0.2f : 0.4f);

            myPatroller->Update(deltaTime, px, py, playerInClump, g_camera.x, myLevel->GetHeatLevel());
            if (myPatroller->IsInactive()) {
                myLevel->SetNPCAlerted(false);
            }
            myLevel->Update(deltaTime);
            myCrowdManager->Update(deltaTime);
            myPlayer->Update(deltaTime);

            myNPCManager->Update(deltaTime);

            g_nearLadder = myLevel->IsPlayerNearLadder(px) && !myUI->inPickpocketUI;
            if (g_nearLadder && App::IsKeyPressed(VK_UP)) {
                myPlayer->SetPosition(myRooftop->GetSpawnX(), myRooftop->GetSpawnY());
                g_scene = SceneState::ROOFTOP;
                return;
            }

            bool nearNPC = myNPCManager->IsPlayerNearNPC(myPlayer);

            if (nearNPC && !myUI->inPickpocketUI) {
                NPC* activeNPC = myNPCManager->GetActiveNPC();
                if (activeNPC && activeNPC->GetIsAlerted()) {
                    App::PrintTTF(10, 140, "Can't steal from an alert NPC", 0.239f, 0.0f, 0.0f, 0);
                    if (myPatroller->IsInactive()) myPatroller->Activate();
                }
                else {
                    if (App::IsKeyPressed(VK_RETURN)) {
                        if (myLevel->IsPlayerInWalkingNPCVision(px, py)) {
                            activeNPC->SetAlerted(true);
                            myLevel->SetNPCAlerted(true);
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

            myUI->Update(deltaTime, myNPCManager->GetActiveNPC(), playerInventory);

            g_camera.x = px - 512.0f;
            if (g_camera.x < 0.0f) g_camera.x = 0.0f;
            if (g_camera.x > 3225.0f - 1024.0f) g_camera.x = 3225.0f - 1024.0f;
            if (px > 3225.0f) myPlayer->SetPosition(3225.0f, py);

        }
    }
}



void Render() {
    if (g_scene == SceneState::BAD_OUTRO) {
        myBadOutro->Render();
        return;
    }
    if (g_scene == SceneState::OUTRO_INTRO) {
        
        float textAlpha = 1.0f;
        if (g_outroIntroTimer < OUTRO_INTRO_FADE) {
            textAlpha = g_outroIntroTimer / OUTRO_INTRO_FADE;
        }
        App::PrintTTF(174, 420, g_outroIntroText.c_str(), textAlpha, textAlpha, textAlpha, 0);
        return;
    }
    if (g_scene == SceneState::OUTRO) {
        myOutro->Render();
        return;
    }
    if (g_scene == SceneState::MAIN_MENU) {
        myMainMenu->Render();
        return;
    }

    if (g_scene == SceneState::INTRO) {
        myIntro->Render();
        return;
    }

    if (g_scene == SceneState::STAT_SELECT) {
        if (myStatSelect) myStatSelect->Render();
        return;
    }
    
    if (g_scene == SceneState::CAR_TRANSITION) {
        
        int frame = g_transitionCurrentFrame % 4;
        if (g_transitionCurrentFrame < 8) {
            g_transitionFrames[frame]->Draw();
        }
        return;
    }



    if (g_scene == SceneState::ROOFTOP) {
        myRooftop->Render(g_clock->IsDay());

        float fade = myRooftop->GetFadeBrightness();

        if (!myRooftop->IsSleeping() && !myRooftop->IsSittingByFire()) {
            myPlayer->SetColor(fade, fade, fade);
            myPlayer->Render(0.0f, 0.0f, false);
            myPlayer->SetColor(1.0f, 1.0f, 1.0f);
        }

        myRooftop->RenderOverlayText(g_nearHatch, g_clock->IsDay());
        g_clock->Render();
        char timeBuf[64];
        int daysLeft = MAX_DAYS - g_clock->GetDay() + 1;
        if (daysLeft < 0) daysLeft = 0;
        if (daysLeft == 1) {
            sprintf(timeBuf, "1 day left  %02d:00", g_clock->GetHour());
        } else {
            sprintf(timeBuf, "%d days left  %02d:00", daysLeft, g_clock->GetHour());
        }
        App::PrintTTF(760, 730, timeBuf, fade, fade, fade, 1);

        if (g_nearHatch && !myRooftop->IsTrading() && !myRooftop->IsSleeping()) {
            App::PrintTTF(10, 60, "Press Down to climb back down", fade * 1.0f, 1.0f, 1.0f, 0);
        }

        myRooftop->RenderPlant();
        myRooftop->RenderTradeUI(playerInventory);

        if (fade >= 1.0f) {
            g_loreIndex = rand() % 13;
        }

        if (fade < 1.0f) {
            float textAlpha = 1.0f - fade;
            if (myRooftop->IsSittingByFire()) {
                App::PrintTTF(150, 400, "The sound of the train and muffled conversation lull you into a trance-like state...", textAlpha, textAlpha, textAlpha, 0);
            } else if (myRooftop->IsSleepTransition()) {
                App::PrintTTF(150, 420, LORE_STRINGS[g_loreIndex][0], textAlpha, textAlpha, textAlpha, 0);
                App::PrintTTF(150, 390, LORE_STRINGS[g_loreIndex][1], textAlpha, textAlpha, textAlpha, 0);
                if (LORE_STRINGS[g_loreIndex][2][0] != '\0') {
                    App::PrintTTF(150, 360, LORE_STRINGS[g_loreIndex][2], textAlpha, textAlpha, textAlpha, 0);
                }
            }
        }

        if (g_isPaused) {
            myPauseMenu->Render();
        }
        return;
    }

    float tod = g_clock->IsDay() ? 1.0f : 0.45f;
    myLevel->RenderBackground(g_camera.x, tod);


    NPC* activeNPC = myNPCManager->GetActiveNPC();
    if (activeNPC && !myUI->inPickpocketUI) {
        App::PrintTTF(10, 60, "Press Enter to check their pockets...", 0.239f, 0.0f, 0.0f, 0);
    }

    if (g_nearLadder) {
        App::PrintTTF(10, 60, "Press Up to climb the ladder", 0.239f, 0.0f, 0.0f, 0);
    }

    myCrowdManager->Render(g_camera.x, g_camera.y);

    myNPCManager->Render(g_camera.x, g_camera.y, myUI->inPickpocketUI);
    myNPCManager->DrawAlertIcons(g_camera.x);

    float px, py;
    myPlayer->GetPosition(px, py);
    bool inClump = myCrowdManager->IsPlayerInClump(px, py);
    bool hidden = inClump && myPlayer->IsHiding() && !(myLevel->IsPlayerInWalkingNPCVision(px, py));
    myPlayer->Render(g_camera.x, g_camera.y, hidden);

    myPatroller->Render(g_camera.x, g_camera.y);


    
    myPlayer->GetPosition(px, py);
    bool isCollidingWithCone = myLevel->IsPlayerInWalkingNPCVision(px, py);
    myLevel->RenderWalkingNPCVision(g_camera.x, g_camera.y, isCollidingWithCone);
    myLevel->RenderForeground(g_camera.x, g_camera.y, isCollidingWithCone);

    g_clock->Render();
    char timeBuf[64];
    int daysLeft = MAX_DAYS - g_clock->GetDay() + 1;
    if (daysLeft < 0) daysLeft = 0;
    if (daysLeft == 1) {
        sprintf(timeBuf, "1 day left  %02d:00", g_clock->GetHour());
    } else {
        sprintf(timeBuf, "%d days left  %02d:00", daysLeft, g_clock->GetHour());
    }
    App::PrintTTF(760, 730, timeBuf, 1.0f, 1.0f, 1.0f, 1);

    
    myLevel->RenderGuardUI();
    myPlayer->GetPosition(px, py);
    if (myLevel->IsPlayerNearGuard(px) && !myLevel->IsGuardUIOpen()) {
        App::PrintTTF(10, 60, "Press Enter to speak to the Ticketman", 0.239f, 0.0f, 0.0f, 0);
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

    if (myCharismaticMenu) {
        myCharismaticMenu->Render();
    }

    
    if (!g_firstRequestDismissed) {

        m_dialogue_bg->SetPosition(500.0f, 400.0f);
        m_dialogue_bg->Draw();
        
        const DailyRequest& req = Rooftop::GetRequest(GearManager::GetInstance().GetGearCount());

        App::PrintTTF(200, 520, "Welcome aboard... Please 'borrow' a very special item for me: ", 0.239f, 0.0f, 0.0f, 1);
        App::PrintTTF(200, 480, req.riddle.c_str(), 1.0f, 1.0f, 1.0f, 0);
        App::PrintTTF(200, 430, "Deliver it to me on top of the train tonight.", 0.239f, 0.0f, 0.0f, 0);
        App::PrintTTF(200, 390, "Love, Goblin", 0.239f, 0.0f, 0.0f, 0);
    }

    myUI->Render(activeNPC, playerInventory);
    if (g_isPaused) {
        myPauseMenu->Render();
    }
}

void Shutdown() {
    for (int i = 0; i < 4; i++) delete g_transitionFrames[i];
    delete myBadOutro;
    delete m_dialogue_bg;
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
    delete myStatSelect;
    delete myPauseMenu;
    delete myCaughtMenu;
    delete myCharismaticMenu;
    delete myOutro;
    delete myNPCManager;
}