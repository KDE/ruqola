/*
   Copyright (c) 2020 Milian Wolff <mail@milianw.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef LRUCACHE_H
#define LRUCACHE_H

#include <array>
#include <algorithm>

template<typename Key, typename Value, size_t N>
class LRUCache
{
public:
    struct Entry
    {
        Key key;
        Value value;
        bool operator==(const Key &rhs) const
        {
            return key == rhs;
        }
    };
    using Entries = std::array<Entry, N>;
    using value_type = typename Entries::value_type;
    using size_type = typename Entries::size_type;
    using difference_type = typename Entries::difference_type;
    // only const access
    using reference = typename Entries::const_reference;
    using const_reference = typename Entries::const_reference;
    using pointer = typename Entries::const_pointer;
    using iterator = typename Entries::const_iterator;
    using const_iterator = typename Entries::const_iterator;

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
        if (mNumEntries < mEntries.size()) {
            // open up a new slot
            ++mNumEntries;
        }

        // right shift to make space at the front
        std::rotate(mEntries.begin(),
                    std::next(mEntries.begin(), mNumEntries - 1),
                    std::next(mEntries.begin(), mNumEntries));

        // insert up front
        mEntries.front() = {std::move(key), std::move(value)};
    }

private:
    Entries mEntries;
    std::size_t mNumEntries = 0;
};

#endif
