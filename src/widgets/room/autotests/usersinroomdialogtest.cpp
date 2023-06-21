/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usersinroomdialogtest.h"
#include "room/usersinroomdialog.h"
#include "room/usersinroomwidget.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(UsersInRoomDialogTest)
UsersInRoomDialogTest::UsersInRoomDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void UsersInRoomDialogTest::shouldHaveDefaultValues()
{
    UsersInRoomDialog w(nullptr);
    QVERIFY(!w.windowTitle().isEmpty());
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mUsersInRoomWidget = w.findChild<UsersInRoomWidget *>(QStringLiteral("mUsersInRoomWidget"));
    QVERIFY(mUsersInRoomWidget);

    auto buttonBox = w.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(buttonBox);
}

#include "moc_usersinroomdialogtest.cpp"
