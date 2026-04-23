#pragma once
#include "App\stdafx.h"
#include "App\app.h"
#include "App\SimpleSprite.h"

class GearManager {
public:
    static GearManager& GetInstance() {
        static GearManager instance;
        return instance;
    }

    void AddGear();
    int GetGearCount() const { return m_gearCount; }

    // render
    void RenderUI(float x, float y);

private:
    GearManager();
    ~GearManager();

    GearManager(const GearManager&) = delete;
    void operator=(const GearManager&) = delete;

    int m_gearCount;
    CSimpleSprite* m_watchSprite;
};