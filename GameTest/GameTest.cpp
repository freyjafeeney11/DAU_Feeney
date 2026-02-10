#include "stdafx.h"
//------------------------------------------------------------------------
#include <windows.h> 
#include <math.h>  
//------------------------------------------------------------------------
#include "app\app.h"
#include <string>
#include <ctime>
//------------------------------------------------------------------------
// INVENTORY STUFF
CSimpleSprite* inventory_screen;
CSimpleSprite* window;
CSimpleSprite* rain;
// overlays
CSimpleSprite* rosamund_inv_sprite;
CSimpleSprite* randy_inv_sprite;
CSimpleSprite* granny_inv_sprite;
CSimpleSprite* alertIcon;

// navigation
CSimpleSprite* icon_gold;
CSimpleSprite* icon_letter;
CSimpleSprite* icon_flashdrive;
CSimpleSprite* icon_picture;

// smaller icons
CSimpleSprite* icon_gold_small;
CSimpleSprite* icon_letter_small;
CSimpleSprite* icon_flashdrive_small;
CSimpleSprite* icon_picture_small;


CSimpleSprite* ui_cursor;
int currentSlot = 0;
bool navButtonDown = false;

// inventory 
struct Item {
	int id;
	std::string name;
	std::string flavorText;
	int value;
};
// npc struct

struct NPCData {
	CSimpleSprite* sprite;
	std::string name;
	int* lootTable;
	bool isAlerted = false;
	int difficulty;
};

// list of Npcs
std::vector<NPCData> allNPCs;

// players actual inventory
std::vector<Item> playerInventory;

// items available
enum { ITEM_NONE, ITEM_GOLD, ITEM_FLASHDRIVE, ITEM_LETTER, ITEM_PICTURE};

// loot tables
int rosamundLoot[6] = { ITEM_GOLD, ITEM_LETTER, ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE };
int grannyLoot[6] = { ITEM_GOLD, ITEM_PICTURE, ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE };
int randyLoot[6] = { ITEM_GOLD, ITEM_FLASHDRIVE, ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE };

// coords
float slotCoords[6][2] = {
	{ 190.0f, 600.0f },
	{ 310.0f, 600.0f },
	{ 450.0f, 600.0f },
	{ 190.0f, 450.0f },
	{ 310.0f, 450.0f },
	{ 450.0f, 450.0f } 
};

//-------------------

CSimpleSprite *player;
// metro bg
CSimpleSprite* background;
CSimpleSprite* rosamund;
CSimpleSprite* granny;
CSimpleSprite* randy;
// crowd clumps 
const int NUM_CLUMPS = 2;
const int MEMBERS_PER_CLUMP = 5;

// pickpocket stuff (steal mech)
int lastDiceRoll = 0;
bool showDiceResult = false;
bool enterButtonDown = false;
bool lastStealSuccess = false;
bool pickpocketRolled = false;
bool canRob = true;
const float WALK_SPEED = 2.0f;
const float RUN_SPEED = 4.0f;
// ------------------------------

// moving npc

CSimpleSprite* roamingNPC = nullptr;

bool npcActive = false;
float npcTimer = 0.0f;
float npcSpawnDelay = 3.0f;
float npcSpeed = 2.5f;
bool npcMoveRight = true;

// pickpocket globals
CSimpleSprite* npcPortrait;
bool npcPickpocketable = true;
bool inPickpocketUI = false;
bool pickpocketSuccess = false;

// NEW COLLISION GLOBALS
std::vector<CSimpleSprite*> npcList;
CSimpleSprite* activeNPC = nullptr;
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

//------------------------------------------------------------------------'helper functions

NPCData* GetNPCDataFromSprite(CSimpleSprite* sprite)
{
	for (auto& npc : allNPCs)
	{
		if (npc.sprite == sprite)
			return &npc;
	}
	return nullptr;
}

void DrawAlertIconAboveNPC(CSimpleSprite* npc)
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
		x,
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

