//------------------------------------------------------------------------
// GameTest.cpp
//------------------------------------------------------------------------
#include "stdafx.h"
//------------------------------------------------------------------------
#include <windows.h> 
#include <math.h>  
//------------------------------------------------------------------------
#include "app\app.h"
//------------------------------------------------------------------------
// INVENTORY STUFF
CSimpleSprite *inventory_screen;
// overlays
CSimpleSprite* rosamund_inv_sprite;
CSimpleSprite* randy_inv_sprite;
// navigation
CSimpleSprite* icon_gold;
CSimpleSprite* icon_letter;
CSimpleSprite* icon_flashdrive;
CSimpleSprite* ui_cursor;       // cursor
int currentSlot = 0;
bool navButtonDown = false;  // prevent super fast scrolling

// items available
enum { ITEM_NONE, ITEM_GOLD, ITEM_FLASHDRIVE, ITEM_LETTER };

// loot tables
int rosamundLoot[4] = { ITEM_GOLD, ITEM_LETTER, ITEM_NONE, ITEM_NONE };
int randyLoot[4] = { ITEM_GOLD, ITEM_FLASHDRIVE, ITEM_NONE, ITEM_GOLD };

// coords
float slotCoords[6][2] = {
	{ 190.0f, 600.0f },
	{ 310.0f, 600.0f },
	{ 430.0f, 600.0f },
	{ 190.0f, 450.0f },
	{ 310.0f, 450.0f },
	{ 430.0f, 450.0f } 
};

//-------------------

CSimpleSprite *player;
// metro bg
CSimpleSprite* background;
CSimpleSprite* rosamund;        // NPC
CSimpleSprite* randy;        // NPC
// crowd clumps 
const int NUM_CLUMPS = 3;
const int MEMBERS_PER_CLUMP = 5;

bool pickpocketRolled = false;

bool canRob = true;  // can npc be pickpocketed
const float WALK_SPEED = 2.0f;
const float RUN_SPEED = 4.0f;

// moving npc

CSimpleSprite* roamingNPC = nullptr;

bool npcActive = false;
float npcTimer = 0.0f;
float npcSpawnDelay = 3.0f;   // seconds between appearances
float npcSpeed = 2.5f;
bool npcMoveRight = true;

// pickpocket globals
CSimpleSprite* npcPortrait;
bool npcPickpocketable = true;
bool inPickpocketUI = false;
bool pickpocketSuccess = false;

// NEW COLLISION GLOBALS
std::vector<CSimpleSprite*> npcList; // list of NPCs to collide with
CSimpleSprite* activeNPC = nullptr;  // NPC we are currently near
// -----------------------------

enum
{
	ANIM_IDLE,
	ANIM_HIDE,
	ANIM_WALK,
	ANIM_STEAL,
	ANIM_RUN
};

struct CrowdMember
{
	CSimpleSprite* sprite;
	float offsetX;
	float offsetY;
	float personalSwayOffset;
};

struct CrowdClump
{
	float baseX;
	float baseY;
	float swayOffset;
	CrowdMember members[MEMBERS_PER_CLUMP];
};

CrowdClump crowdClumps[NUM_CLUMPS];

