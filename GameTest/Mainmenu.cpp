#include "stdafx.h"
#include "MainMenu.h"

MainMenu::MainMenu() {
    m_background = App::CreateSprite(".\\TestData\\start_menu.png", 1, 1);
    m_background->SetPosition(512.0f, 400.0f);
    m_background->SetScale(0.5f);
    
    m_cursor = App::CreateSprite(".\\TestData\\mask_temp.png", 1, 1);
    m_cursor->SetScale(0.04f);
    
    m_choice = 0;
    m_navDown = false;
    m_startSelected = false;
    m_exitSelected = false;
}

MainMenu::~MainMenu() {
    delete m_background;
    delete m_cursor;
}

void MainMenu::Update(float deltaTime) {
    if (!m_navDown) {
        if (App::IsKeyPressed(VK_UP) || App::IsKeyPressed(VK_DOWN)) {
            m_choice = 1 - m_choice;
            m_navDown = true;
            App::PlaySound(".\\TestData\\audio\\confirm.wav", false);
            App::SetSoundVolume(".\\TestData\\audio\\confirm.wav", 0.4f);
        }
    }
    if (!App::IsKeyPressed(VK_UP) && !App::IsKeyPressed(VK_DOWN)) {
        m_navDown = false;
    }
    
    if (App::IsKeyPressed(VK_RETURN)) {
        if (m_choice == 0) {
            m_startSelected = true;
        } else {
            m_exitSelected = true;
        }
        App::PlaySound(".\\TestData\\audio\\menu_open.wav", false);
        App::SetSoundVolume(".\\TestData\\audio\\menu_open.wav", 0.4f);
    }
}

void MainMenu::Render() {
    m_background->SetPosition(512.0f, 400.0f);
    m_background->Draw();
    
    float cursorX = 220.0f;
    float cursorY = (m_choice == 0) ? 460.0f : 405.0f;
    
    static float t = 0.0f;
    t += 0.05f;
    float bob = sinf(t) * 4.0f;

    m_cursor->SetPosition(cursorX + bob, cursorY);
    m_cursor->Draw();
}