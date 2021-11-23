/*
   Copyright (c) 2019-2021 Laurent Montel <montel@kde.org>

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
