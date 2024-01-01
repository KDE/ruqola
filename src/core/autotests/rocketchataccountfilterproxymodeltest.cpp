/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "rocketchataccountfilterproxymodeltest.h"
#include "model/rocketchataccountfilterproxymodel.h"
#include "model/rocketchataccountmodel.h"
#include <QTest>

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
}

#include "moc_rocketchataccountfilterproxymodeltest.cpp"
