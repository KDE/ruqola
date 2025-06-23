/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "statusmodelfilterproxymodeltest.h"
using namespace Qt::Literals::StringLiterals;

#include "model/statusmodelfilterproxymodel.h"
#include <QTest>
QTEST_GUILESS_MAIN(StatusModelFilterProxyModelTest)
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
