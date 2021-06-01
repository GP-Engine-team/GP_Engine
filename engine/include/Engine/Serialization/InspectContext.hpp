#pragma once

#include <vector>

namespace GPE
{

class InspectContext
{
private:
    struct Node
    {
        float indentLength;
        bool hasChanged = false;
    };

    std::vector<Node> hierarchy;
    bool              hasLastChanged = false;
    float             sepRatio       = 0.5f;

public:
    float startingWidth = 0.f;

    void startProperty(const char* name, float indentLength = 16.f);
    void endProperty();

    void setDirty();
    void setDirty(bool shouldBecomeDirty);
    bool isDirty() const;
    bool wasLastDirty() const;
};

} // namespace GPE