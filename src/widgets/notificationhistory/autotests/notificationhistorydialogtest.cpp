/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationhistorydialogtest.h"
using namespace Qt::Literals::StringLiterals;

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
    auto mainLayout = d.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    auto mNotificationHistoryWidget = d.findChild<NotificationHistoryWidget *>(u"mNotificationHistoryWidget"_s);
    QVERIFY(mNotificationHistoryWidget);

    auto button = d.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(button);
}

#include "moc_notificationhistorydialogtest.cpp"
