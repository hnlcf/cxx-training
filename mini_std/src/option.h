// Copyright (c) 2022 Changfeng Lou. All rights reserved.

#pragma once

#include <stdexcept>
#include <utility>

namespace mini_std {

template<typename T>
class Option
{
    using data_t    = T;
    using pointer_t = T *;

public:
    Option()
      : m_flag(false)
      , m_data(nullptr)
    {
    }

    explicit Option(const T &v) { create(v); }

    explicit Option(T &&v) { create(std::move(v)); }

    ~Option() { destroy(); }

    Option(const Option &other)
      : m_flag(false)
      , m_data(nullptr)
    {
        if (other.is_some()) {
            assign(other);
        }
    }

    Option(Option &&other) noexcept
      : m_flag(false)
      , m_data(nullptr)
    {
        if (other.is_some()) {
            assign(std::move(other));
            other.destroy();
        }
    }

    Option &operator=(Option &&other) noexcept
    {
        assign(std::move(other));
        return *this;
    }

    Option &operator=(const Option &other)
    {
        assign(other);
        return *this;
    }

    template<class... Args>
    void emplace(Args &&...args)
    {
        destroy();
        create(std::forward<Args>(args)...);
    }

    bool is_none() const { return !m_flag; }

    bool is_some() const { return m_flag; }

    data_t const &value() const &
    {
        if (is_some()) {
            return **this;
        }

        throw std::logic_error("Option is None.");
    }

    template<class U>
    data_t value_or(U &&default_value) const &
    {
        if (is_some()) {
            return **this;
        } else {
            return static_cast<T>(std::forward<U>(default_value));
        }
    }

    explicit operator bool() const { return is_some(); }

    data_t &operator*()
    {
        if (is_some()) {
            return *((pointer_t)(m_data));
        }

        throw std::logic_error("Option is None.");
    }

    data_t const &operator*() const
    {
        if (is_some()) {
            return *((pointer_t)(m_data));
        }

        throw std::logic_error("Option is None.");
    }

    bool operator==(const Option<T> &rhs) const
    {
        return (is_none() && rhs.is_none()) ||
               (is_some() && rhs.is_some() && ((*(*this)) == (*rhs)));
    }

    bool operator<(const Option<T> &rhs) const
    {
        return rhs.is_some() && (is_none() || ((*(*this)) < (*rhs)));
    }

    bool operator!=(const Option<T> &rhs) { return *this != (rhs); }

private:
    template<class... Args>
    void create(Args &&...args)
    {
        m_data = new T(std::forward<Args>(args)...);
        m_flag = true;
    }

    void destroy()
    {
        if (m_flag) {
            m_flag = false;

            delete m_data;
            m_data = nullptr;
        }
    }

    void assign(const Option &other)
    {
        if (other.is_some()) {
            copy(*((pointer_t)(other.m_data)));
            m_flag = true;
        } else {
            destroy();
        }
    }

    void assign(Option &&other)
    {
        if (other.is_some()) {
            move(std::move(*((pointer_t)(other.m_data))));
            m_flag = true;

            other.destroy();
        } else {
            destroy();
        }
    }

    void move(data_t &&val)
    {
        destroy();
        m_data = new T(std::move(*((pointer_t)(&val))));
    }

    void copy(const data_t &val)
    {
        destroy();
        m_data = new T(*((pointer_t)(&val)));
    }

private:
    bool      m_flag;
    pointer_t m_data;
};

} // namespace mini_std