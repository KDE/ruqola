/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationhistorydialogtest.h"
#include "notificationhistory/notificationhistorydialog.h"
#include <QTest>

QTEST_MAIN(NotificationHistoryDialogTest)
NotificationHistoryDialogTest::NotificationHistoryDialogTest(QObject *parent)
    : QObject{parent}
{
}

void NotificationHistoryDialogTest::shouldHaveDefaultValues()
{
    NotificationHistoryDialog w;
    // TODO
}
