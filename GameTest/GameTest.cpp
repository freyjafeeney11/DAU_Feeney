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
#include "Patroller.h"

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

CSimpleSprite* alertIcon;

std::vector<Item> playerInventory;
std::vector<NPC*> allNPCs;
NPC* activeNPC = nullptr;

int rosamundLoot[6] = { ITEM_GOLD, ITEM_LETTER, ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE };
int grannyLoot[6] = { ITEM_GOLD, ITEM_PICTURE, ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE };
int randyLoot[6] = { ITEM_GOLD, ITEM_FLASHDRIVE, ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE };

NPC* rosamund;
NPC* granny;
NPC* randy;

void DrawAlertIconAboveNPC(NPC* npc) {
	if (!npc || !alertIcon) return;
	float x, y;
	npc->GetPosition(x, y);
	float height = npc->GetHeight();
	float scale = npc->GetScale();
	float worldHeight = height * scale;

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
	float playerRadius = 20.0f;
	float npcRadius = 100.0f;
	for (NPC* npc : allNPCs) {
		npc->GetPosition(nx, ny);
		float dx = px - nx;
		float dy = py - ny;
		float distance = sqrtf(dx * dx + dy * dy);
		if (distance < (playerRadius + npcRadius)) {
			activeNPC = npc;
			return true;
		}
	}
	return false;
}

void Init() {
	myPatroller = new Patroller();
	srand((unsigned int)time(nullptr));

	alertIcon = App::CreateSprite(".\\TestData\\exclamation.png", 1, 1);
	alertIcon->SetScale(0.1f);

	rosamund = new NPC(".\\TestData\\rosamund_idle.png", "Rosamund", 11, rosamundLoot, 600.0f, 360.0f, 0.2f); // ypos, xpos, size
	granny = new NPC(".\\TestData\\granny_idle.png", "Granny", 8, grannyLoot, 1220.0f, 320.0f, 0.15f);
	randy = new NPC(".\\TestData\\randy_idle.png", "Randy", 14, randyLoot, 250.0f, 340.0f, 0.2f);

	allNPCs.push_back(rosamund);
	allNPCs.push_back(granny);
	allNPCs.push_back(randy);

	myCrowdManager = new CrowdManager();
	myPlayer = new Player();
	myUI = new UIManager();
	myLevel = new Level();
}

void Update(float deltaTime) {
	if (myPatroller->IsPlayerCaught()) {
		// game is frozen
		return;
	}
	float px, py;
	myPlayer->GetPosition(px, py);
	g_camera.x = px - 512.0f;

	if (g_camera.x < 0) {
		g_camera.x = 0;
	}
	bool playerInClump = myCrowdManager->IsPlayerInClump(px, py);
	myPatroller->Update(deltaTime, px, py, playerInClump, g_camera.x);

	myLevel->Update(deltaTime);
	myCrowdManager->Update(deltaTime);
	myPlayer->Update(deltaTime);

	rosamund->Update(deltaTime);
	randy->Update(deltaTime);
	granny->Update(deltaTime);

	bool nearNPC = IsPlayerNearNPC();

	if (nearNPC && !myUI->inPickpocketUI) {
		if (activeNPC && activeNPC->GetIsAlerted()) {
			App::Print(10, 140, "Can't steal from an alert NPC", 1.0f, 0.0f, 0.0f);
			if (myPatroller->IsInactive()) {  // only activate if not already running
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
}

void Render() {
	myLevel->RenderBackground(g_camera.x);

	bool colliding = (activeNPC != nullptr);
	if (colliding && !myUI->inPickpocketUI) {
		App::Print(10, 100, "Press Space to view inventory", 0.0f, 0.0f, 0.0f);
	}
	myCrowdManager->Render(g_camera.x, g_camera.y);
	rosamund->Render(g_camera.x, g_camera.y);
	granny->Render(g_camera.x, g_camera.y);
	myPlayer->Render(g_camera.x, g_camera.y);
	randy->Render(g_camera.x, g_camera.y);

	for (NPC* npc : allNPCs) {
		if (npc->GetIsAlerted()) {
			DrawAlertIconAboveNPC(npc);
		}
	}

	myLevel->RenderForeground(g_camera.x, g_camera.y);
	myPatroller->Render(g_camera.x, g_camera.y);
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
	delete myPatroller;
}