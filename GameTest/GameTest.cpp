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

#define SKIP_INTRO true

// things to fix: y level too high after leaving camp, player inventory sprites

SceneState g_scene = SKIP_INTRO ? SceneState::TRAIN_INTERIOR : SceneState::MAIN_MENU;
bool g_nearLadder = false;
bool g_nearHatch = false;

struct Camera {
	float x = 0.0f;
	float y = 0.0f;
	float targetX = 0.0f;
	float width = 1024.0f;
} g_camera;

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
int grannyLoot[6] = { ITEM_GOLD, ITEM_PICTURE,   ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE };
int randyLoot[6] = { ITEM_GOLD, ITEM_FLASHDRIVE, ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE };

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

void Init() {
	//App::PlaySound(".\\TestData\\jazz.wav", DSBPLAY_LOOPING);
	//App::PlaySound(".\\TestData\\rain.wav", true);
	//App::PlaySound(".\\TestData\\train_sounds.wav", true);

	myPatroller = new Patroller();
	srand((unsigned int)time(nullptr));

	alertIcon = App::CreateSprite(".\\TestData\\exclamation.png", 1, 1);
	alertIcon->SetScale(0.1f);

	rosamund = new NPC(".\\TestData\\rosamund_idle.png", "Rosamund", 11, rosamundLoot, 700.0f, 330.0f, 0.19f);
	granny = new NPC(".\\TestData\\granny_idle.png", "Granny", 8, grannyLoot, 1620.0f, 320.0f, 0.16f);
	randy = new NPC(".\\TestData\\randy_idle.png", "Randy", 14, randyLoot, 250.0f, 330.0f, 0.2f);

	allNPCs.push_back(rosamund);
	allNPCs.push_back(granny);
	allNPCs.push_back(randy);

	myCrowdManager = new CrowdManager();
	myPlayer = new Player();
	myUI = new UIManager();
	myLevel = new Level();
	myRooftop = new Rooftop();
	myMainMenu = new MainMenu();
	myIntro = new Intro();
}

void Update(float deltaTime) {

	if (g_scene == SceneState::MAIN_MENU) {
		myMainMenu->Update(deltaTime);
		if (myMainMenu->ShouldStart()) {
			g_scene = SceneState::INTRO;
		}
		return;
	}

	if (g_scene == SceneState::INTRO) {
		myIntro->Update(deltaTime);
		if (myIntro->IsDone()) {
			g_scene = SceneState::TRAIN_INTERIOR;
		}
		return;
	}

	if (g_scene == SceneState::ROOFTOP) {
		float px, py;
		myPlayer->GetPosition(px, py);
		myRooftop->Update(deltaTime, px);

		g_nearHatch = myRooftop->IsPlayerNearHatch(px);
		if (g_nearHatch && App::IsKeyPressed(VK_DOWN)) {
			myPlayer->SetPosition(myLevel->GetLadderX(), 250.0f);
			g_scene = SceneState::TRAIN_INTERIOR;
		}

		myPlayer->Update(deltaTime);
		return;
	}

	if (myPatroller->IsPlayerCaught()) return;

	float px, py;
	myPlayer->GetPosition(px, py);

	g_camera.x = px - 512.0f;
	if (g_camera.x < 0.0f) g_camera.x = 0.0f;

	bool playerInClump = myCrowdManager->IsPlayerInClump(px, py);
	myPatroller->Update(deltaTime, px, py, playerInClump, g_camera.x);

	myLevel->Update(deltaTime);
	myCrowdManager->Update(deltaTime);
	myPlayer->Update(deltaTime);

	rosamund->Update(deltaTime);
	randy->Update(deltaTime);
	granny->Update(deltaTime);

	g_nearLadder = IsPlayerNearLadder() && !myUI->inPickpocketUI;
	if (g_nearLadder && App::IsKeyPressed(VK_UP)) {
		myPlayer->SetPosition(myRooftop->GetSpawnX(), myRooftop->GetSpawnY());
		g_scene = SceneState::ROOFTOP;
		return;
	}

	bool nearNPC = IsPlayerNearNPC();

	if (nearNPC && !myUI->inPickpocketUI) {
		if (activeNPC && activeNPC->GetIsAlerted()) {
			App::Print(10, 140, "Can't steal from an alert NPC", 1.0f, 0.0f, 0.0f);
			if (myPatroller->IsInactive()) {
				myPatroller->Activate();
			}
		}
		else {
			if (App::IsKeyPressed(VK_SPACE)) {
				myUI->OpenUI();
			}
		}
	}

	if (!nearNPC && myUI->inPickpocketUI) {
		myUI->CloseUI();
	}

	myUI->Update(deltaTime, activeNPC, playerInventory);
	// x boundary?
	g_camera.x = px - 512.0f;
	if (g_camera.x < 0.0f) g_camera.x = 0.0f;
	if (g_camera.x > 3225.0f - 1024.0f) g_camera.x = 3225.0f - 1024.0f;
	if (px > 3225.0f) myPlayer->SetPosition(3225.0f, py);
}

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
		myRooftop->Render();
		if (!myRooftop->IsSleeping())
			myPlayer->Render(0.0f, 0.0f);
		myRooftop->RenderPlant();
		if (g_nearHatch) {
			App::Print(10, 60, "Press Down to climb back down", 1.0f, 1.0f, 0.0f);
		}
		return;
	}

	myLevel->RenderBackground(g_camera.x);

	if (activeNPC && !myUI->inPickpocketUI) {
		App::Print(10, 100, "Press Space to view inventory", 0.0f, 0.0f, 0.0f);
	}

	if (g_nearLadder) {
		App::Print(10, 60, "Press Up to climb to the roof", 1.0f, 1.0f, 0.0f);
	}

	myCrowdManager->Render(g_camera.x, g_camera.y);
	rosamund->Render(g_camera.x, g_camera.y);
	granny->Render(g_camera.x, g_camera.y);
	randy->Render(g_camera.x, g_camera.y);

	for (NPC* npc : allNPCs) {
		if (npc->GetIsAlerted()) {
			DrawAlertIconAboveNPC(npc);
		}
	}

	myPatroller->Render(g_camera.x, g_camera.y);
	myPlayer->Render(g_camera.x, g_camera.y);
	myLevel->RenderForeground(g_camera.x, g_camera.y);

	if (myPatroller->IsPlayerCaught()) {
		App::Print(350, 400, "You got caught!", 1.0f, 0.0f, 0.0f);
		App::Print(300, 350, "Close the window to restart.", 1.0f, 1.0f, 1.0f);
	}

	myUI->Render(activeNPC, playerInventory);
}

void Shutdown() {
	delete rosamund;
	delete granny;
	delete randy;
	delete alertIcon;

	delete myCrowdManager;
	delete myPlayer;
	delete myUI;
	delete myLevel;
	delete myRooftop;
	delete myPatroller;
	delete myMainMenu;
	delete myIntro;
}