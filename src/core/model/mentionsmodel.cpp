/*
   Copyright (c) 2019-2020 Laurent Montel <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "mentionsmodel.h"
#include "mentions.h"
#include "rocketchataccount.h"
#include "textconverter.h"

#include <QDateTime>

MentionsModel::MentionsModel(RocketChatAccount *account, QObject *parent)
    : QAbstractListModel(parent)
    , mRocketChatAccount(account)
{
    mMentions = new Mentions;
    mTextConverter = new TextConverter(mRocketChatAccount ? mRocketChatAccount->emojiManager() : nullptr);
}

MentionsModel::~MentionsModel()
{
    delete mMentions;
    delete mTextConverter;
}

void MentionsModel::initialize()
{
    mRoomId.clear();
    setHasFullList(false);
    mLoadMoreMentionsInProgress = false;
}

int MentionsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mMentions->count();
}

QVariant MentionsModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mMentions->count()) {
        return {};
    }
    const int idx = index.row();
    const Message &message = mMentions->at(idx);
    switch (role) {
    case MentionsModel::Username:
        return message.username();
    case MentionsModel::OriginalMessage:
        return message.text();
    case MentionsModel::MessageConvertedText:
        //TODO improve it.
        if (message.messageType() == Message::System) {
            return message.systemMessageText();
        } else {
#if 0
            if (mRoom && mRoom->userIsIgnored(message.userId())) {
                return QString(QStringLiteral("<i>") + i18n("Ignored Message") + QStringLiteral("</i>"));
            }
#endif
            const QString userName = mRocketChatAccount ? mRocketChatAccount->userName() : QString();
            return convertMessageText(message.text(), userName);
        }

    case MentionsModel::Timestamp:
        return message.displayTime();
    case MentionsModel::UserId:
        return message.userId();
    case MentionsModel::SystemMessageType:
        return message.systemMessageType();
    case MentionsModel::MessageId:
        return message.messageId();
    case MentionsModel::Alias:
        return message.alias();
    case MentionsModel::MessageType:
        return message.messageType();
    case MentionsModel::Avatar:
        return message.avatar();
    case MentionsModel::EditedAt:
        return message.editedAt();
    case MentionsModel::EditedByUserName:
        return message.editedByUsername();
    case MentionsModel::Attachments:
    {
        QVariantList lst;
        lst.reserve(message.attachements().count());
        const auto attachs = message.attachements();
        for (const MessageAttachment &att : attachs) {
            lst.append(QVariant::fromValue(att));
        }
        return lst;
    }
    case MentionsModel::Urls:
    {
        QVariantList lst;
        lst.reserve(message.urls().count());
        const auto urls = message.urls();
        for (const MessageUrl &url : urls) {
            lst.append(QVariant::fromValue(url));
        }
        return lst;
    }
    case MentionsModel::Date:
    {
        QDateTime currentDate;
        currentDate.setMSecsSinceEpoch(message.timeStamp());
        return currentDate.date().toString();
    }
    case MentionsModel::CanEditMessage:
        return false;
    case MentionsModel::Starred:
        return message.starred();
    case MentionsModel::UsernameUrl:
    {
        const QString username = message.username();
        if (username.isEmpty()) {
            return {};
        }
        return QStringLiteral("<a href=\'ruqola:/user/%1\'>@%1</a>").arg(message.username());
    }
    case MentionsModel::Roles:
        //const QString str = roomRoles(message.userId()).join(QLatin1Char(','));
        //return str;
        return QString();
    case MentionsModel::Reactions:
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
    case MentionsModel::Ignored:
        return false;//mRoom && mRoom->userIsIgnored(message.userId());
    case MentionsModel::Pinned:
        return message.messagePinned().pinned();
    case MentionsModel::DiscussionCount:
        return message.discussionCount();
    case MentionsModel::DiscussionRoomId:
        return message.discussionRoomId();
    case MentionsModel::DiscussionLastMessage:
        return message.discussionLastMessage();
    case MentionsModel::ThreadCount:
        return message.threadCount();
    case MentionsModel::ThreadLastMessage:
        return message.threadLastMessage();
    case MentionsModel::ThreadMessageId:
        return message.threadMessageId();
    case MentionsModel::ThreadMessagePreview:
        return QString(); //threadMessagePreview(message.threadMessageId());
    case MentionsModel::Groupable:
        return message.groupable();
    case MentionsModel::SortByTimeStamp:
        return message.timeStamp();
    case MentionsModel::ShowTranslatedMessage:
        return message.showTranslatedMessage();
    }

    return {};
}

void MentionsModel::setMentions(const Mentions &mentions)
{
    if (rowCount() != 0) {
        beginRemoveRows(QModelIndex(), 0, mMentions->count() - 1);
        mMentions->clear();
        endRemoveRows();
    }
    if (!mentions.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, mentions.count() - 1);
        mMentions->setMentions(mentions.mentions());
        endInsertRows();
    }
    checkFullList();
}

QHash<int, QByteArray> MentionsModel::roleNames() const
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
    roles[ShowTranslatedMessage] = QByteArrayLiteral("showTranslatedMessage");
    return roles;
}

QString MentionsModel::roomId() const
{
    return mRoomId;
}

void MentionsModel::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

void MentionsModel::parseMentions(const QJsonObject &mentionsObj, const QString &roomId)
{
    mRoomId = roomId;
    if (rowCount() != 0) {
        beginRemoveRows(QModelIndex(), 0, mMentions->mentions().count() - 1);
        mMentions->clear();
        endRemoveRows();
    }
    mMentions->parseMentions(mentionsObj);
    if (!mMentions->isEmpty()) {
        beginInsertRows(QModelIndex(), 0, mMentions->mentions().count() - 1);
        endInsertRows();
    }
    checkFullList();
}

Mentions *MentionsModel::mentions() const
{
    return mMentions;
}

int MentionsModel::total() const
{
    if (mMentions) {
        return mMentions->total();
    }
    return -1;
}

void MentionsModel::setHasFullList(bool state)
{
    if (mHasFullList != state) {
        mHasFullList = state;
        Q_EMIT hasFullListChanged();
    }
}

bool MentionsModel::hasFullList() const
{
    return mHasFullList;
}

void MentionsModel::checkFullList()
{
    setHasFullList(mMentions->count() == mMentions->total());
}

void MentionsModel::addMoreMentions(const QJsonObject &mentionsObj)
{
    const int numberOfElement = mMentions->mentions().count();
    mMentions->parseMoreMentions(mentionsObj);
    beginInsertRows(QModelIndex(), numberOfElement, mMentions->mentions().count() - 1);
    endInsertRows();
    checkFullList();
}

QString MentionsModel::convertMessageText(const QString &str, const QString &userName) const
{
    return mTextConverter->convertMessageText(str, userName, {} /*mMentions->mentions() TODO*/);
}

bool MentionsModel::loadMoreMentionsInProgress() const
{
    return mLoadMoreMentionsInProgress;
}

void MentionsModel::setLoadMoreMentionsInProgress(bool loadMoreMentionsInProgress)
{
    mLoadMoreMentionsInProgress = loadMoreMentionsInProgress;
}
