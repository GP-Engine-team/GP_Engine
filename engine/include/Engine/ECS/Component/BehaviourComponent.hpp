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
    class TransformComponent;

    class RFKClass(Inspect(), ComponentGen, Serialize()) BehaviourComponent : public Component
    {
    protected:
        RFKField(Inspect(), Serialize()) bool m_useUpdate       = false;
        RFKField(Inspect(), Serialize()) bool m_useFixedUpdate  = false;
        RFKField(Inspect(), Serialize()) bool m_useOnGUI        = false;
        RFKField(Inspect(), Serialize()) bool m_useUpdateEditor = false;

    protected:
        virtual void updateToSystem() noexcept override;

    public:
        BehaviourComponent(GameObject & owner) noexcept;

        BehaviourComponent() noexcept                                = default;
        BehaviourComponent(const BehaviourComponent& other) noexcept = delete;
        virtual ~BehaviourComponent() noexcept;

        virtual void start()
        {
        }

        virtual void fixedUpdate(double deltaTime)
        {
        }

        virtual void update(double deltaTime)
        {
        }

        virtual void updateEditor(double deltaTime)
        {
        }

        virtual void onGUI()
        {
        }

        void enableUpdate(bool flag) noexcept;
        void enableFixedUpdate(bool flag) noexcept;
        void enableOnGUI(bool flag) noexcept;
        void enableUpdateEditor(bool flag) noexcept;

        [[nodiscard]] bool isUpdateEnable() const noexcept;
        [[nodiscard]] bool isFixedUpdateEnable() const noexcept;
        [[nodiscard]] bool isOnGUIEnable() const noexcept;
        [[nodiscard]] bool isUpdateEditorEnable() const noexcept;

        // UTILITY
    public:
        /**
         * @brief Stop the game if condition is false in editor mode. In game mode work in debug only with real
         * assertion.
         * If you want more detail about assertion use the macro GAME_ASSERT
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

        void drawDebugLine(const GPM::Vec3& pt1, const GPM::Vec3& pt2,
                           const ColorRGB& color = ColorRGB::black()) noexcept;

        void log(const std::string& msg);
        void logWarning(const std::string& msg);
        void logError(const std::string& msg);

        void reloadScene();
        void loadNewScene(const char* path);

        void closeApplication();

        TransformComponent& transform();

        BehaviourComponent_GENERATED
    };

/**
 * @brief Stop the game if condition is false in editor mode. In game mode work in debug only with real
 * assertion.
 */
#ifdef NDEBUG

#define GAME_ASSERT(expr, msg)

#else /* Not NDEBUG.  */

#define GAME_ASSERT(expr, msg)                                                                                         \
    if (!expr)                                                                                                         \
    {                                                                                                                  \
        gameAssert(expr,                                                                                               \
                   GPE::stringFormat("%s in function %s %s : %d\nExpression \"%s\" == false.\n%s",                     \
                                     F_RED("Game Assertion"), __FUNCTION__, __FILE__, __LINE__, BOLD(#expr), msg)      \
                       .c_str());                                                                                      \
        return;                                                                                                        \
    }
#endif // NDEBUG

} // namespace )