//------------------------------------------------------------------------
bool IsPlayerNearNPC()
{
	float px, py, nx, ny;
	player->GetPosition(px, py);

	activeNPC = nullptr;

	float playerRadius = 20.0f;
	float npcRadius = 100.0f; // might need to be 100

	for (CSimpleSprite* npc : npcList)
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

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
	// make npc pickpocketable
	npcPickpocketable = true;
	// background
	background = App::CreateSprite(".\\TestData\\background2.png", 1, 1);
	background->SetPosition(500.0f, 400.0f);
	background->SetScale(0.6f);

	//inventory
	inventory_screen = App::CreateSprite(".\\TestData\\Inventory.png", 1, 1);
	inventory_screen->SetPosition(500.0f, 400.0f);
	inventory_screen->SetScale(0.6f);

	//overlays
	rosamund_inv_sprite = App::CreateSprite(".\\TestData\\rosamund_inventory.png", 1, 1);
	rosamund_inv_sprite->SetPosition(500.0f, 400.0f);
	rosamund_inv_sprite->SetScale(0.6f);

	randy_inv_sprite = App::CreateSprite(".\\TestData\\randy_inventory.png", 1, 1);
	randy_inv_sprite->SetPosition(500.0f, 400.0f);
	randy_inv_sprite->SetScale(0.6f);

	// inventory items
	icon_gold = App::CreateSprite(".\\TestData\\gold_icon.png", 1, 1);
	icon_gold->SetPosition(500.0f, 400.0f); // Center of screen overlay
	icon_gold->SetScale(0.6f);

	icon_flashdrive = App::CreateSprite(".\\TestData\\flashdrive_icon.png", 1, 1);
	icon_flashdrive->SetPosition(500.0f, 400.0f);
	icon_flashdrive->SetScale(0.6f);

	icon_letter = App::CreateSprite(".\\TestData\\Letter_Icon.png", 1, 1);
	icon_letter->SetPosition(500.0f, 400.0f);
	icon_letter->SetScale(0.6f);

	// temporary cursor
	ui_cursor = App::CreateSprite(".\\TestData\\mask_temp.png", 1, 1);
	ui_cursor->SetScale(0.4f);

	// pickpocket dialogue panel
	npcPortrait = App::CreateSprite(".\\TestData\\pickpocket_dialogue.png", 1, 1);
	npcPortrait->SetPosition(500.0f, 400.0f);
	npcPortrait->SetScale(0.6f);

	//------------------------------------------------------------------------
	// moving npc
	roamingNPC = App::CreateSprite(".\\TestData\\npc_walk.png", 4, 1);
	roamingNPC->SetScale(0.2f);
	roamingNPC->CreateAnimation(0, 0.2f, { 0,1,2,3 });
	roamingNPC->SetAnimation(0);

	// start off screen
	roamingNPC->SetPosition(-200.0f, -200.0f);

	// npc
	rosamund = App::CreateSprite(".\\TestData\\IMG_1297.png", 1, 1); // 4 frame sprite, 1 row
	rosamund->SetPosition(600.0f, 340.0f);
	rosamund->SetScale(0.5f);
	rosamund->CreateAnimation(0, 0.2f, { 0,1,2,3 }); // idle anim
	rosamund->SetAnimation(0);

	// randy
	randy = App::CreateSprite(".\\TestData\\randy.png", 1, 1); // 4 frame sprite, 1 row
	randy->SetPosition(350.0f, 340.0f);
	randy->SetScale(0.5f);
	randy->CreateAnimation(0, 0.2f, { 0,1,2,3 }); // idle anim coming soon
	randy->SetAnimation(0);

	// adding npcs to collision list
	npcList.push_back(rosamund);
	npcList.push_back(randy);

	// crowd clumps 

	float baseX = 300.0f;
	float spacing = 120.0f;

	for (int i = 0; i < NUM_CLUMPS; i++)
	{
		crowdClumps[i].baseX = baseX + i * spacing;
		crowdClumps[i].baseY = 350.0f;
		crowdClumps[i].swayOffset = (rand() % 100) / 100.0f * 6.28f;

		for (int j = 0; j < MEMBERS_PER_CLUMP; j++)
		{
			CrowdMember& m = crowdClumps[i].members[j];

			m.sprite = App::CreateSprite(".\\TestData\\IMG_1297.png", 1, 1);
			m.sprite->SetScale(0.35f);

			// spread people inside the clump
			m.offsetX = (rand() % 41) - 20;   // -20 to +20
			m.offsetY = (rand() % 31) - 15;

			m.personalSwayOffset = (rand() % 100) / 100.0f * 6.28f;
		}
	}



	// Example Sprite Code....
	player = App::CreateSprite(".\\TestData\\player_sprite.png", 3,5);
	player->SetPosition(400.0f, 250.0f);
	float speed = 1.0f / 7.0f;
	player->CreateAnimation(ANIM_IDLE, speed, { 0,1,2 });
	player->CreateAnimation(ANIM_WALK, speed, { 3, 4, 5 });
	player->CreateAnimation(ANIM_HIDE, speed, { 9, 10, 11 });
	player->CreateAnimation(ANIM_STEAL, speed, { 12, 13, 14 });
	player->CreateAnimation(ANIM_RUN, speed, { 6, 7, 8 });
	player->SetScale(0.15f);
	//------------------------------------------------------------------------
}

