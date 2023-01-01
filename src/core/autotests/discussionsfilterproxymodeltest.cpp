/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "discussionsfilterproxymodeltest.h"
#include "discussions/discussions.h"
#include "model/discussionsfilterproxymodel.h"
#include "model/discussionsmodel.h"
#include <QTest>
QTEST_GUILESS_MAIN(DiscussionsFilterProxyModelTest)
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
        discussionList.append(std::move(c));
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
        discussionList.append(std::move(c));
    }
    model.setDiscussions(discussionList);
    QCOMPARE(model.rowCount(), 3);
    QCOMPARE(proxy.rowCount(), 3);
}
