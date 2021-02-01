# Code normes

All sources, comments included, must be in english.

#### Key case
- Structures, classes and namespaces are in PascalCase
- Functions and variables are in camelCase (except for elementary/canonical types)
- Private data members must be prefixed with `m_`
- Macros and enum members are written in capitalized SNAKE_CASE
```C++
// Sample code
#define STATIC_ASSERT(condition, message) \
/* ... */

namespace Math
{
struct Vector3f
{
    float x;
    float y;
    float z;
};
}

class Mesh
{
public:
    enum { HIDDEN, HALF_HIDDEN, VISIBLE };

private:
    std::vector<Math::Vector3f> m_vertices;

public:
    float scale;
    /* ... */
};
```

#### Type hints
- Interfaces are prefixed with `I`
- Abstract classes and structures are prefixed with `A`
- Enum types and enum classes are prefixed with `E`
- When it adds to their description, booleans must start with `has`/`have` or `is`/`are`

```C++
// Sample code
struct IExampleInterface
{
    virtual doStuff()     const = 0;
    virtual launchThing() const = 0;
};

class AShape
{
    protected:
        float verticalWidth;
        float horizontalHeight;
        bool  isConvex;

    public:
        void draw() const = 0;
};

enum class EShapeType
{
    LINE,
    TRIANGLE,
    RECTANGLE
};
```

#### Code practices
- No `typedef` or `using` in a header, except if it is in a class structure declaration or a function's body
- Forward declaration must be used where possible
- Variables inside classes and structures must be memory-aligned
- Variable name must be explicit, but as short as possible
- Don't define default constructor if the default values can be defined in tbe class declaration
```C++
// Sample code
class AShape;

class ShapeContainer
{
    using pointer_type = AShape*;
    using ref_type     = AShape&;

    enum EExpansionPolicy { LOG, LINEAR, DOUBLE, SQUARED };

    private:
        pointer_type     data     = nullptr;
        size_t           size     = 0u;
        size_t           capacity = 0u;
        EExpansionPolicy policy   = EExpansionPolicy::DOUBLE;

    public:
        Container() = default;
    /* ... */
};
```

#### Formatting
- Tabulation must be replaced by 4 spaces
- Align consecutive variables, return types, fonction names, cv-qualifiers and other keywords when possible

```C++
// Sample code
MeshRenderer meshRenderer;
double       a;
int          bcd;
float        efgh;
```