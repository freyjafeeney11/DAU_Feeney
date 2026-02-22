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

struct Camera {
	float x = 0.0f;
	float y = 0.0f;
	float targetX = 0.0f;
	float width = 1024.0f;
} g_camera;

CrowdManager* myCrowdManager;
Player* myPlayer;
UIManager* myUI;

CSimpleSprite* window;
CSimpleSprite* rain;
CSimpleSprite* alertIcon;
CSimpleSprite* background;
CSimpleSprite* generic;
CSimpleSprite* roamingNPC = nullptr;

std::vector<Item> playerInventory;
std::vector<NPC*> allNPCs;
NPC* activeNPC = nullptr;

int rosamundLoot[6] = { ITEM_GOLD, ITEM_LETTER, ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE };
int grannyLoot[6] = { ITEM_GOLD, ITEM_PICTURE, ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE };
int randyLoot[6] = { ITEM_GOLD, ITEM_FLASHDRIVE, ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE };

NPC* rosamund;
NPC* granny;
NPC* randy;

bool npcActive = false;
float npcTimer = 0.0f;
float npcSpawnDelay = 3.0f;
float npcSpeed = 2.5f;
bool npcMoveRight = true;
bool npcPickpocketable = true;

void DrawInWorld(CSimpleSprite* sprite) {
	if (!sprite) return;
	float actualX, actualY;
	sprite->GetPosition(actualX, actualY);
	sprite->SetPosition(actualX - g_camera.x, actualY - g_camera.y);
	sprite->Draw();
	sprite->SetPosition(actualX, actualY);
}

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
	srand((unsigned int)time(nullptr));

	background = App::CreateSprite(".\\TestData\\Train_bg.png", 1, 1);
	background->SetPosition(500.0f, 400.0f);
	background->SetScale(0.6f);

	window = App::CreateSprite(".\\TestData\\cityscape.png", 7, 1);
	window->SetPosition(470.0f, 450.0f);
	window->SetScale(0.6f);
	window->CreateAnimation(0, 0.5f, { 0, 1, 2, 3, 4, 5, 6 });
	window->SetAnimation(0);

	rain = App::CreateSprite(".\\TestData\\rain.png", 3, 1);
	rain->SetPosition(475.0f, 450.0f);
	rain->SetScale(0.5f);
	rain->CreateAnimation(0, 0.2f, { 0, 1, 2 });
	rain->SetAnimation(0);

	alertIcon = App::CreateSprite(".\\TestData\\exclamation.png", 1, 1);
	alertIcon->SetScale(0.1f);

	roamingNPC = App::CreateSprite(".\\TestData\\npc_walk.png", 4, 1);
	roamingNPC->SetScale(0.2f);
	roamingNPC->CreateAnimation(0, 0.2f, { 0,1,2,3 });
	roamingNPC->SetAnimation(0);
	roamingNPC->SetPosition(-200.0f, -200.0f);

	generic = App::CreateSprite(".\\TestData\\IMG_1297.png", 1, 1);
	generic->SetPosition(600.0f, 340.0f);
	generic->SetScale(0.5f);

	rosamund = new NPC(".\\TestData\\rosamund_idle.png", "Rosamund", 11, rosamundLoot, 590.0f, 360.0f, 0.2f);
	granny = new NPC(".\\TestData\\granny_idle.png", "Granny", 8, grannyLoot, 710.0f, 300.0f, 0.14f);
	randy = new NPC(".\\TestData\\randy_idle.png", "Randy", 14, randyLoot, 350.0f, 340.0f, 0.2f);

	allNPCs.push_back(rosamund);
	allNPCs.push_back(granny);
	allNPCs.push_back(randy);

	myCrowdManager = new CrowdManager();
	myPlayer = new Player();
	myUI = new UIManager();
}

void Update(float deltaTime) {
	float px, py;
	myPlayer->GetPosition(px, py);
	g_camera.x = px - 512.0f;

	if (g_camera.x < 0) {
		g_camera.x = 0;
	}

	npcTimer += deltaTime / 1000.0f;

	if (!npcActive) {
		if (npcTimer >= npcSpawnDelay) {
			npcTimer = 0.0f;
			npcActive = true;
			npcMoveRight = (rand() % 2) == 0;
			float startX = npcMoveRight ? -100.0f : 1100.0f;
			float startY = 280.0f;
			roamingNPC->SetPosition(startX, startY);
			roamingNPC->SetFlipX(npcMoveRight);
			roamingNPC->SetAnimation(0);
		}
	}
	else {
		float x, y;
		roamingNPC->GetPosition(x, y);
		x += (npcMoveRight ? npcSpeed : -npcSpeed);
		roamingNPC->SetPosition(x, y);
		roamingNPC->SetFlipX(!npcMoveRight);

		if (x > 1200.0f || x < -200.0f) {
			npcActive = false;
			npcTimer = 0.0f;
			roamingNPC->SetPosition(-200.0f, -200.0f);
		}
		roamingNPC->Update(deltaTime);
	}

	myCrowdManager->Update(deltaTime);
	myPlayer->Update(deltaTime);

	rosamund->Update(deltaTime);
	randy->Update(deltaTime);
	granny->Update(deltaTime);
	window->Update(deltaTime);
	rain->Update(deltaTime);

	bool nearNPC = IsPlayerNearNPC();

	if (nearNPC && npcPickpocketable && !myUI->inPickpocketUI) {
		if (activeNPC && activeNPC->GetIsAlerted()) {
			App::Print(10, 140, "Can't steal from an alert NPC", 1.0f, 0.0f, 0.0f);
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

	if (App::GetController().CheckButton(XINPUT_GAMEPAD_B, true)) {
		App::PlaySound(".\\TestData\\Test.wav");
	}
}

void Render() {
	float bgWidth = 1075.0f;
	float windowWidth = 1024.0f;

	float windowScrollX = fmodf(g_camera.x * 0.3f, windowWidth);
	float bgScrollX = fmodf(g_camera.x, bgWidth);

	window->SetPosition(512.0f - windowScrollX, 450.0f);
	window->Draw();
	window->SetPosition(512.0f - windowScrollX + windowWidth, 450.0f);
	window->Draw();

	rain->SetPosition(475.0f - windowScrollX, 450.0f);
	rain->Draw();
	rain->SetPosition(475.0f - windowScrollX + windowWidth, 450.0f);
	rain->Draw();

	background->SetPosition(500.0f - bgScrollX, 400.0f);
	background->Draw();
	background->SetPosition(500.0f - bgScrollX + bgWidth, 400.0f);
	background->Draw();

	bool colliding = (activeNPC != nullptr);
	if (colliding && !myUI->inPickpocketUI) {
		App::Print(10, 100, "Press Space to view inventory", 0.0f, 0.0f, 0.0f);
	}

	rosamund->Render(g_camera.x, g_camera.y);
	myCrowdManager->Render(g_camera.x, g_camera.y);
	myPlayer->Render(g_camera.x, g_camera.y);
	granny->Render(g_camera.x, g_camera.y);
	randy->Render(g_camera.x, g_camera.y);

	if (npcActive) {
		DrawInWorld(roamingNPC);
	}

	for (NPC* npc : allNPCs) {
		if (npc->GetIsAlerted()) {
			DrawAlertIconAboveNPC(npc);
		}
	}

	myUI->Render(activeNPC, playerInventory);
}

void Shutdown() {
	delete window;
	delete rain;
	delete rosamund;
	delete granny;
	delete randy;
	delete background;
	delete roamingNPC;
	delete generic;
	delete alertIcon;

	delete myCrowdManager;
	delete myPlayer;
	delete myUI;
}