#include "Engine/Serialization/STDReflect.hpp"

rfk::ArchetypeRegisterer ListRegisterer = &List<GPE::Component*>::staticGetArchetype();


rfk::ArchetypeRegisterer CustomClassRegisterer = &MyCustomClass::staticGetArchetype();