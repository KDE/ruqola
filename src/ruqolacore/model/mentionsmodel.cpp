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

MentionsModel::MentionsModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

MentionsModel::~MentionsModel()
{
}

int MentionsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mMentions.count();
}

QVariant MentionsModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mMentions.count()) {
        return {};
    }
    const Mention mention = mMentions.at(index.row());

    //TODO
    return {};
}

void MentionsModel::setMentions(const Mentions &mentions)
{
    if (rowCount() != 0) {
        beginRemoveRows(QModelIndex(), 0, mMentions.count() - 1);
        mMentions.clear();
        endRemoveRows();
    }
    if (!mMentions.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, mMentions.count() - 1);
        mMentions = mentions;
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
    roles[MessageType] = QByteArrayLiteral("messagetype");
    roles[Attachments] = QByteArrayLiteral("attachments");
    roles[Urls] = QByteArrayLiteral("urls");
    roles[Date] = QByteArrayLiteral("date");
    roles[UsernameUrl] = QByteArrayLiteral("usernameurl");
    roles[Roles] = QByteArrayLiteral("roles");
    roles[Reactions] = QByteArrayLiteral("reactions");
    return roles;
}
