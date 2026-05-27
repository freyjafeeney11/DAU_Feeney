#include "stdafx.h"
#include "CharismaticMenu.h"
#include <string>
#include <windows.h>

CharismaticMenu::CharismaticMenu() {
    m_background = App::CreateSprite(".\\TestData\\dialogue.png", 1, 1);
    m_background->SetPosition(512.0f, 700.0f);
    m_background->SetScale(0.7f);

    m_dim = App::CreateSprite(".\\TestData\\overlay.png", 1, 1);
    m_dim->SetPosition(512.0f, 512.0f);

    m_typeTimer    = 0.0f;
    m_typeIndex    = 0;
    m_enterDown    = false;
    m_dismissed    = false;
}

CharismaticMenu::~CharismaticMenu() {
    delete m_background;
    delete m_dim;
}

void CharismaticMenu::Show(int livesRemaining) {
    m_dismissed     = false;
    m_typeTimer     = 0.0f;
    m_typeIndex     = 0;
    m_displayedText = "";
    m_enterDown     = true;

    std::string base = "You recite a beautiful string of zeroes and ones so moving\nthat the patroller lets you escape... this time.";
    if (livesRemaining == 1) {
        base += " (1 life left)";
    } else if (livesRemaining == 0) {
        base += " (no lives left)";
    }
    m_fullText = base;
}

void CharismaticMenu::Update(bool& outDismissed) {
    outDismissed = false;

    float dt = 1.0f / 60.0f;

    if (m_typeIndex < (int)m_fullText.size()) {
        m_typeTimer += dt;
        if (m_typeTimer >= TYPE_SPEED) {
            m_typeTimer = 0.0f;
            if (m_fullText[m_typeIndex] == '\n') {
                m_displayedText += '\n';
            } else {
                m_displayedText += m_fullText[m_typeIndex];
            }
            m_typeIndex++;
        }
    }

    bool enterNow = App::IsKeyPressed(VK_RETURN);
    if (enterNow && !m_enterDown) {
        m_dismissed  = true;
        outDismissed = true;
    }
    m_enterDown = enterNow;
}

void CharismaticMenu::Render() {
    m_dim->Draw();
    m_background->Draw();

    size_t nl = m_displayedText.find('\n');
    if (nl == std::string::npos) {
        App::PrintTTF(240, 410, m_displayedText.c_str(), 1.0f, 1.0f, 1.0f, 0);
    } else {
        std::string line1 = m_displayedText.substr(0, nl);
        std::string line2 = (nl + 1 < m_displayedText.size()) ? m_displayedText.substr(nl + 1) : "";
        App::PrintTTF(240, 370, line1.c_str(), 1.0f, 1.0f, 1.0f, 0);
        App::PrintTTF(240, 345, line2.c_str(), 1.0f, 1.0f, 1.0f, 0);
    }

    bool typingDone = (m_typeIndex >= (int)m_fullText.size());
    if (typingDone) {
        App::PrintTTF(455, 335, "Continue", 1.0f, 1.0f, 1.0f, 0);
        App::PrintTTF(430, 335, ">", 0.239f, 0.0f, 0.0f, 1);
    }
}
