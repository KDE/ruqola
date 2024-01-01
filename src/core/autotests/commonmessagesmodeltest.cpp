/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "commonmessagesmodeltest.h"
#include "model/commonmessagesmodel.h"
#include <QTest>

QTEST_GUILESS_MAIN(CommonMessagesModelTest)

CommonMessagesModelTest::CommonMessagesModelTest(QObject *parent)
    : QObject(parent)
{
}

void CommonMessagesModelTest::shouldHaveDefaultValue()
{
    CommonMessagesModel w;
    QCOMPARE(w.rowCount(), 0);
    QVERIFY(!w.loadCommonMessagesInProgress());
}

#include "moc_commonmessagesmodeltest.cpp"
