#pragma once

#include <vector>
#include <memory>
#include <cassert>
#include <iosfwd>

// Global Allocator
template<typename T>
class UnrolledListAllocator
{
private:
    // What's inside the Node elements.
    union SubNode
    {
        // Points to the next element that should be constructed.
        // Can be in a different chunk.
        SubNode* next = nullptr;
        T elem;
    };

    std::vector<SubNode*> m_nodes; // Pointers to all chunks to free them later.
    size_t m_size; // nb Elements per node
    SubNode* nextToConstruct = nullptr; // The location of the next node that will be constructed

    // A node should only be added when there is no space left in existing nodes.
    // Adds a chunk.
    void addNode();

public:
    // The size must always be non null.
    UnrolledListAllocator() = delete;

    // Without a big performance issue, we don't know elemenets that have been constructed, so we can't copy them.
    UnrolledListAllocator(const UnrolledListAllocator&) = delete;
    UnrolledListAllocator(UnrolledListAllocator&&);

    UnrolledListAllocator(size_t nbElements);

    // Without a big performance issue, we don't know elemenets that have been constructed, so we can't copy them.
    UnrolledListAllocator& operator=(const UnrolledListAllocator&) = delete;
    UnrolledListAllocator& operator=(UnrolledListAllocator&&);

    ~UnrolledListAllocator();

    static UnrolledListAllocator fromNbElements(size_t nbElementsPerNode);
    static UnrolledListAllocator fromNbBytes(size_t nbBytes);

    T* allocate(size_t nbElements);
    void deallocate(T* ptr, std::size_t nbElements);

    //  Define it if you want to debug or see what's inside UnrolledListAllocator
    template<typename U>
    friend std::ostream& operator<<(std::ostream& out, const UnrolledListAllocator<U>& list);
};

template<typename T>
using UListAllocator = UnrolledListAllocator<T>;

#include "UnrolledListAllocator.inl"