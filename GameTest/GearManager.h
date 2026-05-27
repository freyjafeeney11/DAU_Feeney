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

    
    void RenderUI(float x, float y);
    
    bool IsComplete() const { return m_gearCount >= 7; }
    void Reset();

private:
    GearManager();
    ~GearManager();

    GearManager(const GearManager&) = delete;
    void operator=(const GearManager&) = delete;

    int m_gearCount;
    CSimpleSprite* m_watchSprite;
};