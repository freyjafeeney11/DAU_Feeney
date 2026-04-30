#include "stdafx.h"
#include "PauseMenu.h"

PauseMenu::PauseMenu() {
    m_background = App::CreateSprite(".\\TestData\\ui_screen.png", 1, 1);
    m_background->SetPosition(512.0f, 512.0f);
    m_background->SetScale(0.5f);

    m_text = App::CreateSprite(".\\TestData\\pause_menu.png", 1, 1);
    m_text->SetPosition(512.0f, 560.0f);
    m_text->SetScale(0.3f);

    m_dim = App::CreateSprite(".\\TestData\\overlay.png", 1, 1);
    m_choice = 0;
    m_navDown = false;
    m_enterDown = false;
}

PauseMenu::~PauseMenu() {
    delete m_background;
    delete m_dim;
    delete m_text;
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
    m_text->Draw();

    float resumeR = (m_choice == 0) ? 1.0f : 0.5f;
    float quitR = (m_choice == 1) ? 1.0f : 0.5f;

    App::PrintTTF(470, 500.0f, "Resume", resumeR, resumeR, resumeR, 0);
    App::PrintTTF(470, 470.0f, "Quit", quitR, quitR, quitR, 0);

    float cursorY = (m_choice == 0) ? 500.0f : 470.0f;
    App::PrintTTF(440, cursorY, ">", 0.239f, 0.0f, 0.0f, 1);
}