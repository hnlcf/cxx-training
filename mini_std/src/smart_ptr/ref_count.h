// Copyright (c) 2022 Changfeng Lou. All rights reserved.

#pragma once

namespace mini_std {
typedef unsigned long long ref_count_t;

/// Base class of reference count which is responsible for recording the use
/// count and weak count.
class ref_count_base
{
public:
    ref_count_base()
      : m_use_count(1)
      , m_weak_count(1)
    {
    }

    void add_ref_copy() { m_use_count += 1; }

    void weak_add_ref() { m_weak_count += 1; }

    void weak_release()
    {
        if (--m_weak_count == 0) {
            destroy();
        }
    }

    void release()
    {
        if (--m_use_count == 0) {
            dispose();

            if (--m_weak_count == 0) {
                destroy();
            }
        }
    }

    /// Free resources managed.
    virtual void dispose() {}

    /// Free self.
    virtual void destroy() { delete this; }

    ref_count_t get_use_count() const { return m_use_count; }

private:
    ref_count_base(const ref_count_base &)            = delete;
    ref_count_base &operator=(const ref_count_base &) = delete;

private:
    ref_count_t m_use_count;
    ref_count_t m_weak_count;
};

/// Derive class of `ref_count_base` which is responsible for freeing resources
/// managed.
template<class Ptr>
class ref_count_ptr : public ref_count_base
{
public:
    explicit ref_count_ptr(Ptr ptr)
      : m_ptr(ptr)
    {
    }

    void dispose() override { delete m_ptr; }

    void destroy() override { delete this; }

private:
    Ptr m_ptr;
};

} // namespace mini_std
