// NPC.cpp
#include "stdafx.h"
#include "NPC.h"

NPC::NPC(const char* spritePath, const char* npcName, int npcDifficulty, int* npcLoot, float startX, float startY, float scale) {
    name = npcName;
    difficulty = npcDifficulty;
    isAlerted = false;
    // loot table
    for (int i = 0; i < 6; i++) {
        lootTable[i] = npcLoot[i];
    }
    sprite = App::CreateSprite(spritePath, 4, 1);
    sprite->SetPosition(startX, startY);
    sprite->SetScale(scale);
    sprite->CreateAnimation(0, 0.4f, { 0,1,2,3 });
    sprite->SetAnimation(0);
}

NPC::~NPC() {
    delete sprite;
}

void NPC::Update(float deltaTime) {
    sprite->Update(deltaTime);
}

void NPC::Render(float camX, float camY) {
    float actualX, actualY;
    sprite->GetPosition(actualX, actualY);

    sprite->SetPosition(actualX - camX, actualY - camY);
    sprite->Draw();

    sprite->SetPosition(actualX, actualY);
}

void NPC::GetPosition(float& x, float& y) {
    sprite->GetPosition(x, y);
}

float NPC::GetHeight() {
    return sprite->GetHeight();
}

float NPC::GetScale() {
    return sprite->GetScale();
}

std::string NPC::GetName() { return name; }
int NPC::GetDifficulty() { return difficulty; }
int* NPC::GetLootTable() { return lootTable; }
bool NPC::GetIsAlerted() { return isAlerted; }
void NPC::SetAlerted(bool alerted) { isAlerted = alerted; }