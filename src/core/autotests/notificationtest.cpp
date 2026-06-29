/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationtest.h"

#include "notifications/notification.h"
#include <QTest>
QTEST_MAIN(NotificationTest)

using namespace Qt::Literals::StringLiterals;
NotificationTest::NotificationTest(QObject *parent)
    : QObject(parent)
{
}

void NotificationTest::shouldHaveDefaultValue()
{
    Notification w;
    QCOMPARE(w.status(), KStatusNotifierItem::Passive);
}

void NotificationTest::shouldChangeStatus()
{
    Notification w;
    // Notification
    w.updateToolTip(u"foo"_s, true);
    QCOMPARE(w.status(), KStatusNotifierItem::Active);
    // Clear it.
    w.updateToolTip(QString(), false);
    QCOMPARE(w.status(), KStatusNotifierItem::Passive);

    w.updateToolTip(u"Bla"_s, false);
    QCOMPARE(w.status(), KStatusNotifierItem::Active);
    w.updateToolTip(u"Bla"_s, true);
    QCOMPARE(w.status(), KStatusNotifierItem::Active);
    w.updateToolTip(u"Bla"_s, true);
    QCOMPARE(w.status(), KStatusNotifierItem::Active);
    w.updateToolTip(u"Bla"_s, false);
    QCOMPARE(w.status(), KStatusNotifierItem::Active);
}

#include "moc_notificationtest.cpp"
