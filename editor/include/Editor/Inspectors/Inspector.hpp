#pragma once

namespace Editor
{
    class Inspector
    {
    public:
        template<typename T>
        void draw(T& instance)
        {
            /*
            for (rfk::Field& field : T::GetStaticArchetype()->fields)
            {
                // Display field.name
                // Display field.value
            }
            */
        }
    };
}