/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "usersinroomwidgettest.h"
#include "room/usersinroomwidget.h"
#include "room/usersinroomcombobox.h"
#include <QTest>
#include <QVBoxLayout>
#include <QListView>
#include <QLineEdit>
#include <QLabel>

QTEST_MAIN(UsersInRoomWidgetTest)

UsersInRoomWidgetTest::UsersInRoomWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void UsersInRoomWidgetTest::shouldHaveDefaultValues()
{
    UsersInRoomWidget w;
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), {});

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
    QCOMPARE(hMainLayout->contentsMargins(), {});
}
