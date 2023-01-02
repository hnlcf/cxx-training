// Copyright (c) 2022 Changfeng Lou. All rights reserved.

#pragma once

#include <list>
#include <stdexcept>
#include <unordered_map>
#include <utility>

namespace mini_std {

/// Least Recently Used Cache
template<class Key, class Value>
class LRUCache
{
public:
    typedef typename std::pair<Key, Value>          kv_pair_t;
    typedef typename std::list<kv_pair_t>::iterator list_iterator_t;

    explicit LRUCache(std::size_t capacity)
      : m_capacity(capacity)
    {
    }

    const Value &get(const Key &key)
    {
        auto target = m_table.find(key);
        if (target == m_table.end()) {
            throw std::range_error("There is no such key in cache");
        }

        // delete the target key-value pair and insert it to the head
        m_data.splice(m_data.begin(), m_data, target->second);

        return target->second->second;
    }

    void put(const Key &key, const Value &value)
    {
        // update the key-value pair
        auto entry = std::make_pair(key, value);
        m_data.push_front(entry);

        // delete the old pair and table entry if exists
        auto target = m_table.find(key);
        if (target != m_table.end()) {
            m_data.erase(target->second);
            m_table.erase(target);
        }
        m_table[key] = m_data.begin();

        // delete the last pair and table entry if reaches capacity
        if (size() >= m_capacity) {
            m_table.erase(m_data.back().first);
            m_data.pop_back();
        }
    }

    bool isExist(const Key &key) const { return m_table.find(key) != m_table.end(); }

    std::size_t size() const { return m_data.size(); }

private:
    std::list<kv_pair_t>                     m_data;
    std::unordered_map<Key, list_iterator_t> m_table;
    std::size_t                              m_capacity;
};

} // namespace mini_std