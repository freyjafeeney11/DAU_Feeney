
#include "stdafx.h"
#include "Player.h"
#include "UserControls.h"
#include "PlayerArchetype.h"

Player::Player() {
    m_isHiding = false;
    m_sprite = App::CreateSprite(".\\TestData\\sprite_sheet 9.png", 4, 4);
    m_sprite->SetPosition(400.0f, 245.0f);
    m_hideSprite = App::CreateSprite(".\\TestData\\player_hide.png", 2, 1);
    m_hideSprite->SetPosition(400.0f, 250.0f);
    m_hideSprite->SetScale(0.2f);

    float speed = 1.0f / 8.0f;
    m_sprite->CreateAnimation(ANIM_IDLE, speed, { 5,6,7,8 });
    m_sprite->CreateAnimation(ANIM_WALK, speed, { 0, 1, 2, 3, 4 });
    m_hideSprite->CreateAnimation(ANIM_HIDE, speed, { 0, 1 });
    m_sprite->CreateAnimation(ANIM_STEAL, speed, { 12, 13, 14 });
    m_sprite->CreateAnimation(ANIM_RUN, speed, { 9, 10, 11, 12, 13, 14, 15 });
    m_sprite->SetScale(0.2f);
}

Player::~Player() {
    delete m_sprite;
    delete m_hideSprite;
}

void Player::Update(float deltaTime) {
    m_isHiding = false;
    if (UserControls::IsUserGoingRight()) {
        bool sprinting = UserControls::IsUserSprinting();
        m_sprite->SetAnimation(sprinting ? ANIM_RUN : ANIM_WALK);
        m_sprite->SetFlipX(false);
        bool dex = (PlayerArchetype::current == Archetype::DEXTEROUS);
        float walk = dex ? ms_WALK_SPEED * 1.45f : ms_WALK_SPEED;
        float run  = dex ? ms_RUN_SPEED  * 1.45f : ms_RUN_SPEED;
        float x, y;
        m_sprite->GetPosition(x, y);
        m_sprite->SetPosition(x + (sprinting ? run : walk), y);
    }
    else if (UserControls::IsUserGoingLeft()) {
        bool sprinting = UserControls::IsUserSprinting();
        m_sprite->SetAnimation(sprinting ? ANIM_RUN : ANIM_WALK);
        m_sprite->SetFlipX(true);
        bool dex = (PlayerArchetype::current == Archetype::DEXTEROUS);
        float walk = dex ? ms_WALK_SPEED * 1.45f : ms_WALK_SPEED;
        float run  = dex ? ms_RUN_SPEED  * 1.45f : ms_RUN_SPEED;
        float x, y;
        m_sprite->GetPosition(x, y);
        m_sprite->SetPosition(x - (sprinting ? run : walk), y);
    }
    else if (UserControls::IsUserGoingDown()) {
        m_isHiding = true;
        m_hideSprite->SetAnimation(ANIM_HIDE);
    }
    else {
        m_sprite->SetAnimation(ANIM_IDLE);
    }

    m_sprite->Update(deltaTime);
    m_hideSprite->Update(deltaTime);
}

void Player::Render(float camX, float camY, bool hidden = false) {
    float actualX, actualY;
    CSimpleSprite* activeSprite = m_isHiding ? m_hideSprite : m_sprite;
    m_sprite->GetPosition(actualX, actualY);
    m_hideSprite->SetPosition(actualX, actualY);

    if (hidden) {
        activeSprite->SetColor(0.4f, 0.4f, 0.4f);
    }
    else {
        activeSprite->SetColor(1.0f, 1.0f, 1.0f);
    }
    activeSprite->SetPosition(actualX - camX, actualY - camY);
    activeSprite->Draw();

    activeSprite->SetPosition(actualX, actualY);
}

void Player::GetPosition(float& x, float& y) {
    m_sprite->GetPosition(x, y);
}
void Player::SetPosition(float x, float y) {
    m_sprite->SetPosition(x, y);
}