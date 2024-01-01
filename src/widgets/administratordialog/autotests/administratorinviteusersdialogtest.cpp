/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorinviteusersdialogtest.h"
#include "administratordialog/users/administratorinviteusersdialog.h"
#include "administratordialog/users/administratorinviteuserswidget.h"

#include <QDialogButtonBox>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(AdministratorInviteUsersDialogTest)
AdministratorInviteUsersDialogTest::AdministratorInviteUsersDialogTest(QObject *parent)
    : QObject{parent}
{
}

void AdministratorInviteUsersDialogTest::shouldHaveDefaultValues()
{
    AdministratorInviteUsersDialog w;
    QVERIFY(w.emails().isEmpty());

    QVERIFY(!w.windowTitle().isEmpty());
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mCreateWidget = w.findChild<AdministratorInviteUsersWidget *>(QStringLiteral("mInviteUsersWidget"));
    QVERIFY(mCreateWidget);

    auto buttonBox = w.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(buttonBox);
}

#include "moc_administratorinviteusersdialogtest.cpp"
