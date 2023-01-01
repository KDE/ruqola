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

QDebug operator<<(QDebug d, const MessagePinned &t)
{
    d << "isPinned " << t.pinned();
    d << "pinnedby " << t.pinnedBy();
    return d;
}
