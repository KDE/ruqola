/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagepinned.h"
#include "ruqola_message_memory_debug.h"

#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
MessagePinned::MessagePinned()

{
    qCDebug(RUQOLA_MESSAGE_MEMORY_LOG) << " MessagePinned created " << this;
}

MessagePinned::MessagePinned(const MessagePinned &other)
    : QSharedData(other)
{
    qCDebug(RUQOLA_MESSAGE_MEMORY_LOG) << " MessagePinned created " << this;
    mPinned = other.mPinned;
    mPinnedBy = other.mPinnedBy;
}

MessagePinned::~MessagePinned()
{
    qCDebug(RUQOLA_MESSAGE_MEMORY_LOG) << " MessagePinned deleted " << this;
}

bool MessagePinned::isValid() const
{
    return !mPinnedBy.isEmpty();
}

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
    mPinned = o.value("pinned"_L1).toBool();
    mPinnedBy = o.value("pinnedBy"_L1).toObject().value("username"_L1).toString();
}

QJsonObject MessagePinned::serialize(const MessagePinned &messagePinned)
{
    QJsonObject o;
    o["pinned"_L1] = messagePinned.pinned();
    if (!messagePinned.pinnedBy().isEmpty()) {
        o["pinnedBy"_L1] = messagePinned.pinnedBy();
    }
    return o;
}

QDebug operator<<(QDebug d, const MessagePinned &t)
{
    d.space() << "isPinned" << t.pinned();
    d.space() << "pinnedby" << t.pinnedBy();
    return d;
}

MessagePinned *MessagePinned::deserialize(const QJsonObject &o)
{
    MessagePinned *pinned = new MessagePinned;
    pinned->setPinned(o["pinned"_L1].toBool());
    pinned->setPinnedBy(o["pinnedBy"_L1].toString());
    return pinned;
}
