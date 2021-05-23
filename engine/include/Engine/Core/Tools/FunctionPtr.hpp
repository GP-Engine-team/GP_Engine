#pragma once
#include "Engine/Serialization/xml/xmlLoader.hpp"
#include "Engine/Serialization/xml/xmlSaver.hpp"
#include <Refureku/Refureku.h>
#include <variant>

namespace GPE
{

class Function
{
private:
    struct MemberFunction
    {
        void* caller = nullptr;
        rfk::Method const* method = nullptr;

        bool operator==(const MemberFunction& mf) const
        {
            return (caller == mf.caller) && (method == mf.method);
        }
    };

    std::variant<MemberFunction> subFunctor;

public:
    template <typename A>
    void set(A* instance, const std::string& methodName, bool shouldCheckInheritedMethods = true)
    {
        rfk::Struct const& s      = A::staticGetArchetype(); // No archetype found : Check if the class is reflected with RFKClass().
        rfk::Method const* method = s.getMethod(methodName, rfk::EMethodFlags::Default, shouldCheckInheritedMethods);
        subFunctor                = MemberFunction{instance, method};
        assert(method != nullptr); // No matching method found : Method name invalid or method not reflected (put RFKMethod() to reflect it)
    }

    template <typename TClass>
    static GPE::Function make(TClass* owner, const std::string& name)
    {
        GPE::Function func;
        func.set(owner, name);
        return func;
    }

    template <typename R, typename... ARGS>
    R operator()(ARGS&&... args)
    {
        switch (subFunctor.index())
        {
        case 0:
            MemberFunction& memberFunction = std::get<MemberFunction>(subFunctor);
            assert(memberFunction.method != nullptr);
            return memberFunction.method->checkedRInvoke<R>(memberFunction.caller, std::forward<ARGS>(args)...);
            break;
        }
        throw;
    }

    template <typename... ARGS>
    void operator()(ARGS&&... args)
    {
        switch (subFunctor.index())
        {
        case 0:
            MemberFunction& memberFunction = std::get<MemberFunction>(subFunctor);
            assert(memberFunction.method != nullptr);
            memberFunction.method->checkedInvoke(memberFunction.caller, std::forward<ARGS>(args)...);
            break;
        }
    }

    void save(XmlSaver& context) const;
    void load(XmlLoader& context);

    bool operator==(const Function& f) const
    {
        return subFunctor == f.subFunctor;
    }
};

} // namespace GPE