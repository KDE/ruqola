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

#include "searchmessagemodeltest.h"
#include "model/searchmessagemodel.h"
#include "test_model_helpers.h"
#include <QTest>

QTEST_MAIN(SearchMessageModelTest)

SearchMessageModelTest::SearchMessageModelTest(QObject *parent)
    : QObject(parent)
{
}

void SearchMessageModelTest::shouldHaveDefaultValue()
{
    SearchMessageModel w(nullptr);
    QCOMPARE(w.rowCount(), 0);

    QHash<int, QByteArray> roles;
    roles[SearchMessageModel::OriginalMessage] = QByteArrayLiteral("originalMessage");
    roles[SearchMessageModel::MessageConvertedText] = QByteArrayLiteral("messageConverted");
    roles[SearchMessageModel::Username] = QByteArrayLiteral("username");
    roles[SearchMessageModel::Timestamp] = QByteArrayLiteral("timestamp");
    roles[SearchMessageModel::UserId] = QByteArrayLiteral("userID");
    roles[SearchMessageModel::SystemMessageType] = QByteArrayLiteral("type");
    roles[SearchMessageModel::MessageId] = QByteArrayLiteral("messageID");
    roles[SearchMessageModel::RoomId] = QByteArrayLiteral("roomID");
    roles[SearchMessageModel::UpdatedAt] = QByteArrayLiteral("updatedAt");
    roles[SearchMessageModel::EditedAt] = QByteArrayLiteral("editedAt");
    roles[SearchMessageModel::EditedByUserName] = QByteArrayLiteral("editedByUsername");
    roles[SearchMessageModel::EditedByUserId] = QByteArrayLiteral("editedByUserID");
    roles[SearchMessageModel::Alias] = QByteArrayLiteral("alias");
    roles[SearchMessageModel::Avatar] = QByteArrayLiteral("avatar");
    roles[SearchMessageModel::Groupable] = QByteArrayLiteral("groupable");
    roles[SearchMessageModel::MessageType] = QByteArrayLiteral("messagetype");
    roles[SearchMessageModel::Attachments] = QByteArrayLiteral("attachments");
    roles[SearchMessageModel::Urls] = QByteArrayLiteral("urls");
    roles[SearchMessageModel::Date] = QByteArrayLiteral("date");
    roles[SearchMessageModel::CanEditMessage] = QByteArrayLiteral("canEditMessage");
    roles[SearchMessageModel::Starred] = QByteArrayLiteral("starred");
    roles[SearchMessageModel::UsernameUrl] = QByteArrayLiteral("usernameurl");
    roles[SearchMessageModel::Roles] = QByteArrayLiteral("roles");
    roles[SearchMessageModel::Reactions] = QByteArrayLiteral("reactions");
    roles[SearchMessageModel::Ignored] = QByteArrayLiteral("userIsIgnored");
    roles[SearchMessageModel::Pinned] = QByteArrayLiteral("pinned");
    roles[SearchMessageModel::DiscussionCount] = QByteArrayLiteral("discussionCount");
    roles[SearchMessageModel::DiscussionRoomId] = QByteArrayLiteral("discussionRoomId");
    roles[SearchMessageModel::DiscussionLastMessage] = QByteArrayLiteral("discussionLastMessage");
    roles[SearchMessageModel::ThreadCount] = QByteArrayLiteral("threadCount");
    roles[SearchMessageModel::ThreadLastMessage] = QByteArrayLiteral("threadLastMessage");
    roles[SearchMessageModel::ThreadMessageId] = QByteArrayLiteral("threadMessageId");
    roles[SearchMessageModel::ThreadMessagePreview] = QByteArrayLiteral("threadMessagePreview");
    roles[SearchMessageModel::ShowTranslatedMessage] = QByteArrayLiteral("showTranslatedMessage");
    QCOMPARE(w.roleNames(), roles);
}
