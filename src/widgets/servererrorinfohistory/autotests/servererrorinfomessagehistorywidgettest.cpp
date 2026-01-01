/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "servererrorinfomessagehistorywidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "misc/serverscombobox.h"
#include "servererrorinfohistory/servererrorinfohistorydelegate.h"
#include "servererrorinfohistory/servererrorinfomessagehistorywidget.h"
#include <QLineEdit>
#include <QListView>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(ServerErrorInfoMessageHistoryWidgetTest)
ServerErrorInfoMessageHistoryWidgetTest::ServerErrorInfoMessageHistoryWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ServerErrorInfoMessageHistoryWidgetTest::shouldHaveDefaultValues()
{
    ServerErrorInfoMessageHistoryWidget w;

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mListNotifications = w.findChild<QListView *>(u"mListServerInfosListView"_s);
    QVERIFY(mListNotifications);
    QCOMPARE(mListNotifications->horizontalScrollBarPolicy(), Qt::ScrollBarAlwaysOff);
    QCOMPARE(mListNotifications->horizontalScrollBarPolicy(), Qt::ScrollBarAlwaysOff);
    QVERIFY(mListNotifications->wordWrap());
    QCOMPARE(mListNotifications->verticalScrollMode(), QAbstractItemView::ScrollPerPixel);

    auto mListServerErrorInfosDelegate = w.findChild<ServerErrorInfoHistoryDelegate *>(u"mListServerErrorInfosDelegate"_s);
    QVERIFY(mListServerErrorInfosDelegate);
    QCOMPARE(mListNotifications->itemDelegate(), mListServerErrorInfosDelegate);

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

#include "moc_servererrorinfomessagehistorywidgettest.cpp"
