/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <Engine/ECS/Component/BehaviourComponent.hpp>
#include <Engine/ECS/Component/Light/DirectionalLight.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <Engine/Resources/Type.hpp>

// Generated
#include <Generated/Sun.rfk.h>

namespace GPG RFKNamespace()
{
    class RFKClass(Inspect(), ComponentGen, Serialize()) Sun : public GPE::BehaviourComponent
    {

    private:
        RFKField(Inspect(), Serialize()) float m_dayStart{60};
        RFKField(Inspect(), Serialize()) float m_dayDuration{120.f}; // in second
        RFKField(Inspect(), Serialize()) float m_sunRiseDuration{m_dayDuration / 4.f};
        RFKField(Inspect(), Serialize()) float m_nightStart{m_dayDuration + m_dayStart};
        RFKField(Inspect(), Serialize()) float m_nightDuration{60.f}; // in second
        RFKField(Inspect(), Serialize()) float m_sunSetDuration{m_nightDuration / 4.f};

        RFKField(Inspect(), Serialize()) float m_currentTime{120.f};

        RFKField() float                       m_midDay{m_dayStart + m_dayDuration / 2.f};
        RFKField() float                       m_midNight{m_dayStart + m_nightDuration / 2.f};
        RFKField(Inspect(), Serialize()) float m_sunDistance{500.f};

        /*  Day recap :
            3h -> 5h59 : sunrise		        (40,40,70)		->	(100,100,100)
            6h -> 11h59 : light increase	    (100,100,100)	->	(255,255,255)
            12h -> 17h59 : light deacrease		(255,255,255)	->	(100,100,100)
            18h -> 20h59 : sunset		        (100,100,100)	->	(40, 40, 70 )
            21h -> 2h59 : night					(40,40,70)
        */

        RFKField(Inspect(), Serialize())
            GPE::AmbiantComponent morningSunLightColorAmbiante{1.f, 0.7f, 0.3f, 0.3f}; // Sun light color at the evening
        RFKField(Inspect(), Serialize())
            GPE::DiffuseComponent morningSunLightColorDiffuse{1.f, 0.7f, 0.3f, 0.1f}; // Sun light color at the evening
        RFKField(Inspect(), Serialize()) GPE::SpecularComponent morningSunLightColorSpecular{
            1.f, 0.7f, 0.3f, 0.1f}; // Sun light color at the evening

        RFKField(Inspect(), Serialize())
            GPE::AmbiantComponent midDaySunLightColorAmbiante{1.f, 1.f, 1.f, 0.1f}; // Sun light color at the middday
        RFKField(Inspect(), Serialize())
            GPE::DiffuseComponent midDaySunLightColorDiffuse{1.f, 1.f, 1.f, .8f}; // Sun light color at the middday
        RFKField(Inspect(), Serialize())
            GPE::SpecularComponent midDaySunLightColorSpecular{1.f, 1.f, 1.f, 1.f}; // Sun light color at the middday

        RFKField(Inspect(), Serialize())
            GPE::AmbiantComponent eveningSunLightColorAmbiante{1.f, 1.f, 1.f, 0.3f}; // Sun light color at the morning
        RFKField(Inspect(), Serialize())
            GPE::DiffuseComponent eveningSunLightColorDiffuse{1.f, 1.f, 1.f, 0.1f}; // Sun light color at the morning
        RFKField(Inspect(), Serialize())
            GPE::SpecularComponent eveningSunLightColorSpecular{1.f, 1.f, 1.f, 0.1f}; // Sun light color at the morning

        RFKField(Inspect(), Serialize())
            GPE::AmbiantComponent nightColorAmbiante{0.4f, 0.4f, 1.f, 0.2f}; // Sun light color at the // night
        RFKField(Inspect(), Serialize())
            GPE::DiffuseComponent nightColorDiffuse{0.4f, 0.4f, 1.f, 0.f}; // Sun light color at the night
        RFKField(Inspect(), Serialize())
            GPE::SpecularComponent nightColorSpecular{0.4f, 0.4f, 1.f, 0.f}; // Sun light color at the// night

        RFKField() GPE::DirectionalLight* m_pSunDirectionnalLight;
        RFKField() GPE::GameObject*       m_player = nullptr;

    public:
        Sun(GPE::GameObject & owner);

        Sun() noexcept = default;

        virtual ~Sun() = default;

        void start() final;
        void update(double deltaTime) final;

        void onPostLoad() final;

        Sun_GENERATED
    };

} // namespace )

File_GENERATED