/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "accountroomsettings.h"
using namespace Qt::Literals::StringLiterals;
AccountRoomSettings::AccountRoomSettings() = default;

AccountRoomSettings::~AccountRoomSettings() = default;

void AccountRoomSettings::remove(const QByteArray &roomId)
{
    mPendingTypedTexts.remove(roomId);
}

void AccountRoomSettings::add(const QByteArray &roomId, const PendingTypedInfo &info)
{
    mPendingTypedTexts[roomId] = info;
}

bool AccountRoomSettings::hasPendingMessageTyped(const QByteArray &roomId) const
{
    return mPendingTypedTexts.value(roomId).hasPendingMessageTyped();
}

AccountRoomSettings::PendingTypedInfo AccountRoomSettings::value(const QByteArray &roomId)
{
    return mPendingTypedTexts.take(roomId);
}

bool AccountRoomSettings::isEmpty() const
{
    return mPendingTypedTexts.isEmpty();
}

bool AccountRoomSettings::PendingTypedInfo::hasPendingMessageTyped() const
{
    return !text.isEmpty();
}

bool AccountRoomSettings::PendingTypedInfo::operator==(const AccountRoomSettings::PendingTypedInfo &other) const
{
    return text == other.text && messageIdBeingEdited == other.messageIdBeingEdited && threadMessageId == other.threadMessageId
        && quotePermalink == other.quotePermalink && quoteText == other.quoteText && scrollbarPosition == other.scrollbarPosition;
}

QJsonObject AccountRoomSettings::PendingTypedInfo::serialize(const PendingTypedInfo &pendingTypedInfo)
{
    QJsonObject obj;
    if (!pendingTypedInfo.text.isEmpty()) {
        obj["text"_L1] = pendingTypedInfo.text;
    }
    if (!pendingTypedInfo.messageIdBeingEdited.isEmpty()) {
        obj["messageIdBeingEdited"_L1] = QString::fromLatin1(pendingTypedInfo.messageIdBeingEdited);
    }
    if (!pendingTypedInfo.threadMessageId.isEmpty()) {
        obj["threadMessageId"_L1] = QString::fromLatin1(pendingTypedInfo.threadMessageId);
    }
    if (!pendingTypedInfo.quotePermalink.isEmpty()) {
        obj["quotePermalink"_L1] = pendingTypedInfo.quotePermalink;
    }
    if (!pendingTypedInfo.quoteText.isEmpty()) {
        obj["quoteText"_L1] = pendingTypedInfo.quoteText;
    }
    if (pendingTypedInfo.scrollbarPosition != -1) {
        obj["scrollbarPosition"_L1] = pendingTypedInfo.scrollbarPosition;
    }

    return obj;
}

AccountRoomSettings::PendingTypedInfo AccountRoomSettings::PendingTypedInfo::deserialize(const QJsonObject &o)
{
    AccountRoomSettings::PendingTypedInfo pendingTypedInfo;
    pendingTypedInfo.text = o.value("text"_L1).toString();
    pendingTypedInfo.messageIdBeingEdited = o.value("messageIdBeingEdited"_L1).toString().toLatin1();
    pendingTypedInfo.threadMessageId = o.value("threadMessageId"_L1).toString().toLatin1();
    pendingTypedInfo.quotePermalink = o.value("quotePermalink"_L1).toString();
    pendingTypedInfo.quoteText = o.value("quoteText"_L1).toString();
    pendingTypedInfo.scrollbarPosition = o.value("scrollbarPosition"_L1).toInt(-1);
    return pendingTypedInfo;
}

bool AccountRoomSettings::PendingTypedInfo::isValid() const
{
    return !text.isEmpty() || !messageIdBeingEdited.isEmpty() || (scrollbarPosition != -1) || !threadMessageId.isEmpty() || !quotePermalink.isEmpty()
        || !quoteText.isEmpty();
}

QDebug operator<<(QDebug d, const AccountRoomSettings::PendingTypedInfo &t)
{
    d.space() << "text" << t.text;
    d.space() << "messageIdBeingEdited" << t.messageIdBeingEdited;
    d.space() << "scrollbarPosition" << t.scrollbarPosition;
    d.space() << "threadMessageId" << t.threadMessageId;
    d.space() << "quotePermalink" << t.quotePermalink;
    d.space() << "quoteText" << t.quoteText;
    return d;
}
