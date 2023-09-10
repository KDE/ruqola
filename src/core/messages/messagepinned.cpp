/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagepinned.h"
#include <QJsonObject>
MessagePinned::MessagePinned() = default;

bool MessagePinned::pinned() const
{
    return mPinned;
}

void MessagePinned::setPinned(bool pinned)
{
    mPinned = pinned;
}

QString MessagePinned::pinnedBy() const
{
    return mPinnedBy;
}

void MessagePinned::setPinnedBy(const QString &pinnedBy)
{
    mPinnedBy = pinnedBy;
}

bool MessagePinned::operator==(const MessagePinned &other) const
{
    return (mPinned == other.pinned()) && (mPinnedBy == other.pinnedBy());
}

void MessagePinned::parse(const QJsonObject &o)
{
    mPinned = o.value(QLatin1String("pinned")).toBool();
    mPinnedBy = o.value(QLatin1String("pinnedBy")).toObject().value(QLatin1String("username")).toString();
}

QJsonObject MessagePinned::serialize(const MessagePinned &messagePinned)
{
    QJsonObject o;
    o[QLatin1String("pinned")] = messagePinned.pinned();
    if (!messagePinned.pinnedBy().isEmpty()) {
        o[QLatin1String("pinnedBy")] = messagePinned.pinnedBy();
    }
    return o;
}

QDebug operator<<(QDebug d, const MessagePinned &t)
{
    d << "isPinned " << t.pinned();
    d << "pinnedby " << t.pinnedBy();
    return d;
}

MessagePinned MessagePinned::deserialize(const QJsonObject &o)
{
    MessagePinned pinned;
    pinned.setPinned(o[QLatin1String("pinned")].toBool());
    pinned.setPinnedBy(o[QLatin1String("pinnedBy")].toString());
    return pinned;
}
