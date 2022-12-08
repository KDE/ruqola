/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorinviteusersdialogtest.h"
#include "administratordialog/users/administratorinviteusersdialog.h"

#include <QTest>
QTEST_MAIN(AdministratorInviteUsersDialogTest)
AdministratorInviteUsersDialogTest::AdministratorInviteUsersDialogTest(QObject *parent)
    : QObject{parent}
{
}

void AdministratorInviteUsersDialogTest::shouldHaveDefaultValues()
{
    AdministratorInviteUsersDialog w;
    // TODO
}
