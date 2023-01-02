// Copyright (c) 2022 Changfeng Lou. All rights reserved.

#pragma once

#include <iostream>
#include <vector>

// TODO(changfeng): memory align

namespace mini_std {

class MemBlock
{
public:
    MemBlock()
      : m_ptr(nullptr)
      , m_remain(0)
      , m_total(0)
    {
    }

    ~MemBlock()
    {
        for (char *i : m_blocks) {
            delete[] i;
        }
    }

    MemBlock(const MemBlock &)            = delete;
    MemBlock &operator=(const MemBlock &) = delete;

public:
    char *allocate(size_t bytes)
    {
        if (bytes <= m_remain) {
            char *res = m_ptr;
            m_ptr += bytes;
            m_remain -= bytes;
            return res;
        }
        return allocateMore(bytes);
    }

    char *allocateMore(size_t bytes)
    {
        if (bytes > kBlockSize / 4) {
            return allocateNewBlock(bytes);
        }

        // waste the remaining space and allocate a new block,
        // the space wasted is quarter of block size at most.
        m_ptr     = allocateNewBlock(kBlockSize);
        m_remain  = kBlockSize;

        char *res = m_ptr;
        m_ptr += bytes;
        m_remain -= bytes;
        return res;
    }

    char *allocateNewBlock(size_t bytes)
    {
        char *res = new char[bytes];
        m_blocks.push_back(res);
        m_total += bytes;
        return res;
    }

    size_t totalMemory() const { return m_total; }

private:
    static const int kBlockSize = 4096; // default block size

    char               *m_ptr;    // pointer to unused location in current block
    size_t              m_remain; // remaining memory in current block
    std::vector<char *> m_blocks; // record all head pointers of block
    size_t              m_total;  // record all memory usage
};

} // namespace mini_std
