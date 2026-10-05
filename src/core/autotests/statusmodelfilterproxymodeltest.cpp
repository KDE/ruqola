/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "statusmodelfilterproxymodeltest.h"

#include "model/statusmodelfilterproxymodel.h"
#include <QTest>

using namespace Qt::Literals::StringLiterals;

QTEST_GUILESS_MAIN(StatusModelFilterProxyModelTest)
StatusModelFilterProxyModelTest::StatusModelFilterProxyModelTest(QObject *parent)
    : QObject(parent)
{
}

void StatusModelFilterProxyModelTest::shouldHaveDefaultValues()
{
    const StatusModelFilterProxyModel w;
    QVERIFY(!w.useOnlyStandardStatus());
    QVERIFY(w.allowOfflineSupport());
}

#include "moc_statusmodelfilterproxymodeltest.cpp"
