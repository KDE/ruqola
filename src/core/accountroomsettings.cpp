/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "accountroomsettings.h"
#include "localdatabase/localdatabasemanager.h"
#include "rocketchataccount.h"
#include <QJsonArray>

using namespace Qt::Literals::StringLiterals;
AccountRoomSettings::AccountRoomSettings(RocketChatAccount *account)
    : mRocketChatAccount(account)
{
}

AccountRoomSettings::~AccountRoomSettings() = default;

void AccountRoomSettings::remove(const QByteArray &roomId)
{
    if (mPendingTypedTexts.remove(roomId) > 0) {
        mRocketChatAccount->localDatabaseManager()->deleteRoomPendingTypedInfo(mRocketChatAccount->accountName(), roomId);
    }
}

void AccountRoomSettings::add(const QByteArray &roomId, const PendingTypedInfo &info)
{
    mPendingTypedTexts[roomId] = info;
    mRocketChatAccount->localDatabaseManager()->updateRoomPendingTypedInfo(mRocketChatAccount->accountName(), roomId, info);
}

bool AccountRoomSettings::hasPendingMessageTyped(const QByteArray &roomId) const
{
    return mPendingTypedTexts.value(roomId).hasPendingMessageTyped();
}

AccountRoomSettings::PendingTypedInfo AccountRoomSettings::value(const QByteArray &roomId)
{
    const auto pending = mPendingTypedTexts.take(roomId);
    if (pending.isValid()) {
        mRocketChatAccount->localDatabaseManager()->deleteRoomPendingTypedInfo(mRocketChatAccount->accountName(), roomId);
    }
    return pending;
}

bool AccountRoomSettings::isEmpty() const
{
    return mPendingTypedTexts.isEmpty();
}

void AccountRoomSettings::loadRoomPendingTypedInfo()
{
    setPendingTypedTexts(mRocketChatAccount->localDatabaseManager()->loadRoomPendingTypedInfo(mRocketChatAccount->accountName()));
}

void AccountRoomSettings::setPendingTypedTexts(const QMap<QByteArray, PendingTypedInfo> &newPendingTypedTexts)
{
    mPendingTypedTexts = newPendingTypedTexts;
}

bool AccountRoomSettings::PendingTypedInfo::hasPendingMessageTyped() const
{
    return !text.isEmpty();
}

bool AccountRoomSettings::PendingTypedInfo::operator==(const AccountRoomSettings::PendingTypedInfo &other) const
{
    return text == other.text && messageIdBeingEdited == other.messageIdBeingEdited && threadMessageId == other.threadMessageId
        && quotePermalink == other.quotePermalink && quoteText == other.quoteText && scrollbarPosition == other.scrollbarPosition
        && pendingAttachmentInfos == other.pendingAttachmentInfos;
}

QJsonObject AccountRoomSettings::PendingAttachmentInfo::serialize(const AccountRoomSettings::PendingAttachmentInfo &att)
{
    QJsonObject obj;
    if (!att.fileUrl.isEmpty()) {
        obj["fileUrl"_L1] = att.fileUrl.toString();
    }
    if (!att.fileName.isEmpty()) {
        obj["fileName"_L1] = att.fileName;
    }
    if (!att.name.isEmpty()) {
        obj["name"_L1] = att.name;
    }
    if (!att.alternativeText.isEmpty()) {
        obj["alternativeText"_L1] = att.alternativeText;
    }
    return obj;
}

AccountRoomSettings::PendingAttachmentInfo AccountRoomSettings::PendingAttachmentInfo::deserialize(const QJsonObject &o)
{
    AccountRoomSettings::PendingAttachmentInfo att;
    att.fileUrl = QUrl(o["fileUrl"_L1].toString());
    att.fileName = o["fileName"_L1].toString();
    att.name = o["name"_L1].toString();
    att.alternativeText = o["alternativeText"_L1].toString();
    return att;
}

bool AccountRoomSettings::PendingAttachmentInfo::operator==(const PendingAttachmentInfo &other) const = default;

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
    if (!pendingTypedInfo.pendingAttachmentInfos.isEmpty()) {
        QJsonArray array;
        for (const auto &att : std::as_const(pendingTypedInfo.pendingAttachmentInfos)) {
            array += AccountRoomSettings::PendingAttachmentInfo::serialize(att);
        }
        obj["pendingAttachmentInfos"_L1] = array;
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
    if (o.contains("pendingAttachmentInfos"_L1)) {
        const QJsonArray array = o.value("pendingAttachmentInfos"_L1).toArray();
        QList<AccountRoomSettings::PendingAttachmentInfo> lst;
        const int arraySize = array.size();
        lst.reserve(arraySize);
        for (int i = 0; i < arraySize; ++i) {
            lst.append(AccountRoomSettings::PendingAttachmentInfo::deserialize(array.at(i).toObject()));
        }
        pendingTypedInfo.pendingAttachmentInfos = lst;
    }
    return pendingTypedInfo;
}

bool AccountRoomSettings::PendingTypedInfo::isValid() const
{
    return !text.isEmpty() || !messageIdBeingEdited.isEmpty() || (scrollbarPosition != -1) || !threadMessageId.isEmpty() || !quotePermalink.isEmpty()
        || !quoteText.isEmpty() || !pendingAttachmentInfos.isEmpty();
}

QDebug operator<<(QDebug d, const AccountRoomSettings::PendingTypedInfo &t)
{
    d.space() << "text" << t.text;
    d.space() << "messageIdBeingEdited" << t.messageIdBeingEdited;
    d.space() << "scrollbarPosition" << t.scrollbarPosition;
    d.space() << "threadMessageId" << t.threadMessageId;
    d.space() << "quotePermalink" << t.quotePermalink;
    d.space() << "quoteText" << t.quoteText;
    d.space() << "pendingAttachmentInfos" << t.pendingAttachmentInfos;
    return d;
}

QDebug operator<<(QDebug d, const AccountRoomSettings::PendingAttachmentInfo &t)
{
    d.space() << "fileUrl" << t.fileUrl;
    d.space() << "fileName" << t.fileName;
    d.space() << "name" << t.name;
    d.space() << "alternativeText" << t.alternativeText;
    return d;
}
