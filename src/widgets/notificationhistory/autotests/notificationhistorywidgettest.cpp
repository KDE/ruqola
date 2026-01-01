/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationhistorywidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "misc/serverscombobox.h"
#include "notificationhistory/notificationhistorydelegate.h"
#include "notificationhistory/notificationhistorywidget.h"
#include <QHBoxLayout>
#include <QLineEdit>
#include <QListView>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(NotificationHistoryWidgetTest)

NotificationHistoryWidgetTest::NotificationHistoryWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void NotificationHistoryWidgetTest::shouldHaveDefaultValues()
{
    NotificationHistoryWidget w;

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mListNotifications = w.findChild<QListView *>(u"mListNotifications"_s);
    QVERIFY(mListNotifications);
    QCOMPARE(mListNotifications->horizontalScrollBarPolicy(), Qt::ScrollBarAlwaysOff);
    QCOMPARE(mListNotifications->horizontalScrollBarPolicy(), Qt::ScrollBarAlwaysOff);
    QVERIFY(mListNotifications->wordWrap());
    QCOMPARE(mListNotifications->verticalScrollMode(), QAbstractItemView::ScrollPerPixel);

    auto listNotificationsDelegate = w.findChild<NotificationHistoryDelegate *>(u"listNotificationsDelegate"_s);
    QVERIFY(listNotificationsDelegate);
    QCOMPARE(mListNotifications->itemDelegate(), listNotificationsDelegate);

    auto searchLayout = w.findChild<QHBoxLayout *>(u"searchLayout"_s);
    QVERIFY(searchLayout);
    QCOMPARE(searchLayout->contentsMargins(), QMargins{});

    auto mSearchLineEdit = w.findChild<QLineEdit *>(u"mSearchLineEdit"_s);
    QVERIFY(mSearchLineEdit);
    QVERIFY(mSearchLineEdit->isClearButtonEnabled());
    QVERIFY(!mSearchLineEdit->placeholderText().isEmpty());

    auto mServersComboBox = w.findChild<ServersComboBox *>(u"mServersComboBox"_s);
    QVERIFY(mServersComboBox);
}

#include "moc_notificationhistorywidgettest.cpp"
