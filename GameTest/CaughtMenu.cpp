#include "stdafx.h"
#include "CaughtMenu.h"

CaughtMenu::CaughtMenu() {
    m_background = App::CreateSprite(".\\TestData\\ui_screen.png", 1, 1);
    m_background->SetPosition(512.0f, 460.0f);
    m_background->SetScale(0.6f);
    m_dim = App::CreateSprite(".\\TestData\\overlay.png", 1, 1);
    m_dim->SetPosition(512.0f, 512.0f);
    m_choice = 0;
    m_navDown = false;
    m_enterDown = false;
    App::InitFont(".\\TestData\\fonts\\UncialAntiqua-Regular.ttf", 30.0f);
}

CaughtMenu::~CaughtMenu() {
    delete m_background;
    delete m_dim;
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

    App::PrintTTF(385, 465, "YOU GOT CAUGHT!", 1.0f, 0.2f, 0.2f);

    float retryR = (m_choice == 0) ? 1.0f : 0.45f;
    float quitR = (m_choice == 1) ? 1.0f : 0.45f;

    App::PrintTTF(470, 410.0f, "Retry", retryR, retryR, retryR);
    App::PrintTTF(470, 375.0f, "Quit", quitR, quitR, quitR);

    float cursorY = (m_choice == 0) ? 410.0f : 375.0f;
    App::PrintTTF(448, cursorY, ">", 1.0f, 0.9f, 0.2f);
}