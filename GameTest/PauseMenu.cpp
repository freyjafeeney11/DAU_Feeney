#include "stdafx.h"
#include "PauseMenu.h"

PauseMenu::PauseMenu() {
    m_background = App::CreateSprite(".\\TestData\\ui_screen.png", 1, 1);
    m_background->SetPosition(512.0f, 512.0f);
    m_background->SetScale(0.4f);

    m_dim = App::CreateSprite(".\\TestData\\overlay.png", 1, 1);
    m_choice = 0;
    m_navDown = false;
    m_enterDown = false;
}

PauseMenu::~PauseMenu() {
    delete m_background;
    delete m_dim;
}

void PauseMenu::Update(bool& isPaused) {
    if (!m_navDown) {
        if (App::IsKeyPressed(VK_UP)) {
            m_choice = 0;
            m_navDown = true;
        }
        if (App::IsKeyPressed(VK_DOWN)) {
            m_choice = 1;
            m_navDown = true;
        }
    }
    if (!App::IsKeyPressed(VK_UP) && !App::IsKeyPressed(VK_DOWN)) {
        m_navDown = false;
    }

    if (App::IsKeyPressed(VK_RETURN) && !m_enterDown) {
        m_enterDown = true;
        if (m_choice == 0) {
            isPaused = false;
        }
        else {
            exit(0);
        }
    }
    if (!App::IsKeyPressed(VK_RETURN)) m_enterDown = false;
}

void PauseMenu::Render() {
    m_dim->SetPosition(512.0f, 512.0f);
    m_dim->Draw();
    m_background->Draw();
    App::PrintTTF(470, 480, "PAUSED", 1.0f, 1.0f, 1.0f);

    float resumeR = (m_choice == 0) ? 1.0f : 0.5f;
    float quitR = (m_choice == 1) ? 1.0f : 0.5f;

    App::PrintTTF(470, 450.0f, "Resume", resumeR, resumeR, resumeR);
    App::PrintTTF(470, 420.0f, "Quit", quitR, quitR, quitR);

    float cursorY = (m_choice == 0) ? 450.0f : 420.0f;
    App::PrintTTF(440, cursorY, ">", 1.0f, 1.0f, 0.0f);
}