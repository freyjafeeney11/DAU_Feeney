// CrowdManager.h
#pragma once
#include "app\app.h"
#include <math.h>
#include <memory>

class CrowdManager {
private:
    static const int ms_NUM_CLUMPS = 3;
    static const int ms_MEMBERS_PER_CLUMP = 5;
    static float GetRandomOffset(int amplitude);

    struct CrowdMember {
        std::unique_ptr<CSimpleSprite> m_sprite;
        float m_offsetX;
        float m_offsetY;
        float m_personalSwayOffset;
    };

    struct CrowdClump {
        float m_baseX;
        float m_baseY;
        float m_swayOffset;
        CrowdMember m_members[ms_MEMBERS_PER_CLUMP];
    };

    CrowdClump m_crowdClumps[ms_NUM_CLUMPS];
    float m_swayTime;

public:
    CrowdManager();
    ~CrowdManager() = default;

    void Update(float deltaTime);
    void Render(float camX, float camY);
};