void Init()
{
	srand((unsigned int)time(nullptr));

	// make npc pickpocketable
	npcPickpocketable = true;
	// background
	background = App::CreateSprite(".\\TestData\\Train_bg.png", 1, 1);
	background->SetPosition(500.0f, 400.0f);
	background->SetScale(0.6f);

	// windows
	window = App::CreateSprite(".\\TestData\\cityscape.png", 7, 1);
	window->SetPosition(470.0f, 450.0f);
	window->SetScale(0.55f);
	window->CreateAnimation(0, 0.5f, { 0, 1, 2, 3, 4, 5, 6 });
	window->SetAnimation(0);

	// rain
	rain = App::CreateSprite(".\\TestData\\rain.png", 3, 1);
	rain->SetPosition(475.0f, 450.0f);
	rain->SetScale(0.5f);
	rain->CreateAnimation(0, 0.2f, { 0, 1, 2 });
	rain->SetAnimation(0);

	//inventory
	inventory_screen = App::CreateSprite(".\\TestData\\Inventory.png", 1, 1);
	inventory_screen->SetPosition(500.0f, 400.0f);
	inventory_screen->SetScale(0.6f);

	//overlays
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

	// inventory slot items
	// inventory items detailed view
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

	// inventory items detailed view
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

	// temporary cursor
	ui_cursor = App::CreateSprite(".\\TestData\\mask_temp.png", 1, 1);
	ui_cursor->SetScale(0.4f);

	// pickpocket dialogue panel
	npcPortrait = App::CreateSprite(".\\TestData\\pickpocket_dialogue.png", 1, 1);
	npcPortrait->SetPosition(500.0f, 400.0f);
	npcPortrait->SetScale(0.6f);

	// moving npc
	roamingNPC = App::CreateSprite(".\\TestData\\npc_walk.png", 4, 1);
	roamingNPC->SetScale(0.2f);
	roamingNPC->CreateAnimation(0, 0.2f, { 0,1,2,3 });
	roamingNPC->SetAnimation(0);

	// start off screen
	roamingNPC->SetPosition(-200.0f, -200.0f);

	// rosamund
	rosamund = App::CreateSprite(".\\TestData\\IMG_1297.png", 1, 1); // 4 frame sprite, 1 row
	rosamund->SetPosition(600.0f, 340.0f);
	rosamund->SetScale(0.5f);
	rosamund->CreateAnimation(0, 0.2f, { 0,1,2,3 }); // idle anim
	rosamund->SetAnimation(0);

	NPCData ros;
	ros.sprite = rosamund;
	ros.name = "Rosamund";
	ros.lootTable = rosamundLoot;
	ros.difficulty = 11;
	allNPCs.push_back(ros);

	// granny
	granny = App::CreateSprite(".\\TestData\\granny_idle.png", 4, 1); // 4 frame sprite, 1 row
	granny->SetPosition(695.0f, 340.0f);
	granny->SetScale(0.16f);
	granny->CreateAnimation(0, 0.4f, { 0,1,2,3 }); // idle animf
	granny->SetAnimation(0);

	NPCData gran;
	gran.sprite = granny;
	gran.name = "Granny";
	gran.lootTable = grannyLoot;
	gran.difficulty = 8;
	gran.isAlerted = false;
	allNPCs.push_back(gran);

	// randy
	randy = App::CreateSprite(".\\TestData\\randy_idle.png", 4, 1); // 4 frame sprite, 1 row
	randy->SetPosition(350.0f, 340.0f);
	randy->SetScale(0.2f);
	randy->CreateAnimation(0, 0.4f, { 0,1,2,3 }); // idle anim coming soon
	randy->SetAnimation(0);

	NPCData ran;
	ran.sprite = randy;
	ran.name = "Randy";
	ran.lootTable = randyLoot;
	ran.difficulty = 14;
	ran.isAlerted = false;
	allNPCs.push_back(ran);


	// adding npcs to collision list
	npcList.push_back(rosamund);
	npcList.push_back(randy);
	npcList.push_back(granny);

	// crowd clumps 

	float baseX = 200.0f;
	float spacing = 250.0f;

	for (int i = 0; i < NUM_CLUMPS; i++)
	{
		crowdClumps[i].baseX = baseX + i * spacing;
		crowdClumps[i].baseY = 330.0f;
		crowdClumps[i].swayOffset = (rand() % 100) / 100.0f * 6.28f;

		for (int j = 0; j < MEMBERS_PER_CLUMP; j++)
		{
			CrowdMember& m = crowdClumps[i].members[j];

			m.sprite = App::CreateSprite(".\\TestData\\IMG_1297.png", 1, 1);
			m.sprite->SetScale(0.5f);

			m.sprite->SetFlipX((rand() % 2) == 0);

			// spread people inside the clump
			m.offsetX = (rand() % 41) - 30;
			m.offsetY = (rand() % 31) - 20;

			m.personalSwayOffset = (rand() % 100) / 100.0f * 6.28f;
		}
	}

	player = App::CreateSprite(".\\TestData\\player_sprite.png", 3,5);
	player->SetPosition(400.0f, 250.0f);
	float speed = 1.0f / 7.0f;
	player->CreateAnimation(ANIM_IDLE, speed, { 0,1,2 });
	player->CreateAnimation(ANIM_WALK, speed, { 3, 4, 5 });
	player->CreateAnimation(ANIM_HIDE, speed, { 9, 10, 11 });
	player->CreateAnimation(ANIM_STEAL, speed, { 12, 13, 14 });
	player->CreateAnimation(ANIM_RUN, speed, { 6, 7, 8 });
	player->SetScale(0.15f);
}

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
	randy->Update(deltaTime);
	granny->Update(deltaTime);
	window->Update(deltaTime);
	rain->Update(deltaTime);
	bool nearNPC = IsPlayerNearNPC();

	// close inventory ui if not near anymore
	if (!nearNPC && inPickpocketUI)
	{
		inPickpocketUI = false;
		pickpocketRolled = false;
	}

	// pickpocket UI
	if (nearNPC && npcPickpocketable && !inPickpocketUI)
	{
		NPCData* npcData = GetNPCDataFromSprite(activeNPC);

		// dont let player steal from npcs
		if (npcData && npcData->isAlerted)
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

		// steal with enter
		if (App::IsKeyPressed(VK_RETURN) && !enterButtonDown)
		{
			enterButtonDown = true;

			NPCData* npcData = GetNPCDataFromSprite(activeNPC);
			if (!npcData) return;

			int* currentTable = npcData->lootTable;
			int difficulty = npcData->difficulty;

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

					// FAIL CONSEQUENCES
					npcData->isAlerted = true;
					inPickpocketUI = false;
				}

				showDiceResult = true;
			}

		}

		// Reset enter button
		if (!App::IsKeyPressed(VK_RETURN))
		{
			enterButtonDown = false;
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

void Render()
{	
	// background
	window->Draw();
	rain->Draw();
	background->Draw();

	bool colliding = (activeNPC != nullptr);

	if (colliding && !inPickpocketUI) {
		App::Print(10, 100, "Press Space to view inventory", 0.0f, 0.0f, 0.0f);
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

	if (App::IsKeyPressed('I'))
	{
		App::Print(100, 700, "--- YOUR INVENTORY ---", 1.0f, 1.0f, 1.0f);

		for (int i = 0; i < playerInventory.size(); i++) {
			float yOffset = 650.0f - (i * 30.0f);
			App::Print(100, yOffset, playerInventory[i].name.c_str(), 1.0f, 1.0f, 1.0f);
			//if (i == playerInventory.size() - 1) {
			//	App::Print(400, 400, playerInventory[i].flavorText.c_str(), 0.7f, 0.7f, 1.0f);
			//}
		}
	}

	for (int i = 0; i < NUM_CLUMPS; i++) {
		for (int j = 0; j < MEMBERS_PER_CLUMP; j++) {
			if (crowdClumps[i].members[j].offsetY < 0) {
				crowdClumps[i].members[j].sprite->Draw();
			}
		}
	}

	player->Draw();

	for (int i = 0; i < NUM_CLUMPS; i++) {
		for (int j = 0; j < MEMBERS_PER_CLUMP; j++) {
			if (crowdClumps[i].members[j].offsetY >= 0) {
				crowdClumps[i].members[j].sprite->Draw();
			}
		}
	}

	granny->Draw();

	randy->Draw();

	if (npcActive)
	{
		roamingNPC->Draw();
	}

	for (auto& npc : allNPCs)
	{
		if (npc.isAlerted)
		{
			DrawAlertIconAboveNPC(npc.sprite);
		}
	}

	// pickpocket dialogue panel
	if (inPickpocketUI)
	{
		inventory_screen->Draw();

		if (activeNPC == rosamund) {
			rosamund_inv_sprite->Draw();
		}
		else if (activeNPC == randy) {
			randy_inv_sprite->Draw();
		}
		else if (activeNPC == granny) {
			granny_inv_sprite->Draw();
		}

		// which table
		// should improve this system
		int* currentTable = nullptr;
		if (activeNPC == rosamund) currentTable = rosamundLoot;
		else if (activeNPC == randy) currentTable = randyLoot;
		else if (activeNPC == granny) currentTable = grannyLoot;

		if (currentTable != nullptr)
		{
			for (int i = 0; i < 6; i++)
			{
				int itemID = currentTable[i];
				if (itemID == ITEM_GOLD) {
					icon_gold_small->Draw();
				}
				else if (itemID == ITEM_FLASHDRIVE) {
					icon_flashdrive_small->Draw();
				}
				else if (itemID == ITEM_LETTER) {
					icon_letter_small->Draw();
				}
				else if (itemID == ITEM_PICTURE) {
					icon_picture_small->Draw();
				}
			}
		}

		// cursor
		ui_cursor->SetPosition(slotCoords[currentSlot][0], slotCoords[currentSlot][1]);
		ui_cursor->Draw();
		if (currentTable != nullptr)
		{
			int selectedItem = currentTable[currentSlot];

			if (selectedItem == ITEM_GOLD) {
				icon_gold->Draw();
			}
			else if (selectedItem == ITEM_FLASHDRIVE) {
				icon_flashdrive->Draw();
			}
			else if (selectedItem == ITEM_LETTER) {
				icon_letter->Draw();
			}
			else if (selectedItem == ITEM_PICTURE) {
				icon_picture->Draw();
			}
		}

		// draw results of die roll
		if (showDiceResult)
		{
			char resultText[100];
			char resultText1[100];
			if (lastStealSuccess)
			{
				sprintf(resultText, "%d", lastDiceRoll);
				sprintf(resultText1, "Success!");
				App::Print(925, 275, resultText, 0.0f, 0.0f, 0.0f); // Green
				App::Print(890, 190, resultText1, 0.0f, 1.0f, 0.0f); // Green
			}
			else
			{
				sprintf(resultText, "%d", lastDiceRoll);
				sprintf(resultText1, "Failure");
				App::Print(925, 275, resultText, 0.0f, 0.0f, 0.0f); 
				App::Print(890, 190, resultText1, 1.0f, 0.0f, 0.0f);
			}
		}
		else
		{
			// Default instruction if no result showing
			App::Print(10, 100, "Press Enter to Steal", 0.0f, 0.0f, 0.0f);
		}
			NPCData* npcData = GetNPCDataFromSprite(activeNPC);
	if (npcData && npcData->isAlerted)
	{
		App::Print(450, 400, "FAIL !", 1.0f, 0.0f, 0.0f);
	}
	}
}

void Shutdown()
{	
	delete player;
	delete window;
	delete rain;
	delete rosamund;
	delete granny;
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
	delete icon_gold_small;
	delete icon_flashdrive_small;
	delete icon_letter_small;
	delete ui_cursor;
	delete alertIcon;
}
