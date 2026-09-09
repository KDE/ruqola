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
 * more than that object to parse itself.
 *
 * A subclass whose elements need more than that (a validity check, an extra parse argument…)
 * still inherits the storage and the counters: it parses the array itself and calls
 * parseFirstPageCounters()/parseNextPageCounters() around its own loop.
 */

template<typename T, auto ParseElement>
class PaginatedInfoList
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

    [[nodiscard]] bool operator==(const PaginatedInfoList &other) const = default;

protected:
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

    QList<T> mList;
    int mLoadedCount = 0;
    int mOffset = 0;
    int mTotal = 0;
};
