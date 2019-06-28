/*
   Copyright (c) 2018-2019 Montel Laurent <montel@kde.org>

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

#include "searchmessagemodel.h"
#include "ruqola_debug.h"
#include "rocketchataccount.h"
#include "textconverter.h"

#include <QDateTime>
#include <QJsonArray>

SearchMessageModel::SearchMessageModel(RocketChatAccount *account, QObject *parent)
    : QAbstractListModel(parent), mRocketChatAccount(account)
{
    mTextConverter = new TextConverter(mRocketChatAccount ? mRocketChatAccount->emojiManager() : nullptr);
}

SearchMessageModel::~SearchMessageModel()
{
    delete mTextConverter;
}

QVariant SearchMessageModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mSearchMessages.count()) {
        return {};
    }
    const int idx = index.row();
    const SearchMessage &message = mSearchMessages.at(idx);
    switch (role) {
    case SearchMessageModel::Username:
        return message.username();
    case SearchMessageModel::OriginalMessage:
        return message.text();
    case SearchMessageModel::MessageConvertedText:
        //TODO improve it.
        if (message.messageType() == Message::System) {
            return message.messageTypeText();
        } else {
#if 0
            if (mRoom && mRoom->userIsIgnored(message.userId())) {
                return QString(QStringLiteral("<i>") + i18n("Ignored Message") + QStringLiteral("</i>"));
            }
#endif
            const QString userName = mRocketChatAccount ? mRocketChatAccount->userName() : QString();
            return convertMessageText(message.text(), userName);
        }

    case SearchMessageModel::Timestamp:
        return message.displayTime();
    case SearchMessageModel::UserId:
        return message.userId();
    case SearchMessageModel::SystemMessageType:
        return message.systemMessageType();
    case SearchMessageModel::MessageId:
        return message.messageId();
    case SearchMessageModel::Alias:
        return message.alias();
    case SearchMessageModel::MessageType:
        return message.messageType();
    case SearchMessageModel::Avatar:
        return message.avatar();
    case SearchMessageModel::EditedAt:
        return message.editedAt();
    case SearchMessageModel::EditedByUserName:
        return message.editedByUsername();
    case SearchMessageModel::Attachments:
    {
        QVariantList lst;
        lst.reserve(message.attachements().count());
        const auto attachs = message.attachements();
        for (const MessageAttachment &att : attachs) {
            lst.append(QVariant::fromValue(att));
        }
        return lst;
    }
    case SearchMessageModel::Urls:
    {
        QVariantList lst;
        lst.reserve(message.urls().count());
        const auto urls = message.urls();
        for (const MessageUrl &url : urls) {
            lst.append(QVariant::fromValue(url));
        }
        return lst;
    }
    case SearchMessageModel::Date:
    {
        QDateTime currentDate;
        currentDate.setMSecsSinceEpoch(message.timeStamp());
        return currentDate.date().toString();
    }
    case SearchMessageModel::CanEditMessage:
        return false;
    case SearchMessageModel::Starred:
        return message.starred();
    case SearchMessageModel::UsernameUrl:
    {
        const QString username = message.username();
        if (username.isEmpty()) {
            return {};
        }
        return QStringLiteral("<a href=\'ruqola:/user/%1\'>@%1</a>").arg(message.username());
    }
    case SearchMessageModel::Roles:
    {
        //const QString str = roomRoles(message.userId()).join(QLatin1Char(','));
        //return str;
        return QString();
    }
    case SearchMessageModel::Reactions:
    {
        QVariantList lst;
        const auto reactions = message.reactions().reactions();
        lst.reserve(reactions.count());
        for (const Reaction &react : reactions) {
            //Convert reactions
            lst.append(QVariant::fromValue(react));
        }
        return lst;
    }
    case SearchMessageModel::Ignored:
        return false;//mRoom && mRoom->userIsIgnored(message.userId());
    case SearchMessageModel::Pinned:
        return message.messagePinned().pinned();
    case SearchMessageModel::DiscussionCount:
        return message.discussionCount();
    case SearchMessageModel::DiscussionRoomId:
        return message.discussionRoomId();
    case SearchMessageModel::DiscussionLastMessage:
        return message.discussionLastMessage();
    case SearchMessageModel::ThreadCount:
        return message.threadCount();
    case SearchMessageModel::ThreadLastMessage:
        return message.threadLastMessage();
    case SearchMessageModel::ThreadMessageId:
        return message.threadMessageId();
    case SearchMessageModel::ThreadMessagePreview:
        return QString(); //threadMessagePreview(message.threadMessageId());
    case SearchMessageModel::Groupable:
        return message.groupable();
    case SearchMessageModel::SortByTimeStamp:
        return message.timeStamp();
    }

    return {};

}

QHash<int, QByteArray> SearchMessageModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[OriginalMessage] = QByteArrayLiteral("originalMessage");
    roles[MessageConvertedText] = QByteArrayLiteral("messageConverted");
    roles[Username] = QByteArrayLiteral("username");
    roles[Timestamp] = QByteArrayLiteral("timestamp");
    roles[UserId] = QByteArrayLiteral("userID");
    roles[SystemMessageType] = QByteArrayLiteral("type");
    roles[MessageId] = QByteArrayLiteral("messageID");
    roles[RoomId] = QByteArrayLiteral("roomID");
    roles[UpdatedAt] = QByteArrayLiteral("updatedAt");
    roles[EditedAt] = QByteArrayLiteral("editedAt");
    roles[EditedByUserName] = QByteArrayLiteral("editedByUsername");
    roles[EditedByUserId] = QByteArrayLiteral("editedByUserID");
    roles[Alias] = QByteArrayLiteral("alias");
    roles[Avatar] = QByteArrayLiteral("avatar");
    roles[Groupable] = QByteArrayLiteral("groupable");
    roles[MessageType] = QByteArrayLiteral("messagetype");
    roles[Attachments] = QByteArrayLiteral("attachments");
    roles[Urls] = QByteArrayLiteral("urls");
    roles[Date] = QByteArrayLiteral("date");
    roles[CanEditMessage] = QByteArrayLiteral("canEditMessage");
    roles[Starred] = QByteArrayLiteral("starred");
    roles[UsernameUrl] = QByteArrayLiteral("usernameurl");
    roles[Roles] = QByteArrayLiteral("roles");
    roles[Reactions] = QByteArrayLiteral("reactions");
    roles[Ignored] = QByteArrayLiteral("userIsIgnored");
    roles[Pinned] = QByteArrayLiteral("pinned");
    roles[DiscussionCount] = QByteArrayLiteral("discussionCount");
    roles[DiscussionRoomId] = QByteArrayLiteral("discussionRoomId");
    roles[DiscussionLastMessage] = QByteArrayLiteral("discussionLastMessage");
    roles[ThreadCount] = QByteArrayLiteral("threadCount");
    roles[ThreadLastMessage] = QByteArrayLiteral("threadLastMessage");
    roles[ThreadMessageId] = QByteArrayLiteral("threadMessageId");
    roles[ThreadMessagePreview] = QByteArrayLiteral("threadMessagePreview");
    return roles;
}

int SearchMessageModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mSearchMessages.count();
}

void SearchMessageModel::parseResult(const QJsonObject &obj, bool restApi)
{
    QVector<SearchMessage> messages;
    const QJsonArray messagesObj = obj.value(QLatin1String("messages")).toArray();
    for (int i = 0; i < messagesObj.size(); i++) {
        const QJsonObject o = messagesObj.at(i).toObject();
        SearchMessage msg;
        msg.parseMessage(o, restApi);
        //Verify that it's valid
        messages.append(msg);
    }
    setMessages(messages);
}

void SearchMessageModel::clear()
{
    if (!mSearchMessages.isEmpty()) {
        beginRemoveRows(QModelIndex(), 0, mSearchMessages.count() - 1);
        mSearchMessages.clear();
        endRemoveRows();
    }
}

void SearchMessageModel::setMessages(const QVector<SearchMessage> &messages)
{
    if (rowCount() != 0) {
        beginRemoveRows(QModelIndex(), 0, mSearchMessages.count() - 1);
        mSearchMessages.clear();
        endRemoveRows();
    }
    if (!messages.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, messages.count() - 1);
        mSearchMessages = messages;
        endInsertRows();
    }
}

QString SearchMessageModel::convertMessageText(const QString &str, const QString &userName) const
{
    return mTextConverter->convertMessageText(str, userName, {}/*mMentions->mentions() TODO*/);
}
