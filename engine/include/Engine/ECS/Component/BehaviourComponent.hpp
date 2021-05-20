/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <Engine/ECS/Component/Component.hpp>
#include <Engine/Serialization/ComponentGen.h>

#include <Engine/Resources/Color.hpp>
#include <gpm/Quaternion.hpp>
#include <gpm/Vector3.hpp>

// Generated
#include <Generated/BehaviourComponent.rfk.h>

namespace GPE RFKNamespace()
{
    class RFKClass(Inspect(), ComponentGen, Serialize()) BehaviourComponent : public Component
    {
    protected:
        RFKField(Inspect(), Serialize()) bool m_useUpdate      = false;
        RFKField(Inspect(), Serialize()) bool m_useFixedUpdate = false;
        RFKField(Inspect(), Serialize()) bool m_useOnGUI       = false;

    public:
        BehaviourComponent(GameObject & owner) noexcept;

        BehaviourComponent() noexcept                                = default;
        BehaviourComponent(const BehaviourComponent& other) noexcept = delete;
        BehaviourComponent(BehaviourComponent && other) noexcept;
        virtual ~BehaviourComponent() noexcept;
        BehaviourComponent& operator=(BehaviourComponent const& other) noexcept = delete;
        BehaviourComponent& operator                                            =(BehaviourComponent&& other) noexcept;

        virtual void onPostLoad();

        virtual void start()
        {
        }

        virtual void fixedUpdate(double deltaTime)
        {
        }

        virtual void update(double deltaTime)
        {
        }

        virtual void onGUI()
        {
        }

        void enableUpdate(bool flag) noexcept;
        void enableFixedUpdate(bool flag) noexcept;
        void enableOnGUI(bool flag) noexcept;

        [[nodiscard]] bool isUpdateEnable() const noexcept;
        [[nodiscard]] bool isFixedUpdateEnable() const noexcept;
        [[nodiscard]] bool isOnGUIEnable() const noexcept;

        /**
         * @brief Add or remove current component from it's system which have for effect to enable or disable it
         * @param newState
         * @return
         */
        void setActive(bool newState) noexcept override;

        // UTILITY
    public:
        /**
         * @brief Stop the game if condition is false in editor mode. In game mode work in debug only with real
         * assertion.
         */
        void gameAssert(bool condition, const char* msg = "");

        void drawDebugSphere(const GPM::Vec3& position, float radius,
                             const ColorRGBA& color = ColorRGBA{0.5f, 0.f, 0.f, 0.5f}, float duration = 0.f,
                             bool enableBackFaceCullling = true) noexcept;

        void drawDebugCube(const GPM::Vec3& position, const GPM::Quat& rotation, const GPM::Vec3& scale,
                           const ColorRGBA& color = ColorRGBA{0.5f, 0.f, 0.f, 0.5f}, float duration = 0.f,
                           bool enableBackFaceCullling = true) noexcept;

        void drawDebugQuad(const GPM::Vec3& position, const GPM::Vec3& dir, const GPM::Vec3& scale,
                           const ColorRGBA& color = ColorRGBA{0.5f, 0.f, 0.f, 0.5f}, float duration = 0.f,
                           bool enableBackFaceCullling = true) noexcept;

        void drawDebugLine(const GPM::Vec3& pt1, const GPM::Vec3& pt2, float width = 1.f,
                           const ColorRGBA& color = ColorRGBA{0.5f, 0.f, 0.f, 0.5f}) noexcept;

        void log(const std::string& msg);
        void logWarning(const std::string& msg);
        void logError(const std::string& msg);

        BehaviourComponent_GENERATED
    };

} // namespace )
