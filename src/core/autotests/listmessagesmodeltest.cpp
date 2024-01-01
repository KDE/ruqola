/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "listmessagesmodeltest.h"
#include "model/listmessagesmodel.h"

#include <QTest>
QTEST_GUILESS_MAIN(ListMessagesModelTest)

ListMessagesModelTest::ListMessagesModelTest(QObject *parent)
    : QObject(parent)
{
}

void ListMessagesModelTest::shouldHaveDefaultValues()
{
    ListMessagesModel w;
    QVERIFY(!w.roomId().isEmpty()); // By default we define a name! Need to fix it
    QVERIFY(!w.loadMoreListMessagesInProgress());
    QCOMPARE(w.listMessageType(), ListMessagesModel::ListMessageType::Unknown);
    QCOMPARE(w.total(), -1);
}

#include "moc_listmessagesmodeltest.cpp"