//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(float deltaTime)
{
	//------------------------------------------------------------------------
	
	// close pickpocket UI with ESC
	if (inPickpocketUI && App::IsKeyPressed(VK_CONTROL))
	{
		inPickpocketUI = false;
	}

	// collider
	
	// moving npc
	npcTimer += deltaTime / 1000.0f;

	if (!npcActive)
	{
		// wait then spawn
		if (npcTimer >= npcSpawnDelay)
		{
			// not sure if i want these here yet
			//npcPickpocketable = true;
			//inPickpocketUI = false;

			npcTimer = 0.0f;
			npcActive = true;

			// random direction
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
		// move across screen
		float x, y;
		roamingNPC->GetPosition(x, y);

		x += (npcMoveRight ? npcSpeed : -npcSpeed);
		roamingNPC->SetPosition(x, y);
		roamingNPC->SetFlipX(!npcMoveRight);

		// despawn when off screen
		if (x > 1200.0f || x < -200.0f)
		{
			npcActive = false;
			npcTimer = 0.0f;
			roamingNPC->SetPosition(-200.0f, -200.0f);
		}

		roamingNPC->Update(deltaTime);
	}
	// crowd clumps
	static float swayTime = 0.0f;
	swayTime += deltaTime / 1000.0f;

	for (int i = 0; i < NUM_CLUMPS; i++)
	{
		CrowdClump& clump = crowdClumps[i];

		float clumpSwayX = sinf(swayTime + clump.swayOffset) * 2.0f;

		for (int j = 0; j < MEMBERS_PER_CLUMP; j++)
		{
			CrowdMember& m = clump.members[j];

			float personalSway =
				sinf(swayTime * 1.5f + m.personalSwayOffset) * 1.5f;

			m.sprite->SetPosition(
				clump.baseX + clumpSwayX + m.offsetX,
				clump.baseY + personalSway + m.offsetY
			);
		}
	}



	if (App::GetController().GetLeftThumbStickX() > 0.5f)
	{
		bool sprinting = App::IsKeyPressed(VK_SHIFT);

		player->SetAnimation(sprinting ? ANIM_RUN : ANIM_WALK);
		player->SetFlipX(true);
		float x, y;
		player->GetPosition(x, y);
		player->SetPosition(x + (sprinting ? RUN_SPEED : WALK_SPEED), y);
	}
	if (App::GetController().GetLeftThumbStickX() < -0.5f)
	{
		bool sprinting = App::IsKeyPressed(VK_SHIFT);

		player->SetAnimation(sprinting ? ANIM_RUN : ANIM_WALK);
		player->SetFlipX(false);
		float x, y;
		player->GetPosition(x, y);
		player->SetPosition(x - (sprinting ? RUN_SPEED : WALK_SPEED), y);
	}
	// STEAL LOGIC
    if (App::GetController().GetLeftThumbStickY() > 0.5f)
    {
		player->SetAnimation(ANIM_HIDE);
    }
	// HIDE LOGIC
	if (App::GetController().GetLeftThumbStickY() < -0.5f)
	{
		player->SetAnimation(ANIM_STEAL);
	}
	// update npc
	rosamund->Update(deltaTime);
	player->Update(deltaTime);
	bool nearNPC = IsPlayerNearNPC();

	// close inventory ui if not near anymore
	if (!nearNPC && inPickpocketUI)
	{
		inPickpocketUI = false;
		pickpocketRolled = false; // reset the minigame
	}

	// pickpocket UI
	if (nearNPC && npcPickpocketable && !inPickpocketUI)
	{
		if (App::IsKeyPressed(VK_SPACE))
		{
			inPickpocketUI = true;
		}
	}
	
	// try adding in the nav 6 slot ver
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
	}

	//------------------------------------------------------------------------
	// Sample Sound.
	//------------------------------------------------------------------------
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_B, true))
	{
		App::PlaySound(".\\TestData\\Test.wav");
	}
}

