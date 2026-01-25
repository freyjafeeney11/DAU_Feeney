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

//------------------------------------------------------------------------
// Eample data....
//------------------------------------------------------------------------
CSimpleSprite *player;
// metro bg
CSimpleSprite* background;
CSimpleSprite* npc;        // NPC
bool canRob = true;  // can npc be pickpocketed
const float WALK_SPEED = 1.0f;
const float RUN_SPEED = 2.5f;



enum
{
	ANIM_IDLE,
	ANIM_HIDE,
	ANIM_WALK,
	ANIM_STEAL,
	ANIM_RUN
};
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
	// background
	background = App::CreateSprite(".\\TestData\\background.png", 1, 1);
	background->SetPosition(500.0f, 400.0f);
	background->SetScale(0.6f);
	//------------------------------------------------------------------------
	// npc
	npc = App::CreateSprite(".\\TestData\\test_npc.png", 4, 1); // 4 frame sprite, 1 row
	npc->SetPosition(450.0f, 250.0f);
	npc->SetScale(2.0f);
	npc->CreateAnimation(0, 0.2f, { 0,1,2,3 }); // idle anim
	npc->SetAnimation(0);
	// 
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
	// Example Sprite Code....
	// update npc
	npc->Update(deltaTime);
	player->Update(deltaTime);
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
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_UP, false))
	{
		player->SetScale(player->GetScale() + 0.1f);
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_DOWN, false))
	{
		player->SetScale(player->GetScale() - 0.1f);
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_LEFT, false))
	{
		player->SetAngle(player->GetAngle() + 0.1f);
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_RIGHT, false))
	{
		player->SetAngle(player->GetAngle() - 0.1f);
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_A, true))
	{
		player->SetAnimation(-1);
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
	//------------------------------------------------------------------------
	// Example Sprite Code....
	npc->Draw();
	player->Draw();
	//------------------------------------------------------------------------

	//------------------------------------------------------------------------
	// Example Text.
	//------------------------------------------------------------------------
	App::Print(100, 100, "A career concious burglary student...");

	//------------------------------------------------------------------------
	// Example Line Drawing.
	//------------------------------------------------------------------------
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
	delete npc;
	delete background;
	//------------------------------------------------------------------------
}