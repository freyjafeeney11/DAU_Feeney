// Player.cpp
#include "stdafx.h"
#include "Player.h"
#include "UserControls.h"

Player::Player() {
    m_sprite = App::CreateSprite(".\\TestData\\sprite_sheet 9.png", 4, 4);
    m_sprite->SetPosition(400.0f, 250.0f);

    float speed = 1.0f / 8.0f;
    m_sprite->CreateAnimation(ANIM_IDLE, speed, { 5,6,7,8 });
    m_sprite->CreateAnimation(ANIM_WALK, speed, { 0, 1, 2, 3, 4 });
    m_sprite->CreateAnimation(ANIM_HIDE, speed, { 9, 10, 11 });
    m_sprite->CreateAnimation(ANIM_STEAL, speed, { 12, 13, 14 });
    m_sprite->CreateAnimation(ANIM_RUN, speed, { 9, 10, 11, 12, 13, 14, 15 });
    m_sprite->SetScale(0.2f);
}

Player::~Player() {
    delete m_sprite;
}

void Player::Update(float deltaTime) {
    if (UserControls::IsUserGoingRight()) {
        bool sprinting = UserControls::IsUserSprinting();
        m_sprite->SetAnimation(sprinting ? ANIM_RUN : ANIM_WALK);
        m_sprite->SetFlipX(false);

        float x, y;
        m_sprite->GetPosition(x, y);
        m_sprite->SetPosition(x + (sprinting ? ms_RUN_SPEED : ms_WALK_SPEED), y);
    }
    else if (UserControls::IsUserGoingLeft()) {
        bool sprinting = UserControls::IsUserSprinting();
        m_sprite->SetAnimation(sprinting ? ANIM_RUN : ANIM_WALK);
        m_sprite->SetFlipX(true);

        float x, y;
        m_sprite->GetPosition(x, y);
        m_sprite->SetPosition(x - (sprinting ? ms_RUN_SPEED : ms_WALK_SPEED), y);
    }
    else if (UserControls::IsUserGoingUp()) {
        m_sprite->SetAnimation(ANIM_HIDE);
    }
    else if (UserControls::IsUserGoingDown()) {
        m_sprite->SetAnimation(ANIM_STEAL);
    }
    else {
        m_sprite->SetAnimation(ANIM_IDLE);
    }

    m_sprite->Update(deltaTime);
}

void Player::Render(float camX, float camY) {
    float actualX, actualY;
    m_sprite->GetPosition(actualX, actualY);

    m_sprite->SetPosition(actualX - camX, actualY - camY);
    m_sprite->Draw();

    m_sprite->SetPosition(actualX, actualY);
}

void Player::GetPosition(float& x, float& y) {
    m_sprite->GetPosition(x, y);
}