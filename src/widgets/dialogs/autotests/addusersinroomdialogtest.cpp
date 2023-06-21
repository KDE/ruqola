/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "addusersinroomdialogtest.h"
#include "dialogs/addusersinroomdialog.h"
#include "dialogs/addusersinroomwidget.h"
#include <QDialogButtonBox>
#include <QPushButton>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(AddUsersInRoomDialogTest)
AddUsersInRoomDialogTest::AddUsersInRoomDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void AddUsersInRoomDialogTest::shouldHaveDefaultValues()
{
    AddUsersInRoomDialog w(nullptr);
    QVERIFY(!w.windowTitle().isEmpty());
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mAddUsersInRoomWidget = w.findChild<AddUsersInRoomWidget *>(QStringLiteral("mAddUsersInRoomWidget"));
    QVERIFY(mAddUsersInRoomWidget);

    auto buttonBox = w.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(buttonBox);
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    QVERIFY(okButton);
    QVERIFY(!okButton->isEnabled());
    QVERIFY(w.userIds().isEmpty());
    QVERIFY(w.userNames().isEmpty());
}

#include "moc_addusersinroomdialogtest.cpp"
