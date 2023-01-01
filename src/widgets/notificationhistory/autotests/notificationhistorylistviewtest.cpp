/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "notificationhistorylistviewtest.h"
#include "notificationhistory/notificationhistorylistview.h"
#include <QTest>
QTEST_MAIN(NotificationHistoryListViewTest)

NotificationHistoryListViewTest::NotificationHistoryListViewTest(QObject *parent)
    : QObject{parent}
{
}

void NotificationHistoryListViewTest::shouldHaveDefaultValues()
{
    NotificationHistoryListView w;
    QVERIFY(w.searchText().isEmpty());
}
