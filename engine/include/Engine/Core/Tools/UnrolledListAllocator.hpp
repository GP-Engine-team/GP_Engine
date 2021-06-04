#pragma once

#include <cassert>
#include <iosfwd>
#include <memory>

// .inl
#include "Engine/Core/Debug/Log.hpp"

namespace GPE
{

// Global Allocator
template <typename T>
class UnrolledListAllocator
{
#ifndef NDEBUG
public:
    size_t nbAllocations   = 0;
    size_t nbDeallocations = 0;
#endif
private:
    // What's inside the Node elements.
    union SubNode {
        // Points to the next element that should be constructed.
        // Can be in a different chunk.
        SubNode* next;
        T        elem;

        ~SubNode() = delete;
    };

    // All allocated subNodes start with a Node,
    // so if Node is free, subNodes are also free.
    struct Node
    {
        Node* next = nullptr;
        SubNode* subNodes = nullptr;
    };

    Node*    m_firstNode       = nullptr; // Points to the first created node
    Node*    m_lastNode        = nullptr; // Points to the last added node
    size_t   m_size            = 0;       // nb Elements per node
    SubNode* m_nextToConstruct = nullptr; // The location of the next node that will be constructed

    static void allocateData(Node*& node, SubNode*& subNodes, size_t nbElements);
    static void initSubNodes(SubNode* subNodes, size_t nbElements);

    // A node should only be added when there is no space left in existing nodes.
    // Adds a chunk.
    void addNode();
    void addFirstNode();

public:
    // The size must always be non null.
    UnrolledListAllocator() = delete;

    // Without a big performance issue, we don't know elemenets that have been constructed, so we can't copy them.
    UnrolledListAllocator(const UnrolledListAllocator&) = delete;
    UnrolledListAllocator(UnrolledListAllocator&&) noexcept;

    UnrolledListAllocator(size_t nbElements);

    // Without a big performance issue, we don't know elemenets that have been constructed, so we can't copy them.
    UnrolledListAllocator& operator=(const UnrolledListAllocator&) = delete;
    UnrolledListAllocator& operator                                =(UnrolledListAllocator&&) noexcept;

    ~UnrolledListAllocator();

    static UnrolledListAllocator fromNbElements(size_t nbElementsPerNode);
    static UnrolledListAllocator fromNbBytes(size_t nbBytes);

    T*   allocate(size_t nbElements = 1);
    void deallocate(T* ptr, std::size_t nbElements = 1);

    //  Define it if you want to debug or see what's inside UnrolledListAllocator
    template <typename U>
    friend std::ostream& operator<<(std::ostream& out, const UnrolledListAllocator<U>& list);
};

template <typename T>
using UListAllocator = UnrolledListAllocator<T>;
} // namespace GPE

#include "UnrolledListAllocator.inl"