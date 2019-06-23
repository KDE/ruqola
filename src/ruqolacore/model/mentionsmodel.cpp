/*
   Copyright (c) 2019 Montel Laurent <montel@kde.org>

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
MentionsModel::MentionsModel(QObject *parent)
    : QAbstractListModel(parent)
{
    mMentions = new Mentions;
}

MentionsModel::~MentionsModel()
{
    delete mMentions;
}

void MentionsModel::initialize()
{
    mRoomId.clear();
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
    const Mention mention = mMentions->at(index.row());
    switch (role) {
    case OriginalMessage:
        return mention.text();
    case MessageConvertedText:
        //return mention.con; //TODO
        return {};
    case Username:
        return mention.username();
    case Timestamp:
        return mention.timeStamp();
    case UserId:
        return mention.userId();
    case MessageId:
        return mention.messageId();
    case RoomId:
        return mention.roomId();
    case UpdatedAt:
        return mention.updatedAt();
    case EditedByUserName:
        return mention.editedByUsername();
    case EditedByUserId:
        return mention.editedByUserId();
    case Alias:
        return mention.alias();
    case Avatar:
        return mention.avatar();
    case Roles:
        return mention.role();
    case Reactions:
    {
        QVariantList lst;
        const auto reactions = mention.reactions().reactions();
        lst.reserve(reactions.count());
        for (const Reaction &react : reactions) {
            //Convert reactions
            lst.append(QVariant::fromValue(react));
        }
        return lst;
    }
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
}

QHash<int, QByteArray> MentionsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[OriginalMessage] = QByteArrayLiteral("originalMessage");
    roles[MessageConvertedText] = QByteArrayLiteral("messageConverted");
    roles[Username] = QByteArrayLiteral("username");
    roles[Timestamp] = QByteArrayLiteral("timestamp");
    roles[UserId] = QByteArrayLiteral("userID");
    roles[MessageId] = QByteArrayLiteral("messageID");
    roles[RoomId] = QByteArrayLiteral("roomID");
    roles[UpdatedAt] = QByteArrayLiteral("updatedAt");
    roles[EditedAt] = QByteArrayLiteral("editedAt");
    roles[EditedByUserName] = QByteArrayLiteral("editedByUsername");
    roles[EditedByUserId] = QByteArrayLiteral("editedByUserID");
    roles[Alias] = QByteArrayLiteral("alias");
    roles[Avatar] = QByteArrayLiteral("avatar");
    roles[Roles] = QByteArrayLiteral("roles");
    roles[Reactions] = QByteArrayLiteral("reactions");
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
}

Mentions *MentionsModel::mentions() const
{
    return mMentions;
}


void MentionsModel::addMoreMentions(const QJsonObject &mentionsObj)
{
    const int numberOfElement = mMentions->mentions().count();
    mMentions->parseMoreMentions(mentionsObj);
    beginInsertRows(QModelIndex(), numberOfElement, mMentions->mentions().count() - 1);
    endInsertRows();
}
