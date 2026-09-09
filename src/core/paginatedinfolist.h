/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QLatin1StringView>
#include <QList>

#include <functional>
#include <utility>

class QJsonValue;

namespace PaginatedInfoListPrivate
{
// The template below is instantiated outside libruqolacore too, so it can't reach RUQOLA_LOG
// directly: the logging category lives in this library.
LIBRUQOLACORE_EXPORT void warnInvalidIndex(int index);
LIBRUQOLACORE_EXPORT void warnInvalidElement(QLatin1StringView arrayKey, const QJsonValue &value);
}

/**
 * Rocket.Chat returns paginated lists as
 * @code
 * { "count": n, "offset": n, "total": n, "<arrayKey>": [ … ] }
 * @endcode
 * and every "<Something>Infos" container in Ruqola stores such a reply the same way, so the
 * storage, the pagination counters and the parsing live here.
 *
 * This class holds the storage and the counters only: it is what a subclass whose elements need
 * more than their own QJsonObject to be parsed (a validity check, the server's role list, an
 * EmojiManager…) derives from, parsing the array itself and calling
 * parseFirstPageCounters()/parseNextPageCounters() around its own loop. A subclass whose elements
 * do parse themselves derives from PaginatedInfoList below instead.
 */
template<typename T>
class PaginatedInfoListBase
{
public:
    [[nodiscard]] bool isEmpty() const
    {
        return mList.isEmpty();
    }

    [[nodiscard]] int count() const
    {
        return mList.count();
    }

    void clear()
    {
        mList.clear();
    }

    [[nodiscard]] T at(int index) const
    {
        if (index < 0 || index >= mList.count()) {
            PaginatedInfoListPrivate::warnInvalidIndex(index);
            return {};
        }
        return mList.at(index);
    }

    T takeAt(int index)
    {
        if (index < 0 || index >= mList.count()) {
            PaginatedInfoListPrivate::warnInvalidIndex(index);
            return {};
        }
        return mList.takeAt(index);
    }

    [[nodiscard]] const QList<T> &list() const
    {
        return mList;
    }

    void setList(QList<T> newList)
    {
        mList = std::move(newList);
    }

    // Number of elements the server reported so far, accumulated over the pages loaded: the
    // offset the next page starts at, as opposed to count(), which is how many elements are
    // actually held.
    [[nodiscard]] int loadedCount() const
    {
        return mLoadedCount;
    }

    void setLoadedCount(int newLoadedCount)
    {
        mLoadedCount = newLoadedCount;
    }

    [[nodiscard]] int offset() const
    {
        return mOffset;
    }

    void setOffset(int newOffset)
    {
        mOffset = newOffset;
    }

    [[nodiscard]] int total() const
    {
        return mTotal;
    }

    void setTotal(int newTotal)
    {
        mTotal = newTotal;
    }

    [[nodiscard]] bool operator==(const PaginatedInfoListBase &other) const = default;

protected:
    void parseFirstPageCounters(const QJsonObject &obj)
    {
        mList.clear();
        mLoadedCount = obj[QLatin1StringView("count")].toInt();
        mOffset = obj[QLatin1StringView("offset")].toInt();
        mTotal = obj[QLatin1StringView("total")].toInt();
        mList.reserve(mLoadedCount);
    }

    void parseNextPageCounters(const QJsonObject &obj)
    {
        mLoadedCount += obj[QLatin1StringView("count")].toInt();
        mOffset = obj[QLatin1StringView("offset")].toInt();
        mTotal = obj[QLatin1StringView("total")].toInt();
    }

    QList<T> mList;
    int mLoadedCount = 0;
    int mOffset = 0;
    int mTotal = 0;
};

/**
 * A subclass only names the element type and how one element is parsed, then exposes its
 * domain-named entry points, which is where the key holding the array belongs:
 * @code
 * class DeviceInfos : public PaginatedInfoList<DeviceInfo, &DeviceInfo::parseDeviceInfo>
 * {
 * public:
 *     void parseDeviceInfos(const QJsonObject &obj) { parseInfos(obj, QLatin1StringView("sessions")); }
 * };
 * @endcode
 *
 * @p ParseElement is anything std::invoke() accepts as parse(element, jsonObject): a member
 * function of @p T taking the element's QJsonObject, or a free function when the element needs
 * more than that object to parse itself. A subclass that parses its own elements derives from
 * PaginatedInfoListBase instead: naming a parse function it cannot use would still have to
 * compile, as MSVC instantiates every member of an exported class.
 */
template<typename T, auto ParseElement>
class PaginatedInfoList : public PaginatedInfoListBase<T>
{
protected:
    using Base = PaginatedInfoListBase<T>;
    using Base::mList;
    using Base::parseFirstPageCounters;
    using Base::parseNextPageCounters;

    // First page: replaces what is held.
    void parseInfos(const QJsonObject &obj, QLatin1StringView arrayKey)
    {
        parseFirstPageCounters(obj);
        parseElements(obj, arrayKey);
    }

    // Next page: appends to what is held.
    void parseMoreInfos(const QJsonObject &obj, QLatin1StringView arrayKey)
    {
        parseNextPageCounters(obj);
        parseElements(obj, arrayKey);
    }

    // Appends the elements found under @p arrayKey, leaving the counters alone.
    void parseElements(const QJsonObject &obj, QLatin1StringView arrayKey)
    {
        const QJsonArray array = obj[arrayKey].toArray();
        mList.reserve(mList.count() + array.count());
        for (const auto &current : array) {
            if (current.type() == QJsonValue::Object) {
                std::invoke(ParseElement, mList.emplace_back(), current.toObject());
            } else {
                PaginatedInfoListPrivate::warnInvalidElement(arrayKey, current);
            }
        }
    }
};
