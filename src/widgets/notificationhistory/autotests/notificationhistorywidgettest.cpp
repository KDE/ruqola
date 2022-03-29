/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationhistorywidgettest.h"
#include "notificationhistory/notificationhistorywidget.h"
#include <QTest>
QTEST_MAIN(NotificationHistoryWidgetTest)

NotificationHistoryWidgetTest::NotificationHistoryWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void NotificationHistoryWidgetTest::shouldHaveDefaultValues()
{
    NotificationHistoryWidget w;
    // TODO
}
