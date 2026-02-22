#include "stdafx.h"
#include <windows.h> 
#include <math.h>  
#include "app\app.h"
#include <string>
#include <ctime>
#include "CrowdManager.h"
#include "Player.h"
#include "NPC.h"

struct Camera {
	float x = 0.0f;
	float y = 0.0f;
	float targetX = 0.0f;
	float width = 1024.0f;
} g_camera;

CrowdManager* myCrowdManager;
Player* myPlayer;

CSimpleSprite* inventory_screen;
CSimpleSprite* window;
CSimpleSprite* rain;
CSimpleSprite* rosamund_inv_sprite;
CSimpleSprite* randy_inv_sprite;
CSimpleSprite* granny_inv_sprite;
CSimpleSprite* alertIcon;

CSimpleSprite* icon_gold;
CSimpleSprite* icon_letter;
CSimpleSprite* icon_flashdrive;
CSimpleSprite* icon_picture;

CSimpleSprite* icon_gold_small;
CSimpleSprite* icon_letter_small;
CSimpleSprite* icon_flashdrive_small;
CSimpleSprite* icon_picture_small;

CSimpleSprite* ui_cursor;
int currentSlot = 0;
bool navButtonDown = false;

struct Item {
	int id;
	std::string name;
	std::string flavorText;
	int value;
};

std::vector<Item> playerInventory;

enum { ITEM_NONE, ITEM_GOLD, ITEM_FLASHDRIVE, ITEM_LETTER, ITEM_PICTURE };

std::vector<NPC*> allNPCs;
NPC* activeNPC = nullptr;

int rosamundLoot[6] = { ITEM_GOLD, ITEM_LETTER, ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE };
int grannyLoot[6] = { ITEM_GOLD, ITEM_PICTURE, ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE };
int randyLoot[6] = { ITEM_GOLD, ITEM_FLASHDRIVE, ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE };

float slotCoords[6][2] = {
	{ 190.0f, 600.0f },
	{ 310.0f, 600.0f },
	{ 450.0f, 600.0f },
	{ 190.0f, 450.0f },
	{ 310.0f, 450.0f },
	{ 450.0f, 450.0f }
};

CSimpleSprite* background;
CSimpleSprite* generic;

NPC* rosamund;
NPC* granny;
NPC* randy;

int lastDiceRoll = 0;
bool showDiceResult = false;
bool enterButtonDown = false;
bool lastStealSuccess = false;
bool pickpocketRolled = false;
bool canRob = true;

CSimpleSprite* roamingNPC = nullptr;

bool npcActive = false;
float npcTimer = 0.0f;
float npcSpawnDelay = 3.0f;
float npcSpeed = 2.5f;
bool npcMoveRight = true;

CSimpleSprite* npcPortrait;
bool npcPickpocketable = true;
bool inPickpocketUI = false;
bool pickpocketSuccess = false;


void DrawInWorld(CSimpleSprite* sprite) {
	if (!sprite) return;

	float actualX, actualY;
	sprite->GetPosition(actualX, actualY);

	sprite->SetPosition(actualX - g_camera.x, actualY - g_camera.y);
	sprite->Draw();

	sprite->SetPosition(actualX, actualY);
}

void DrawAlertIconAboveNPC(NPC* npc)
{
	if (!npc || !alertIcon) return;

	float x, y;
	npc->GetPosition(x, y);
	float height = npc->GetHeight();
	float scale = npc->GetScale();
	float worldHeight = height * scale;

	static float t = 0.0f;
	t += 0.05f;

	float bob = sinf(t) * 4.0f;

	alertIcon->SetPosition(
		x - g_camera.x,
		y + (worldHeight * 0.5f) + 10.0f + bob
	);

	alertIcon->Draw();
}


Item GetItemFromLibrary(int itemId) {
	switch (itemId) {
	case ITEM_GOLD:
		return { ITEM_GOLD, "A sack of gold", "It's gold.", 50 };
	case ITEM_LETTER:
		return { ITEM_LETTER, "Perfumed Letter", "It says: idk ill come up with something", 0 };
	case ITEM_FLASHDRIVE:
		return { ITEM_FLASHDRIVE, "FlashDrive", "I wonder what's on this...", 100 };
	case ITEM_PICTURE:
		return { ITEM_PICTURE, "Old Photograph", "It's a photo of two people on a beach.", 100 };
	default:
		return { ITEM_NONE, "Empty Slot", "Put something here..", 0 };
	}
}
bool IsPlayerNearNPC()
{
	float px, py, nx, ny;
	myPlayer->GetPosition(px, py);
	activeNPC = nullptr;
	float playerRadius = 20.0f;
	float npcRadius = 100.0f;
	for (NPC* npc : allNPCs)
	{
		npc->GetPosition(nx, ny);
		float dx = px - nx;
		float dy = py - ny;
		float distance = sqrtf(dx * dx + dy * dy);

		if (distance < (playerRadius + npcRadius))
		{
			activeNPC = npc;
			return true;
		}
	}
	return false;
}

