#include "stdafx.h"
#include "MainMenu.h"

MainMenu::MainMenu() {
    m_background = App::CreateSprite(".\\TestData\\start_menu.png", 1, 1);
    m_background->SetPosition(512.0f, 400.0f);
    m_background->SetScale(0.6f);
}

MainMenu::~MainMenu() {
    delete m_background;
}

void MainMenu::Update(float deltaTime) {
}

void MainMenu::Render() {
    m_background->SetPosition(512.0f, 400.0f);
    m_background->Draw();
    App::Print(350, 100, "Press Enter to Start", 1.0f, 1.0f, 1.0f);
}

bool MainMenu::ShouldStart() const {
    return App::IsKeyPressed(VK_RETURN);
}