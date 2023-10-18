/*
   SPDX-FileCopyrightText: 2020 Milian Wolff <mail@milianw.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <algorithm>
#include <vector>

template<typename Key, typename Value>
class LRUCache
{
public:
    struct Entry {
        Key key;
        Value value;
        bool operator==(const Key &rhs) const
        {
            return key == rhs;
        }
    };
    using Entries = std::vector<Entry>;
    using value_type = typename Entries::value_type;
    using size_type = typename Entries::size_type;
    using difference_type = typename Entries::difference_type;
    // only const access
    using reference = typename Entries::const_reference;
    using const_reference = typename Entries::const_reference;
    using pointer = typename Entries::const_pointer;
    using iterator = typename Entries::const_iterator;
    using const_iterator = typename Entries::const_iterator;

    void setMaxEntries(int maxEntries)
    {
        mMaxEntries = maxEntries;
    }

    std::size_t size() const
    {
        return mNumEntries;
    }

    const_iterator begin() const
    {
        return mEntries.begin();
    }

    const_iterator end() const
    {
        return std::next(mEntries.begin(), mNumEntries);
    }

    const_iterator find(const Key &key)
    {
        // using non-const iterators here since we will re-insert when we find
        const auto begin = mEntries.begin();
        const auto end = std::next(mEntries.begin(), mNumEntries);
        auto it = std::find(begin, end, key);
        if (it == begin || it == end) { // not found or already the last recently used one
            return it;
        }

        // rotate to mark entry as last recently used one
        std::rotate(begin, it, it + 1);
        return mEntries.cbegin();
    }

    void insert(Key key, Value value)
    {
        if (mMaxEntries == -1 || (mNumEntries < static_cast<size_t>(mMaxEntries))) {
            // open up a new slot
            ++mNumEntries;
            mEntries.resize(mNumEntries);
        }

        // right shift to make space at the front
        std::rotate(mEntries.begin(), std::next(mEntries.begin(), mNumEntries - 1), std::next(mEntries.begin(), mNumEntries));

        // insert up front
        mEntries.front() = {std::move(key), std::move(value)};
    }

    bool remove(const Key &key)
    {
        const auto begin = mEntries.begin();
        const auto end = std::next(mEntries.begin(), mNumEntries);
        auto it = std::find(begin, end, key);
        if (it == end) { // not found or already the last recently used one
            return false;
        }

        std::move(std::next(it), end, it);
        --mNumEntries;
        return true;
    }

    void clear()
    {
        mNumEntries = 0;
        Entries empty;
        mEntries.swap(empty);
    }

private:
    Entries mEntries;
    std::size_t mNumEntries = 0;
    int mMaxEntries = -1;
};
