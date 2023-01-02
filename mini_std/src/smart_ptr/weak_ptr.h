// Copyright (c) 2022 Changfeng Lou. All rights reserved.

#pragma once

#include "smart_ptr/ref_count.h"
#include "smart_ptr/shared_ptr.h"

namespace mini_std {

/// Reference count manager of weak pointer.
class weak_manager
{
public:
    friend class shared_manager;

public:
    weak_manager()
      : m_refcount(nullptr)
    {
    }

    explicit weak_manager(const shared_manager &other)
      : m_refcount(other.m_refcount)
    {
        if (m_refcount != nullptr) {
            m_refcount->weak_add_ref();
        }
    }

    weak_manager(const weak_manager &other)
      : m_refcount(other.m_refcount)
    {
        if (m_refcount != nullptr) {
            m_refcount->weak_add_ref();
        }
    }

    weak_manager &operator=(const shared_manager &other)
    {
        ref_count_base *tmp = other.m_refcount;
        if (tmp != nullptr) {
            tmp->weak_add_ref();
        }

        if (m_refcount != nullptr) {
            m_refcount->weak_release();
        }

        m_refcount = tmp;

        return *this;
    }

    weak_manager &operator=(const weak_manager &other)
    {
        ref_count_base *tmp = other.m_refcount;
        if (tmp != nullptr) {
            tmp->weak_add_ref();
        }

        if (m_refcount != nullptr) {
            m_refcount->weak_release();
        }

        m_refcount = tmp;

        return *this;
    }

    ~weak_manager()
    {
        if (m_refcount != nullptr) {
            m_refcount->weak_release();
        }
    }

    [[nodiscard]] ref_count_t get_use_count() const { return m_refcount->get_use_count(); }

private:
    ref_count_base *m_refcount;
};

template<class T>
class weak_ptr
{
public:
    constexpr weak_ptr()
      : m_ptr(nullptr)
      , m_mgr()
    {
    }

    explicit weak_ptr(T *ptr)
      : m_ptr(ptr)
      , m_mgr(ptr)
    {
    }

    weak_ptr(const weak_ptr &other)
      : m_ptr(other.m_ptr)
      , m_mgr(other.m_mgr)
    {
    }

    weak_ptr &operator=(const weak_ptr &other)
    {
        m_ptr = other.m_ptr;
        m_mgr = other.m_mgr;
        return *this;
    }

    weak_ptr &operator=(const shared_ptr<T> &other)
    {
        m_ptr = other.m_ptr;
        m_mgr = other.m_mgr;
        return *this;
    }

    T *get() { return m_ptr; }

    T *operator->() const noexcept { get(); }

    T &operator*() const noexcept { *get(); }

    shared_ptr<T> lock() { return shared_ptr(*this, std::nothrow); }

    [[nodiscard]] ref_count_t use_count() const { return m_mgr.get_use_count(); }

private:
    T           *m_ptr;
    weak_manager m_mgr;
};

} // namespace mini_std
