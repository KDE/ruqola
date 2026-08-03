/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageencrypted.h"
#include "ruqola_encryption_debug.h"

#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
MessageEncrypted::MessageEncrypted()

{
    qCDebug(RUQOLA_ENCRYPTION_LOG) << " MessageEncrypted created " << this;
}

MessageEncrypted::MessageEncrypted(const MessageEncrypted &other)
    : QSharedData(other)
{
    qCDebug(RUQOLA_ENCRYPTION_LOG) << " MessageEncrypted created " << this;
}

MessageEncrypted::~MessageEncrypted()
{
    qCDebug(RUQOLA_ENCRYPTION_LOG) << " MessageEncrypted deleted " << this;
}

bool MessageEncrypted::isValid() const
{
    return false;
    // return !mPinnedBy.isEmpty();
}

bool MessageEncrypted::operator==(const MessageEncrypted &other) const
{
    // TODO
    return false;
}

void MessageEncrypted::parse(const QJsonObject &o)
{
    // TODO
}

QJsonObject MessageEncrypted::serialize(const MessageEncrypted &messagePinned)
{
    QJsonObject o;
    // TODO
    return o;
}

QDebug operator<<(QDebug d, const MessageEncrypted &t)
{
    // TODO
    return d;
}

MessageEncrypted *MessageEncrypted::deserialize(const QJsonObject &o)
{
    MessageEncrypted *encrypted = new MessageEncrypted;
    // TODO
    return encrypted;
}
