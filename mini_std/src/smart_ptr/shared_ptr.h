// Copyright (c) 2022 Changfeng Lou. All rights reserved.

#pragma once

#include <exception>

#include "smart_ptr/ref_count.h"

namespace mini_std {

/// Reference count manager of shared pointer.
class shared_manager
{
public:
    friend class weak_manager;

public:
    shared_manager()
      : m_refcount(nullptr)
    {
    }

    template<class Ptr>
    explicit shared_manager(Ptr ptr)
      : m_refcount(nullptr)
    {
        m_refcount = new ref_count_ptr(ptr);
    }

    shared_manager(const shared_manager &other)
      : m_refcount(other.m_refcount)
    {
        if (m_refcount != nullptr) {
            m_refcount->add_ref_copy();
        }
    }

    shared_manager &operator=(const shared_manager &other)
    {
        if (m_refcount != other.m_refcount) {
            if (m_refcount != nullptr) {
                m_refcount->release();
            }

            if (other.m_refcount != nullptr) {
                other.m_refcount->add_ref_copy();
            }

            m_refcount = other.m_refcount;
        }

        return *this;
    }

    ~shared_manager()
    {
        if (m_refcount != nullptr) {
            m_refcount->release();
        }
    }

    void swap(shared_manager &other)
    {
        ref_count_base *tmp = other.m_refcount;
        other.m_refcount    = m_refcount;
        m_refcount          = tmp;
    }

    [[nodiscard]] ref_count_t get_use_count() const { return m_refcount->get_use_count(); }

    [[nodiscard]] bool unique() const { return get_use_count() == 1; }

private:
    ref_count_base *m_refcount;
};

template<class T>
class weak_ptr;

/// Shared pointer based on reference count.
template<class T>
class shared_ptr
{
public:
    friend class weak_ptr<T>;

public:
    constexpr shared_ptr()
      : m_ptr(nullptr)
      , m_mgr()
    {
    }

    explicit shared_ptr(T *ptr)
      : m_ptr(ptr)
      , m_mgr(ptr)
    {
    }

    shared_ptr(const shared_ptr &other)
      : m_ptr(other.m_ptr)
      , m_mgr(other.m_mgr)
    {
    }

    explicit shared_ptr(const weak_ptr<T> &other)
      : m_mgr(other.m_mgr)
    {
        m_ptr = other.m_ptr;
    }

    // This constructor is used by __weak_ptr::lock() and
    shared_ptr(const weak_ptr<T> &other, std::nothrow_t)
      : m_mgr(other.m_mgr)
    {
        if (m_mgr.get_use_count() == 0) {
            m_ptr = nullptr;
        } else {
            m_ptr = other.m_ptr;
        }
    }

    shared_ptr &operator=(const shared_ptr &other)
    {
        m_ptr = other.m_ptr;
        m_mgr = other.m_mgr;
        return *this;
    }

    T *get() const { return m_ptr; }

    T *operator->() const noexcept { return get(); }

    T &operator*() const noexcept { *get(); }

    [[nodiscard]] ref_count_t use_count() const { return m_mgr.get_use_count(); }

    [[nodiscard]] bool is_unique() const { return m_mgr.unique(); }

    void swap(shared_ptr &other)
    {
        std::swap(m_ptr, other.m_ptr);
        m_mgr.swap(other.m_mgr);
    }

    void reset() { shared_ptr().swap(*this); }

private:
    T             *m_ptr;
    shared_manager m_mgr;
};

template<typename T, typename... Args>
shared_ptr<T>
make_shared(Args &&...args)
{
    return shared_ptr<T>(std::forward<Args>(args)...);
}

} // namespace mini_std