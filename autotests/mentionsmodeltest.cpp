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

#include "mentionsmodeltest.h"
#include "model/mentionsmodel.h"
#include <QSignalSpy>
#include <QTest>
QTEST_MAIN(MentionsModelTest)

MentionsModelTest::MentionsModelTest(QObject *parent)
    : QObject(parent)
{
}

void MentionsModelTest::shouldHaveDefaultValue()
{
    MentionsModel w(nullptr);
    QSignalSpy rowInsertedSpy(&w, &MentionsModel::rowsInserted);
    // (if it had 0 columns, it would have to emit column insertions, too much trouble)
    QCOMPARE(w.rowCount(), 0);
    QCOMPARE(rowInsertedSpy.count(), 0);
    QVERIFY(!w.loadMoreMentionsInProgress());
    QHash<int, QByteArray> roles;
    roles[MentionsModel::OriginalMessage] = QByteArrayLiteral("originalMessage");
    roles[MentionsModel::MessageConvertedText] = QByteArrayLiteral("messageConverted");
    roles[MentionsModel::Username] = QByteArrayLiteral("username");
    roles[MentionsModel::Timestamp] = QByteArrayLiteral("timestamp");
    roles[MentionsModel::UserId] = QByteArrayLiteral("userID");
    roles[MentionsModel::SystemMessageType] = QByteArrayLiteral("type");
    roles[MentionsModel::MessageId] = QByteArrayLiteral("messageID");
    roles[MentionsModel::RoomId] = QByteArrayLiteral("roomID");
    roles[MentionsModel::UpdatedAt] = QByteArrayLiteral("updatedAt");
    roles[MentionsModel::EditedAt] = QByteArrayLiteral("editedAt");
    roles[MentionsModel::EditedByUserName] = QByteArrayLiteral("editedByUsername");
    roles[MentionsModel::EditedByUserId] = QByteArrayLiteral("editedByUserID");
    roles[MentionsModel::Alias] = QByteArrayLiteral("alias");
    roles[MentionsModel::Avatar] = QByteArrayLiteral("avatar");
    roles[MentionsModel::Groupable] = QByteArrayLiteral("groupable");
    roles[MentionsModel::MessageType] = QByteArrayLiteral("messagetype");
    roles[MentionsModel::Attachments] = QByteArrayLiteral("attachments");
    roles[MentionsModel::Urls] = QByteArrayLiteral("urls");
    roles[MentionsModel::Date] = QByteArrayLiteral("date");
    roles[MentionsModel::CanEditMessage] = QByteArrayLiteral("canEditMessage");
    roles[MentionsModel::Starred] = QByteArrayLiteral("starred");
    roles[MentionsModel::UsernameUrl] = QByteArrayLiteral("usernameurl");
    roles[MentionsModel::Roles] = QByteArrayLiteral("roles");
    roles[MentionsModel::Reactions] = QByteArrayLiteral("reactions");
    roles[MentionsModel::Ignored] = QByteArrayLiteral("userIsIgnored");
    roles[MentionsModel::Pinned] = QByteArrayLiteral("pinned");
    roles[MentionsModel::DiscussionCount] = QByteArrayLiteral("discussionCount");
    roles[MentionsModel::DiscussionRoomId] = QByteArrayLiteral("discussionRoomId");
    roles[MentionsModel::DiscussionLastMessage] = QByteArrayLiteral("discussionLastMessage");
    roles[MentionsModel::ThreadCount] = QByteArrayLiteral("threadCount");
    roles[MentionsModel::ThreadLastMessage] = QByteArrayLiteral("threadLastMessage");
    roles[MentionsModel::ThreadMessageId] = QByteArrayLiteral("threadMessageId");
    roles[MentionsModel::ThreadMessagePreview] = QByteArrayLiteral("threadMessagePreview");
    QCOMPARE(w.roleNames(), roles);
}
