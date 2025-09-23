/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

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
using namespace Qt::Literals::StringLiterals;
UsersInRoomDialogTest::UsersInRoomDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void UsersInRoomDialogTest::shouldHaveDefaultValues()
{
    UsersInRoomDialog w(nullptr);
    QVERIFY(!w.windowTitle().isEmpty());
    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mUsersInRoomWidget = w.findChild<UsersInRoomWidget *>(u"mUsersInRoomWidget"_s);
    QVERIFY(mUsersInRoomWidget);

    auto buttonBox = w.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(buttonBox);
}

#include "moc_usersinroomdialogtest.cpp"
