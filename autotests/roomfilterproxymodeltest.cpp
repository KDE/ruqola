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

#include "roomfilterproxymodeltest.h"
#include "model/roomfilterproxymodel.h"
#include "model/roommodel.h"
#include <QTest>

QTEST_MAIN(RoomFilterProxyModelTest)

RoomFilterProxyModelTest::RoomFilterProxyModelTest(QObject *parent)
    : QObject(parent)
{
}

void RoomFilterProxyModelTest::shouldHaveDefaultValue()
{
    RoomFilterProxyModel w;
    RoomModel sourceModel;
    w.setSourceModel(&sourceModel);
    QCOMPARE(w.rowCount(), 0);
    QVERIFY(w.sourceModel());
    QCOMPARE(w.sourceModel(), &sourceModel);

    QHash<int, QByteArray> roles;
    roles[RoomModel::RoomName] = QByteArrayLiteral("name");
    roles[RoomModel::RoomID] = QByteArrayLiteral("room_id");
    roles[RoomModel::RoomSelected] = QByteArrayLiteral("selected");
    roles[RoomModel::RoomUnread] = QByteArrayLiteral("unread");
    roles[RoomModel::RoomType] = QByteArrayLiteral("type");
    roles[RoomModel::RoomOwnerUserName] = QByteArrayLiteral("username");
    roles[RoomModel::RoomOwnerUserID] = QByteArrayLiteral("userID");
    roles[RoomModel::RoomTopic] = QByteArrayLiteral("topic");
    roles[RoomModel::RoomMutedUsers] = QByteArrayLiteral("mutedUsers");
    roles[RoomModel::RoomJitsiTimeout] = QByteArrayLiteral("jitsiTimeout");
    roles[RoomModel::RoomRo] = QByteArrayLiteral("readOnly");
    roles[RoomModel::RoomAnnouncement] = QByteArrayLiteral("announcement");
    roles[RoomModel::RoomOpen] = QByteArrayLiteral("open");
    roles[RoomModel::RoomAlert] = QByteArrayLiteral("alert");
    roles[RoomModel::RoomOrder] = QByteArrayLiteral("roomorder");
    roles[RoomModel::RoomFavorite] = QByteArrayLiteral("favorite");
    roles[RoomModel::RoomSection] = QByteArrayLiteral("sectionname");
    roles[RoomModel::RoomIcon] = QByteArrayLiteral("channelicon");
    roles[RoomModel::RoomUserMentions] = QByteArrayLiteral("userMentions");
    QCOMPARE(w.roleNames(), roles);
}
