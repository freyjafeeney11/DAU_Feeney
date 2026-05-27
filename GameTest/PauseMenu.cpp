#include "stdafx.h"
#include "PauseMenu.h"

PauseMenu::PauseMenu() {
    m_background = App::CreateSprite(".\\TestData\\ui_screen.png", 1, 1);
    m_background->SetPosition(512.0f, 512.0f);
    m_background->SetScale(0.5f);

    m_controlsBg = App::CreateSprite(".\\TestData\\ui_screen.png", 1, 1);
    m_controlsBg->SetPosition(512.0f, 512.0f);
    m_controlsBg->SetScale(0.8f);

    m_text = App::CreateSprite(".\\TestData\\pause_menu.png", 1, 1);
    m_text->SetPosition(512.0f, 560.0f);
    m_text->SetScale(0.3f);

    m_dim = App::CreateSprite(".\\TestData\\overlay.png", 1, 1);
    m_choice = 0;
    m_navDown = false;
    m_enterDown = false;
    m_showControlsTab = false;
}

PauseMenu::~PauseMenu() {
    delete m_background;
    delete m_controlsBg;
    delete m_dim;
    delete m_text;
}

void PauseMenu::Update(bool& isPaused) {
    if (m_showControlsTab) {
        if (App::IsKeyPressed(VK_ESCAPE) || (App::IsKeyPressed(VK_RETURN) && !m_enterDown)) {
            m_showControlsTab = false;
            m_enterDown = true;
        }
        if (!App::IsKeyPressed(VK_RETURN)) m_enterDown = false;
        return;
    }

    if (!m_navDown) {
        if (App::IsKeyPressed(VK_UP)) {
            m_choice = (m_choice + 2) % 3;
            m_navDown = true;
        }
        if (App::IsKeyPressed(VK_DOWN)) {
            m_choice = (m_choice + 1) % 3;
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
        else if (m_choice == 1) {
            m_showControlsTab = true;
        }
        else if (m_choice == 2) {
            exit(0);
        }
    }
    if (!App::IsKeyPressed(VK_RETURN)) m_enterDown = false;
    if (App::IsKeyPressed('Q')) exit(0);
}

void PauseMenu::Render() {
    m_dim->SetPosition(512.0f, 512.0f);
    m_dim->Draw();

    if (m_showControlsTab) {
        m_controlsBg->Draw();
        float cx = 350.0f;
        App::PrintTTF(460, 680, "Controls", 0.239f, 0.0f, 0.0f, 1);
        App::PrintTTF(cx, 630, "Arrows  - Move", 0.8f, 0.8f, 0.8f, 0);
        App::PrintTTF(cx, 590, "Shift   - Sprint", 0.8f, 0.8f, 0.8f, 0);
        App::PrintTTF(cx, 550, "Down    - Hide in crowd", 0.8f, 0.8f, 0.8f, 0);
        App::PrintTTF(cx, 510, "Up      - Climb ladder", 0.8f, 0.8f, 0.8f, 0);
        App::PrintTTF(cx, 470, "Enter   - Pickpocket / Interact", 0.8f, 0.8f, 0.8f, 0);
        App::PrintTTF(cx, 430, "I       - Open inventory", 0.8f, 0.8f, 0.8f, 0);
        App::PrintTTF(cx, 390, "ESC     - Close menu / Pause", 0.8f, 0.8f, 0.8f, 0);
                return;
    }

    m_background->Draw();
    m_text->Draw();

    float resumeR   = (m_choice == 0) ? 1.0f : 0.5f;
    float controlsR = (m_choice == 1) ? 1.0f : 0.5f;
    float quitR     = (m_choice == 2) ? 1.0f : 0.5f;

    App::PrintTTF(470, 500.0f, "Resume", resumeR, resumeR, resumeR, 0);
    App::PrintTTF(470, 470.0f, "Controls", controlsR, controlsR, controlsR, 0);
    App::PrintTTF(470, 440.0f, "Quit", quitR, quitR, quitR, 0);

    float cursorY = 500.0f;
    if (m_choice == 1) cursorY = 470.0f;
    if (m_choice == 2) cursorY = 440.0f;

    App::PrintTTF(440, cursorY, ">", 0.239f, 0.0f, 0.0f, 1);
}