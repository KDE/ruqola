/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationhistorydialogtest.h"
#include "notificationhistory/notificationhistorydialog.h"
#include "notificationhistory/notificationhistorywidget.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(NotificationHistoryDialogTest)
NotificationHistoryDialogTest::NotificationHistoryDialogTest(QObject *parent)
    : QObject{parent}
{
    QStandardPaths::setTestModeEnabled(true);
}

void NotificationHistoryDialogTest::shouldHaveDefaultValues()
{
    NotificationHistoryDialog d;
    QVERIFY(!d.windowTitle().isEmpty());
    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    auto mNotificationHistoryWidget = d.findChild<NotificationHistoryWidget *>(QStringLiteral("mNotificationHistoryWidget"));
    QVERIFY(mNotificationHistoryWidget);

    auto button = d.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);
}

#include "moc_notificationhistorydialogtest.cpp"
