/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "statusmodelfilterproxymodeltest.h"
#include "model/statusmodelfilterproxymodel.h"
#include <QTest>
QTEST_MAIN(StatusModelFilterProxyModelTest)
StatusModelFilterProxyModelTest::StatusModelFilterProxyModelTest(QObject *parent)
    : QObject(parent)
{
}

void StatusModelFilterProxyModelTest::shouldHaveDefaultValues()
{
    StatusModelFilterProxyModel w;
    QVERIFY(!w.useOnlyStandardStatus());
    QVERIFY(w.allowOfflineSupport());
}

#include "moc_statusmodelfilterproxymodeltest.cpp"