//------------------------------------------------------------------------
// Add your display calls here (DrawLine,Print, DrawSprite.) 
// See App.h 
//------------------------------------------------------------------------
void Render()
{	
	// background
	background->Draw();

	bool colliding = IsPlayerNearNPC();

	if (colliding) {
		App::Print(10, 100, "Press Space to view inventory", 1.0f, 0.0f, 0.0f);
	}
	//------------------------------------------------------------------------

	// draw pickpocketable npc
	if (npcPickpocketable && !inPickpocketUI)
	{
		rosamund->SetColor(1.0f, 1.0f, 1.0f); //  green
	}
	else
	{
		rosamund->SetColor(1.0f, 1.0f, 1.0f);
	}



	rosamund->Draw();

	// crowd draw
	for (int i = 0; i < NUM_CLUMPS; i++)
	{
		for (int j = 0; j < MEMBERS_PER_CLUMP; j++)
		{
			crowdClumps[i].members[j].sprite->Draw();
		}
	}

	player->Draw();

	randy->Draw();

	if (npcActive)
	{
		roamingNPC->Draw();
	}

	// pickpocket dialogue panel
	if (inPickpocketUI)
	{
		inventory_screen->Draw();

		// draw overlay
		if (activeNPC == rosamund) {
			rosamund_inv_sprite->Draw();
		}
		else if (activeNPC == randy) {
			randy_inv_sprite->Draw();
		}

		// draw cursor at coordinates
		ui_cursor->SetPosition(slotCoords[currentSlot][0], slotCoords[currentSlot][1]);
		ui_cursor->Draw();

		// what is the item
		int itemID = ITEM_NONE;

		if (activeNPC == rosamund) {
			itemID = rosamundLoot[currentSlot];
		}
		else if (activeNPC == randy) {
			itemID = randyLoot[currentSlot];
		}

		// draw overlay
		if (itemID == ITEM_GOLD) {
			icon_gold->Draw();
		}
		else if (itemID == ITEM_FLASHDRIVE) {
			icon_flashdrive->Draw();
		}
		else if (itemID == ITEM_LETTER) {
			icon_letter->Draw();
		}
		// -----------------------------------------

		App::Print(10, 100, "Press W to confirm Pickpocket", 1.0f, 0.0f, 0.0f);

		// add here the second overlay of inventory depending on the npc

		if (!pickpocketRolled &&
			App::GetController().GetLeftThumbStickY() < -0.5f)
		{
			pickpocketRolled = true;

			player->SetAnimation(ANIM_STEAL);

			int roll = rand() % 100;
			pickpocketSuccess = (roll < 65);
		}

		if (pickpocketRolled)
		{
			if (pickpocketSuccess)
				App::Print(420, 200, "Pickpocket SUCCESS");
			else
				App::Print(420, 200, "Pickpocket FAILED");
		}

		// ------------------------------------------------
	}

	// 
	//------------------------------------------------------------------------
	// Example Text.
	//------------------------------------------------------------------------

	//------------------------------------------------------------------------
	// Example Line Drawing.
	//-----------------------0.7-------------------------------------------------
	static float a = 0.0f;
	float r = 1.0f;
	float g = 1.0f;
	float b = 1.0f;
	a += 0.1f;
	for (int i = 0; i < 20; i++)
	{

		float sx = 200 + sinf(a + i * 0.1f)*60.0f;
		float sy = 200 + cosf(a + i * 0.1f)*60.0f;
		float ex = 700 - sinf(a + i * 0.1f)*60.0f;
		float ey = 700 - cosf(a + i * 0.1f)*60.0f;
		g = (float)i / 20.0f;
		b = (float)i / 20.0f;
		//App::DrawLine(sx, sy, ex, ey,r,g,b);
	}
}
//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{	
	//------------------------------------------------------------------------
	// Example Sprite Code....
	delete player;
	delete rosamund;
	delete background;

	for (int i = 0; i < NUM_CLUMPS; i++)
	{
		for (int j = 0; j < MEMBERS_PER_CLUMP; j++)
		{
			delete crowdClumps[i].members[j].sprite;
		}
	}

	delete roamingNPC;
	delete inventory_screen;
	delete rosamund_inv_sprite;
	delete randy_inv_sprite;
	delete icon_gold;
	delete icon_flashdrive;
	delete icon_letter;
	delete ui_cursor;

	//------------------------------------------------------------------------
}
