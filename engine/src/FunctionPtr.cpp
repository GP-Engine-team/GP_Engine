#include "Engine/Core/Tools/FunctionPtr.hpp"

void GPE::Function::save(XmlSaver& context) const
{
    GPE::save(context, subFunctor.index(), XmlSaver::SaveInfo{"index", "size_t", 0});
    switch (subFunctor.index())
    {
    case 0:
        const MemberFunction& memberFunction = std::get<MemberFunction>(subFunctor);
        GPE::save(context, size_t(memberFunction.caller), XmlSaver::SaveInfo{"callerAddress", "size_t", 0});

        size_t entityId = memberFunction.method->id;
        GPE::save(context, entityId, XmlSaver::SaveInfo{"entityId", "size_t", 0});
        break;
    }
}
void GPE::Function::load(XmlLoader& context)
{
    size_t index;
    GPE::load(context, index, XmlLoader::LoadInfo{"index", "size_t", 0});
    switch (index)
    {
    case 0:
        MemberFunction& memberFunction = std::get<MemberFunction>(subFunctor);

        size_t callerAddress;
        GPE::load(context, callerAddress, XmlLoader::LoadInfo{"callerAddress", "size_t", 0});
        memberFunction.caller = (void*)callerAddress;
        context.addLazy(memberFunction.caller);

        size_t entityId;
        GPE::load(context, entityId, XmlLoader::LoadInfo{"entityId", "size_t", 0});
        memberFunction.method = rfk::entityCast<rfk::Method>(rfk::getDatabase().getEntity(entityId));
        break;
    }
}

