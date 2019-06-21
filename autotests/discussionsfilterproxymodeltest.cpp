/*
   Copyright (c) 2019 Montel Laurent <montel@kde.org>

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

#include "discussionsfilterproxymodeltest.h"
#include "model/discussionsfilterproxymodel.h"
#include "model/discussionsmodel.h"
#include <QTest>
QTEST_MAIN(DiscussionsFilterProxyModelTest)
DiscussionsFilterProxyModelTest::DiscussionsFilterProxyModelTest(QObject *parent)
    : QObject(parent)
{
}

void DiscussionsFilterProxyModelTest::shouldHaveDefaultValues()
{
    DiscussionsFilterProxyModel proxy;
    DiscussionsModel model;
    proxy.setSourceModel(&model);
    QCOMPARE(proxy.rowCount(), 0);

    QCOMPARE(proxy.filterCaseSensitivity(), Qt::CaseInsensitive);
    QCOMPARE(proxy.filterRole(), DiscussionsModel::Description);
    QCOMPARE(proxy.sortRole(), DiscussionsModel::SortByTimeStamp);


    QHash<int, QByteArray> roles;
    roles[DiscussionsModel::ParentId] = QByteArrayLiteral("parentid");
    roles[DiscussionsModel::Description] = QByteArrayLiteral("description");
    roles[DiscussionsModel::NumberOfMessages] = QByteArrayLiteral("numberofmessages");
    roles[DiscussionsModel::LastMessage] = QByteArrayLiteral("lastmessage");
    roles[DiscussionsModel::DiscussionRoomId] = QByteArrayLiteral("discussionid");
    roles[DiscussionsModel::TimeStamp] = QByteArrayLiteral("timestamp");
    QCOMPARE(proxy.roleNames(), roles);
}

void DiscussionsFilterProxyModelTest::shouldAssignValue()
{
    DiscussionsFilterProxyModel proxy;
    DiscussionsModel model;
    proxy.setSourceModel(&model);

    Discussions discussionList;
    for (int i = 0; i < 10; ++i) {
        Discussion c;
        c.setDescription(QStringLiteral("roomid%1").arg(i));
        c.setNumberMessages(i);
        c.setParentRoomId(QStringLiteral("online"));
        discussionList.append(c);
    }
    model.setDiscussions(discussionList);

    QCOMPARE(model.rowCount(), 10);
    QCOMPARE(proxy.rowCount(), 10);

    discussionList.clear();
    for (int i = 0; i < 3; ++i) {
        Discussion c;
        c.setDescription(QStringLiteral("roomid%1").arg(i));
        c.setNumberMessages(i);
        c.setParentRoomId(QStringLiteral("online"));
        discussionList.append(c);
    }
    model.setDiscussions(discussionList);
    QCOMPARE(model.rowCount(), 3);
    QCOMPARE(proxy.rowCount(), 3);
}
