namespace GPE
{
template <typename T>
UnrolledListAllocator<T>::UnrolledListAllocator(UnrolledListAllocator&& rhs) noexcept
    : m_firstNode(rhs.m_firstNode), m_lastNode(rhs.m_lastNode), m_size(rhs.m_size),
      m_nextToConstruct(rhs.m_nextToConstruct)
{
    rhs.m_nextToConstruct = nullptr;
    rhs.m_firstNode       = nullptr;
    rhs.m_lastNode        = nullptr;
    rhs.m_size            = 0;
}

template <typename T>
UnrolledListAllocator<T>::UnrolledListAllocator(size_t size) : m_size(size)
{
    assert(size != 0);
    addFirstNode();
    m_firstNode = m_lastNode;
}

template <typename T>
UnrolledListAllocator<T>& UnrolledListAllocator<T>::operator=(UnrolledListAllocator&& rhs) noexcept
{
    m_nextToConstruct = rhs.m_nextToConstruct;
    m_firstNode       = rhs.m_firstNode;
    m_lastNode        = rhs.m_lastNode;

    m_size = rhs.m_size;

    rhs.m_nextToConstruct = nullptr;
    rhs.m_firstNode       = nullptr;
    rhs.m_lastNode        = nullptr;
    return *this;
}

template <typename T>
UnrolledListAllocator<T>::~UnrolledListAllocator()
{
    while (m_firstNode != m_lastNode)
    {
        Node* nextNode = m_firstNode->next;
        free(m_firstNode->subNodes);
        delete m_firstNode;
        m_firstNode = nextNode;
    }

    if (m_firstNode != nullptr)
    {
        if (m_firstNode->subNodes != nullptr)
            free(m_firstNode->subNodes);
        delete m_lastNode;
    }

#ifndef NDEBUG
    GPE::Log::getInstance()->log("Allocations : " + std::to_string(nbAllocations));
    GPE::Log::getInstance()->log("Deallocations : " + std::to_string(nbDeallocations));
#endif
}

template <typename T>
UnrolledListAllocator<T> UnrolledListAllocator<T>::fromNbElements(size_t nbElementsPerNode)
{
    UnrolledListAllocator list(nbElementsPerNode);
    return list;
}

template <typename T>
UnrolledListAllocator<T> UnrolledListAllocator<T>::fromNbBytes(size_t nbBytes)
{
    UnrolledListAllocator list(nbBytes / sizeof(SubNode));
    return list;
}

template <typename T>
void UnrolledListAllocator<T>::allocateData(Node*& node, SubNode*& subNodes, size_t nbElements)
{
    node = new Node();
    subNodes = node->subNodes = (SubNode*) malloc(nbElements * sizeof(SubNode));

    if (node->subNodes == nullptr)
        throw std::bad_alloc();
}

template <typename T>
void UnrolledListAllocator<T>::initSubNodes(SubNode* subNodes, size_t nbElements)
{
    // Init SubNodes / Free list
    for (SubNode* s = subNodes; s < subNodes + nbElements; s++)
    {
        s->next = s + 1;
    }

    subNodes[nbElements - 1].next = nullptr;
}

template <typename T>
void UnrolledListAllocator<T>::addNode()
{
    Node*    newNode;
    SubNode* newSubNodes;
    allocateData(newNode, newSubNodes, m_size);

    // Set Node
    new (newNode) Node();
    m_lastNode->next = newNode;
    m_lastNode       = newNode;

    initSubNodes(newSubNodes, m_size);

    m_nextToConstruct = newSubNodes;
}

template <typename T>
void UnrolledListAllocator<T>::addFirstNode()
{
    Node*    newNode;
    SubNode* newSubNodes;
    allocateData(newNode, newSubNodes, m_size);

    // Set Node
    m_firstNode = newNode;
    m_lastNode  = newNode;

    initSubNodes(newSubNodes, m_size);

    m_nextToConstruct = newSubNodes;
}

template <typename T>
T* UnrolledListAllocator<T>::allocate(size_t nbElements)
{
    assert(nbElements == 1);
#ifndef NDEBUG
    nbAllocations += nbElements;
#endif
    SubNode* constructedElem = m_nextToConstruct;
    m_nextToConstruct        = m_nextToConstruct->next;

    // If no space left
    if (m_nextToConstruct == nullptr)
    {
        addNode();
    }

    return &constructedElem->elem;
}

template <typename T>
void UnrolledListAllocator<T>::deallocate(T* ptr, std::size_t nbElements)
{
    assert(nbElements == 1);
#ifndef NDEBUG
    nbDeallocations += nbElements;
#endif
    SubNode* subNode  = reinterpret_cast<SubNode*>(ptr);
    subNode->next     = m_nextToConstruct;
    m_nextToConstruct = subNode;
}
} // namespace GPE