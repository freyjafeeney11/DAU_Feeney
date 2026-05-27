#include "stdafx.h"
#include "StatSelect.h"
#include "PlayerArchetype.h"
#include <windows.h>

StatSelect::StatSelect() {
    m_bg = App::CreateSprite(".\\TestData\\stat_select.png", 1, 1);
    m_bg->SetPosition(512.0f, 400.0f);
    m_bg->SetScale(0.6f);

    m_dialogueBg = App::CreateSprite(".\\TestData\\dialogue.png", 1, 1);
    m_dialogueBg->SetPosition(512.0f, 80.0f);
    m_dialogueBg->SetScale(0.6f);

    m_choice       = 0;
    m_navDown      = false;
    m_enterDown    = true;
    m_selected     = false;
    m_fading       = false;
    m_showDialogue = false;
    m_done         = false;
    m_fadeTimer    = 0.0f;
    m_dialogueTimer= 0.0f;
    m_typeTimer    = 0.0f;
    m_typeIndex    = 0;
}

StatSelect::~StatSelect() {
    delete m_bg;
    delete m_dialogueBg;
}

void StatSelect::Update(float deltaTime) {
    float dt = deltaTime / 1000.0f;

    if (m_fading) {
        m_fadeTimer += dt;
        if (m_fadeTimer >= FADE_DURATION && !m_showDialogue) {
            m_showDialogue  = true;
            m_dialogueTimer = 0.0f;
            m_fullText      = "You hear the sound of rain and rattling tracks...";
            m_displayedText = "";
            m_typeTimer     = 0.0f;
            m_typeIndex     = 0;
        }
        if (m_showDialogue) {
            if (m_typeIndex < (int)m_fullText.size()) {
                m_typeTimer += dt;
                if (m_typeTimer >= TYPE_SPEED) {
                    m_typeTimer = 0.0f;
                    m_displayedText += m_fullText[m_typeIndex];
                    m_typeIndex++;
                }
            }
            m_dialogueTimer += dt;
            if (m_dialogueTimer >= DIALOGUE_DURATION) {
                m_done = true;
            }
        }
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
    if (!App::IsKeyPressed(VK_UP) && !App::IsKeyPressed(VK_DOWN)) m_navDown = false;

    if (App::IsKeyPressed(VK_RETURN) && !m_enterDown) {
        m_enterDown = true;
        m_selected  = true;
        m_fading    = true;
        m_fadeTimer = 0.0f;
        switch (m_choice) {
            case 0: PlayerArchetype::current = Archetype::CHARISMATIC; break;
            case 1: PlayerArchetype::current = Archetype::DEXTEROUS;   break;
            case 2: PlayerArchetype::current = Archetype::LUCKY;       break;
        }
        App::PlaySound(".\\TestData\\audio\\confirm.wav", false);
        App::SetSoundVolume(".\\TestData\\audio\\confirm.wav", 0.5f);
    }
    if (!App::IsKeyPressed(VK_RETURN)) m_enterDown = false;
}

void StatSelect::Render() {
    float fade = 1.0f;
    if (m_fading && !m_showDialogue) {
        fade = 1.0f - (m_fadeTimer / FADE_DURATION);
        if (fade < 0.0f) fade = 0.0f;
    } else if (m_fading && m_showDialogue) {
        fade = 0.0f;
    }

    m_bg->SetColor(fade, fade, fade);
    m_bg->Draw();

    if (!m_fading) {
        const float optY[3] = { 355.0f, 290.0f, 235.0f };
        const char* labels[3] = { "", "", "" };

        for (int i = 0; i < 3; i++) {
            float r = (m_choice == i) ? 0.9f : 0.55f;
            float g = (m_choice == i) ? 0.78f : 0.45f;
            float b = (m_choice == i) ? 0.35f : 0.25f;
            App::PrintTTF(430, optY[i], labels[i], r, g, b, 0);
            if (m_choice == i) {
                App::PrintTTF(360, optY[i], ">", 0.239f, 0.0f, 0.0f, 0);
            }
        }

        const char* descs[3] = {
            "Silver tongued and slippery",
            "Being quick on your feet is a plus",
            "Play with weighted dice to even"
        };
        const char* descs2[3] = {
            "means more slack and more lives.",
            "in this business...",
            "the odds."
        };

        App::PrintTTF(360, 175, descs[m_choice],  0.239f, 0.0f, 0.0f, 0);
        App::PrintTTF(360, 155, descs2[m_choice], 0.239f, 0.0f, 0.0f, 0);
    }

    if (m_showDialogue) {
        float dFade = 1.0f;
        if (m_dialogueTimer > DIALOGUE_DURATION - 0.8f) {
            dFade = (DIALOGUE_DURATION - m_dialogueTimer) / 0.8f;
            if (dFade < 0.0f) dFade = 0.0f;
        }
        m_dialogueBg->SetColor(dFade, dFade, dFade);
        m_dialogueBg->Draw();
        App::PrintTTF(174, 60, m_displayedText.c_str(), dFade, dFade, dFade, 0);
    }
}
