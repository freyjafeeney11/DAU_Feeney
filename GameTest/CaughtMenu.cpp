#include "stdafx.h"
#include "CaughtMenu.h"

CaughtMenu::CaughtMenu() {
    m_background = App::CreateSprite(".\\TestData\\ui_screen.png", 1, 1);
    m_background->SetPosition(512.0f, 460.0f);
    m_background->SetScale(0.6f);

    m_text = App::CreateSprite(".\\TestData\\caught_menu.png", 1, 1);
    m_text->SetPosition(512.0f, 500.0f);
    m_text->SetScale(0.3f);

    m_dim = App::CreateSprite(".\\TestData\\overlay.png", 1, 1);
    m_dim->SetPosition(512.0f, 512.0f);
    m_choice = 0;
    m_navDown = false;
    m_enterDown = false;
}

CaughtMenu::~CaughtMenu() {
    delete m_background;
    delete m_dim;
    delete m_text;
}

void CaughtMenu::Update(bool& outRetry) {
    outRetry = false;

    if (!m_navDown) {
        if (App::IsKeyPressed(VK_UP)) { m_choice = 0; m_navDown = true; }
        if (App::IsKeyPressed(VK_DOWN)) { m_choice = 1; m_navDown = true; }
    }
    if (!App::IsKeyPressed(VK_UP) && !App::IsKeyPressed(VK_DOWN))
        m_navDown = false;

    if (App::IsKeyPressed(VK_RETURN) && !m_enterDown) {
        m_enterDown = true;
        if (m_choice == 0) outRetry = true;
        else exit(0);
    }
    if (!App::IsKeyPressed(VK_RETURN)) m_enterDown = false;
}

void CaughtMenu::Render() {
    m_dim->Draw();
    m_background->Draw();
    m_text->Draw();

    float retryR = (m_choice == 0) ? 1.0f : 0.45f;
    float quitR = (m_choice == 1) ? 1.0f : 0.45f;

    App::PrintTTF(470, 440.0f, "Retry", retryR, retryR, retryR, 1);
    App::PrintTTF(470, 400.0f, "Quit", quitR, quitR, quitR, 1);

    float cursorY = (m_choice == 0) ? 440.0f : 400.0f;
    App::PrintTTF(448, cursorY, ">", 0.239f, 0.0f, 0.0f, 1);
}