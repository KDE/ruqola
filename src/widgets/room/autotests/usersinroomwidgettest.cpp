/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usersinroomwidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "room/usersinroomcombobox.h"
#include "room/usersinroomlistview.h"
#include "room/usersinroomwidget.h"
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(UsersInRoomWidgetTest)

UsersInRoomWidgetTest::UsersInRoomWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void UsersInRoomWidgetTest::shouldHaveDefaultValues()
{
    UsersInRoomWidget w(nullptr);
    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mListView = w.findChild<UsersInRoomListView *>(u"mListView"_s);
    QVERIFY(mListView);

    auto mSearchLineEdit = w.findChild<QLineEdit *>(u"mSearchLineEdit"_s);
    QVERIFY(mSearchLineEdit);
    QVERIFY(!mSearchLineEdit->placeholderText().isEmpty());
    QVERIFY(mSearchLineEdit->isClearButtonEnabled());

    auto mMessageListInfo = w.findChild<QLabel *>(u"mMessageListInfo"_s);
    QVERIFY(mMessageListInfo);
    QCOMPARE(mMessageListInfo->textFormat(), Qt::RichText);
    QCOMPARE(mMessageListInfo->contextMenuPolicy(), Qt::NoContextMenu);

    auto mUsersInRoomComboBox = w.findChild<UsersInRoomComboBox *>(u"mUsersInRoomComboBox"_s);
    QVERIFY(mUsersInRoomComboBox);

    auto hMainLayout = w.findChild<QHBoxLayout *>(u"hMainLayout"_s);
    QVERIFY(hMainLayout);
    QCOMPARE(hMainLayout->contentsMargins(), QMargins{});
}

#include "moc_usersinroomwidgettest.cpp"
