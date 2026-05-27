#include "stdafx.h"
#include "Intro.h"
#include <windows.h>
#include <math.h>

Intro::Intro() {
    m_hypnotize = App::CreateSprite(".\\TestData\\hypnotize.png", 4, 1);
    m_hypnotize->SetPosition(512.0f, 400.0f);
    m_hypnotize->SetScale(0.6f);
    m_hypnotize->CreateAnimation(0, 0.5f, { 0, 1, 2, 3 });
    m_hypnotize->SetAnimation(0);

    m_slides[0] = App::CreateSprite(".\\TestData\\train_fall_1.png", 1, 1);
    m_slides[1] = App::CreateSprite(".\\TestData\\train_fall_2.png", 1, 1);
    m_slides[2] = App::CreateSprite(".\\TestData\\train_fall_3.png", 1, 1);

    for (int i = 0; i < 3; i++) {
        m_slides[i]->SetPosition(512.0f, 400.0f);
        m_slides[i]->SetScale(0.6f);
    }

    m_dialogueBox = App::CreateSprite(".\\TestData\\dialogue.png", 1, 1);
    m_dialogueBox->SetPosition(512.0f, 680.0f);
    m_dialogueBox->SetScale(0.6f);

    m_textIndex = 0;
    m_enterDown = false;

    StartLine(0);
}

Intro::~Intro() {
    delete m_hypnotize;
    for (int i = 0; i < 3; i++) {
        delete m_slides[i];
    }
    delete m_dialogueBox;
}

void Intro::StartLine(int index) {
    if (index < 5) {
        m_fullText = m_lines[index];
        m_displayedText = "";
        m_typeTimer = 0.0f;
        m_typeIndex = 0;
    }
}

void Intro::Update(float deltaTime) {
    if (m_textIndex < 5) {
        if (m_typeIndex < (int)m_fullText.size()) {
            m_typeTimer += deltaTime / 1000.0f;
            if (m_typeTimer >= TYPE_SPEED) {
                m_typeTimer = 0.0f;
                m_displayedText += m_fullText[m_typeIndex];
                m_typeIndex++;
            }
        }
    }

    if (m_textIndex == 0) {
        m_hypnotize->Update(deltaTime);
    }

    bool enterNow = App::IsKeyPressed(VK_RETURN);
    if (enterNow && !m_enterDown) {
        if (m_typeIndex < (int)m_fullText.size()) {
            m_typeIndex = (int)m_fullText.size();
            m_displayedText = m_fullText;
        } else {
            m_textIndex++;
            StartLine(m_textIndex);
        }
    }
    m_enterDown = enterNow;
}

void Intro::Render() {
    if (m_textIndex == 0) {
        m_hypnotize->Draw();
    } else if (m_textIndex == 1) {
        m_slides[0]->Draw();
    } else if (m_textIndex == 2) {
        m_slides[1]->Draw();
    } else if (m_textIndex >= 3) {
        m_slides[2]->Draw();
    }

    m_dialogueBox->SetPosition(500.0f, 980.0f);
    m_dialogueBox->Draw();
    App::PrintTTF(174, 710, m_displayedText.c_str(), 1.0f, 1.0f, 1.0f, 0);

    if (m_typeIndex >= (int)m_fullText.size() && m_textIndex < 5) {
        static float t = 0.0f;
        t += 0.05f;
        float pulse = 0.5f + 0.5f * sinf(t);
        App::PrintTTF(760, 675, "Press Enter", pulse, pulse, pulse, 0);
    }
}

bool Intro::IsDone() const {
    return m_textIndex >= 5;
}