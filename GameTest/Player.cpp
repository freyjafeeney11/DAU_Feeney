// Player.cpp
#include "stdafx.h"
#include "Player.h"

Player::Player() {
    sprite = App::CreateSprite(".\\TestData\\player_sprite.png", 3, 5);
    sprite->SetPosition(400.0f, 250.0f);

    float speed = 1.0f / 7.0f;
    sprite->CreateAnimation(ANIM_IDLE, speed, { 0,1,2 });
    sprite->CreateAnimation(ANIM_WALK, speed, { 3, 4, 5 });
    sprite->CreateAnimation(ANIM_HIDE, speed, { 9, 10, 11 });
    sprite->CreateAnimation(ANIM_STEAL, speed, { 12, 13, 14 });
    sprite->CreateAnimation(ANIM_RUN, speed, { 6, 7, 8 });
    sprite->SetScale(0.15f);
}

Player::~Player() {
    delete sprite;
}

void Player::Update(float deltaTime) {
    if (App::GetController().GetLeftThumbStickX() > 0.5f) {
        bool sprinting = App::IsKeyPressed(VK_SHIFT);
        sprite->SetAnimation(sprinting ? ANIM_RUN : ANIM_WALK);
        sprite->SetFlipX(true);

        float x, y;
        sprite->GetPosition(x, y);
        sprite->SetPosition(x + (sprinting ? RUN_SPEED : WALK_SPEED), y);
    }
    else if (App::GetController().GetLeftThumbStickX() < -0.5f) {
        bool sprinting = App::IsKeyPressed(VK_SHIFT);
        sprite->SetAnimation(sprinting ? ANIM_RUN : ANIM_WALK);
        sprite->SetFlipX(false);

        float x, y;
        sprite->GetPosition(x, y);
        sprite->SetPosition(x - (sprinting ? RUN_SPEED : WALK_SPEED), y);
    }
    else if (App::GetController().GetLeftThumbStickY() > 0.5f) {
        sprite->SetAnimation(ANIM_HIDE);
    }
    else if (App::GetController().GetLeftThumbStickY() < -0.5f) {
        sprite->SetAnimation(ANIM_STEAL);
    }
    else {
        sprite->SetAnimation(ANIM_IDLE);
    }

    sprite->Update(deltaTime);
}

void Player::Render(float camX, float camY) {
    float actualX, actualY;
    sprite->GetPosition(actualX, actualY);

    sprite->SetPosition(actualX - camX, actualY - camY);
    sprite->Draw();

    sprite->SetPosition(actualX, actualY);
}

void Player::GetPosition(float& x, float& y) {
    sprite->GetPosition(x, y);
}