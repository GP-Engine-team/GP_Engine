template <typename T>
UnrolledListAllocator<T>::UnrolledListAllocator(UnrolledListAllocator&& rhs)
    : m_nodes(std::move(rhs.m_nodes)), m_size(rhs.m_size), nextToConstruct(rhs.nextToConstruct)
{
    rhs.nextToConstruct = nullptr;
}

template <typename T>
UnrolledListAllocator<T>::UnrolledListAllocator(size_t size) : m_size(size)
{
    addNode();
}

template <typename T>
UnrolledListAllocator<T>& UnrolledListAllocator<T>::operator=(UnrolledListAllocator&& rhs)
{
    m_nodes = std::move(rhs.m_nodes);
    m_size  = rhs.m_size;

    nextToConstruct     = rhs.nextToConstruct;
    rhs.nextToConstruct = nullptr;
    return *this;
}

template <typename T>
UnrolledListAllocator<T>::~UnrolledListAllocator()
{
    for (SubNode* node : m_nodes)
    {
        free(node);
    }
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
void UnrolledListAllocator<T>::addNode()
{
    SubNode* allocated = (SubNode*)malloc(m_size * sizeof(SubNode));
    m_nodes.push_back(allocated);

    for (SubNode* s = allocated; s < allocated + m_size; s++)
    {
        s->next = s + 1;
    }

    allocated[m_size - 1].next = nullptr;

    nextToConstruct = allocated;
}

template <typename T>
T* UnrolledListAllocator<T>::allocate(size_t nbElements)
{
    assert(nbElements == 1);

    SubNode* constructedElem = nextToConstruct;
    nextToConstruct          = nextToConstruct->next;

    // If not space left
    if (nextToConstruct == nullptr)
    {
        addNode();
    }

    return &constructedElem->elem;
}

template <typename T>
void UnrolledListAllocator<T>::deallocate(T* ptr, std::size_t nbElements)
{
    assert(nbElements == 1);
    SubNode* subNode = reinterpret_cast<SubNode*>(ptr);
    subNode->next    = nextToConstruct;
    nextToConstruct  = subNode;
}