void Init()
{
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

	inventory_screen = App::CreateSprite(".\\TestData\\Inventory.png", 1, 1);
	inventory_screen->SetPosition(500.0f, 400.0f);
	inventory_screen->SetScale(0.6f);

	rosamund_inv_sprite = App::CreateSprite(".\\TestData\\rosamund_portrait_2.png", 1, 1);
	rosamund_inv_sprite->SetPosition(500.0f, 400.0f);
	rosamund_inv_sprite->SetScale(0.6f);
	alertIcon = App::CreateSprite(".\\TestData\\exclamation.png", 1, 1);
	alertIcon->SetScale(0.1f);

	randy_inv_sprite = App::CreateSprite(".\\TestData\\randy_portrait_2.png", 1, 1);
	randy_inv_sprite->SetPosition(500.0f, 400.0f);
	randy_inv_sprite->SetScale(0.6f);

	granny_inv_sprite = App::CreateSprite(".\\TestData\\granny_portrait.png", 1, 1);
	granny_inv_sprite->SetPosition(500.0f, 400.0f);
	granny_inv_sprite->SetScale(0.6f);

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

	npcPortrait = App::CreateSprite(".\\TestData\\pickpocket_dialogue.png", 1, 1);
	npcPortrait->SetPosition(500.0f, 400.0f);
	npcPortrait->SetScale(0.6f);

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
}

