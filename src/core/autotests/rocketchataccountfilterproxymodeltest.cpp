/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "rocketchataccountfilterproxymodeltest.h"
#include "config-ruqola.h"
#include "model/rocketchataccountfilterproxymodel.h"
#include "model/rocketchataccountmodel.h"
#include <QTest>
#if HAVE_ACTIVITY_SUPPORT
#include "activities/activitiesmanager.h"
#endif
QTEST_GUILESS_MAIN(RocketChatAccountFilterProxyModelTest)

RocketChatAccountFilterProxyModelTest::RocketChatAccountFilterProxyModelTest(QObject *parent)
    : QObject(parent)
{
}

void RocketChatAccountFilterProxyModelTest::shouldHaveDefaultValue()
{
    RocketChatAccountFilterProxyModel w;
    RocketChatAccountModel sourceModel;
    w.setSourceModel(&sourceModel);
    QCOMPARE(w.rowCount(), 0);
    QVERIFY(w.sourceModel());
    QCOMPARE(w.sourceModel(), &sourceModel);
    QVERIFY(w.accountOrder().isEmpty());
#if HAVE_ACTIVITY_SUPPORT
    QVERIFY(!w.activitiesManager());
#endif
    QVERIFY(w.filterActivities());
}

#include "moc_rocketchataccountfilterproxymodeltest.cpp"
