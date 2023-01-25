/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "servererrorinfomessagehistorywidgettest.h"
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

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mListNotifications = w.findChild<QListView *>(QStringLiteral("mListServerInfosListView"));
    QVERIFY(mListNotifications);
    QCOMPARE(mListNotifications->horizontalScrollBarPolicy(), Qt::ScrollBarAlwaysOff);
    QCOMPARE(mListNotifications->horizontalScrollBarPolicy(), Qt::ScrollBarAlwaysOff);
    QVERIFY(mListNotifications->wordWrap());
    QCOMPARE(mListNotifications->verticalScrollMode(), QAbstractItemView::ScrollPerPixel);

    auto mListServerErrorInfosDelegate = w.findChild<ServerErrorInfoHistoryDelegate *>(QStringLiteral("mListServerErrorInfosDelegate"));
    QVERIFY(mListServerErrorInfosDelegate);
    QCOMPARE(mListNotifications->itemDelegate(), mListServerErrorInfosDelegate);

    auto searchLayout = w.findChild<QHBoxLayout *>(QStringLiteral("searchLayout"));
    QVERIFY(searchLayout);
    QCOMPARE(searchLayout->contentsMargins(), QMargins{});

    auto mSearchLineEdit = w.findChild<QLineEdit *>(QStringLiteral("mSearchLineEdit"));
    QVERIFY(mSearchLineEdit);
    QVERIFY(mSearchLineEdit->isClearButtonEnabled());
    QVERIFY(!mSearchLineEdit->placeholderText().isEmpty());

    auto mServersComboBox = w.findChild<ServersComboBox *>(QStringLiteral("mServersComboBox"));
    QVERIFY(mServersComboBox);
}