void Update(float deltaTime)
{
	if (inPickpocketUI && App::IsKeyPressed(VK_CONTROL))
	{
		inPickpocketUI = false;
	}

	float px, py;
	myPlayer->GetPosition(px, py);
	g_camera.x = px - 512.0f;

	if (g_camera.x < 0)
	{
		g_camera.x = 0;
	}

	npcTimer += deltaTime / 1000.0f;

	if (!npcActive)
	{
		if (npcTimer >= npcSpawnDelay)
		{

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
	else
	{
		float x, y;
		roamingNPC->GetPosition(x, y);

		x += (npcMoveRight ? npcSpeed : -npcSpeed);
		roamingNPC->SetPosition(x, y);
		roamingNPC->SetFlipX(!npcMoveRight);

		if (x > 1200.0f || x < -200.0f)
		{
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

	if (!nearNPC && inPickpocketUI)
	{
		inPickpocketUI = false;
		pickpocketRolled = false;
	}

	if (nearNPC && npcPickpocketable && !inPickpocketUI)
	{
		if (activeNPC && activeNPC->GetIsAlerted())
		{
			App::Print(10, 140, "Can't steal from an alert NPC", 1.0f, 0.0f, 0.0f);
		}
		else
		{
			if (App::IsKeyPressed(VK_SPACE))
			{
				showDiceResult = false;
				inPickpocketUI = true;
			}
		}
	}

	if (inPickpocketUI)
	{
		if (!navButtonDown)
		{
			if (App::IsKeyPressed(VK_RIGHT)) {
				if (currentSlot == 0) currentSlot = 1;
				else if (currentSlot == 1) currentSlot = 2;
				else if (currentSlot == 2) currentSlot = 0;
				else if (currentSlot == 3) currentSlot = 4;
				else if (currentSlot == 4) currentSlot = 5;
				else if (currentSlot == 5) currentSlot = 3;
				navButtonDown = true;
			}
			if (App::IsKeyPressed(VK_LEFT)) {
				if (currentSlot == 1) currentSlot = 0;
				else if (currentSlot == 2) currentSlot = 1;
				else if (currentSlot == 0) currentSlot = 2;
				else if (currentSlot == 4) currentSlot = 3;
				else if (currentSlot == 5) currentSlot = 4;
				else if (currentSlot == 3) currentSlot = 5;
				navButtonDown = true;
			}
			if (App::IsKeyPressed(VK_DOWN)) {
				if (currentSlot >= 0 && currentSlot <= 2) {
					currentSlot += 3;
				}
				navButtonDown = true;
			}
			if (App::IsKeyPressed(VK_UP)) {
				if (currentSlot >= 3 && currentSlot <= 5) {
					currentSlot -= 3;
				}
				navButtonDown = true;
			}
		}
		if (!App::IsKeyPressed(VK_RIGHT) && !App::IsKeyPressed(VK_LEFT) &&
			!App::IsKeyPressed(VK_DOWN) && !App::IsKeyPressed(VK_UP))
		{
			navButtonDown = false;
		}

		if (App::IsKeyPressed(VK_RETURN) && !enterButtonDown)
		{
			enterButtonDown = true;

			if (!activeNPC) return;

			int* currentTable = activeNPC->GetLootTable();
			int difficulty = activeNPC->GetDifficulty();

			if (currentTable[currentSlot] != ITEM_NONE)
			{
				lastDiceRoll = (rand() % 20) + 1;

				if (lastDiceRoll >= difficulty)
				{
					lastStealSuccess = true;

					int stolenId = currentTable[currentSlot];
					playerInventory.push_back(GetItemFromLibrary(stolenId));
					currentTable[currentSlot] = ITEM_NONE;
				}
				else
				{
					lastStealSuccess = false;

					activeNPC->SetAlerted(true);
					inPickpocketUI = false;
				}

				showDiceResult = true;
			}

		}

		if (!App::IsKeyPressed(VK_RETURN))
		{
			enterButtonDown = false;
		}
	}

	if (App::GetController().CheckButton(XINPUT_GAMEPAD_B, true))
	{
		App::PlaySound(".\\TestData\\Test.wav");
	}
}

void Render()
{
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
	if (colliding && !inPickpocketUI) {
		App::Print(10, 100, "Press Space to view inventory", 0.0f, 0.0f, 0.0f);
	}

	rosamund->Render(g_camera.x, g_camera.y);

	if (App::IsKeyPressed('I'))
	{
		App::Print(100, 700, "--- YOUR INVENTORY ---", 1.0f, 1.0f, 1.0f);
		for (int i = 0; i < (int)playerInventory.size(); i++) {
			float yOffset = 650.0f - (i * 30.0f);
			App::Print(100, yOffset, playerInventory[i].name.c_str(), 1.0f, 1.0f, 1.0f);
		}
	}

	myCrowdManager->Render(g_camera.x, g_camera.y);
	myPlayer->Render(g_camera.x, g_camera.y);

	granny->Render(g_camera.x, g_camera.y);
	randy->Render(g_camera.x, g_camera.y);

	if (npcActive)
	{
		DrawInWorld(roamingNPC);
	}

	for (NPC* npc : allNPCs)
	{
		if (npc->GetIsAlerted())
		{
			DrawAlertIconAboveNPC(npc);
		}
	}

	if (inPickpocketUI)
	{
		inventory_screen->Draw();

		if (activeNPC == rosamund) rosamund_inv_sprite->Draw();
		else if (activeNPC == randy) randy_inv_sprite->Draw();
		else if (activeNPC == granny) granny_inv_sprite->Draw();

		int* currentTable = activeNPC->GetLootTable();

		if (currentTable != nullptr)
		{
			for (int i = 0; i < 6; i++)
			{
				int itemID = currentTable[i];
				if (itemID == ITEM_GOLD) icon_gold_small->Draw();
				else if (itemID == ITEM_FLASHDRIVE) icon_flashdrive_small->Draw();
				else if (itemID == ITEM_LETTER) icon_letter_small->Draw();
				else if (itemID == ITEM_PICTURE) icon_picture_small->Draw();
			}
		}

		ui_cursor->SetPosition(slotCoords[currentSlot][0], slotCoords[currentSlot][1]);
		ui_cursor->Draw();

		if (currentTable != nullptr)
		{
			int selectedItem = currentTable[currentSlot];
			if (selectedItem == ITEM_GOLD) icon_gold->Draw();
			else if (selectedItem == ITEM_FLASHDRIVE) icon_flashdrive->Draw();
			else if (selectedItem == ITEM_LETTER) icon_letter->Draw();
			else if (selectedItem == ITEM_PICTURE) icon_picture->Draw();
		}

		if (showDiceResult)
		{
			char resultText[100];
			char resultText1[100];
			sprintf(resultText, "%d", lastDiceRoll);
			if (lastStealSuccess)
			{
				sprintf(resultText1, "Success!");
				App::Print(925, 275, resultText, 0.0f, 0.0f, 0.0f);
				App::Print(890, 190, resultText1, 0.0f, 1.0f, 0.0f);
			}
			else
			{
				sprintf(resultText1, "Failure");
				App::Print(925, 275, resultText, 0.0f, 0.0f, 0.0f);
				App::Print(890, 190, resultText1, 1.0f, 0.0f, 0.0f);
			}
		}
		else
		{
			App::Print(10, 100, "Press Enter to Steal", 0.0f, 0.0f, 0.0f);
		}
	}
}

void Shutdown()
{
	delete window;
	delete rain;
	delete rosamund;
	delete granny;
	delete randy;
	delete background;
	delete roamingNPC;
	delete inventory_screen;
	delete rosamund_inv_sprite;
	delete randy_inv_sprite;
	delete icon_gold;
	delete icon_flashdrive;
	delete icon_letter;
	delete icon_gold_small;
	delete icon_flashdrive_small;
	delete icon_letter_small;
	delete ui_cursor;
	delete alertIcon;

	delete myCrowdManager;
	delete myPlayer;
}