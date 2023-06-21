/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usersinroomwidgettest.h"
#include "room/usersinroomcombobox.h"
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
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mListView = w.findChild<QListView *>(QStringLiteral("mListView"));
    QVERIFY(mListView);

    auto mSearchLineEdit = w.findChild<QLineEdit *>(QStringLiteral("mSearchLineEdit"));
    QVERIFY(mSearchLineEdit);
    QVERIFY(!mSearchLineEdit->placeholderText().isEmpty());
    QVERIFY(mSearchLineEdit->isClearButtonEnabled());

    auto mMessageListInfo = w.findChild<QLabel *>(QStringLiteral("mMessageListInfo"));
    QVERIFY(mMessageListInfo);
    QCOMPARE(mMessageListInfo->textFormat(), Qt::RichText);
    QCOMPARE(mMessageListInfo->contextMenuPolicy(), Qt::NoContextMenu);

    auto mUsersInRoomComboBox = w.findChild<UsersInRoomComboBox *>(QStringLiteral("mUsersInRoomComboBox"));
    QVERIFY(mUsersInRoomComboBox);

    auto hMainLayout = w.findChild<QHBoxLayout *>(QStringLiteral("hMainLayout"));
    QVERIFY(hMainLayout);
    QCOMPARE(hMainLayout->contentsMargins(), QMargins{});
}

#include "moc_usersinroomwidgettest.cpp"
