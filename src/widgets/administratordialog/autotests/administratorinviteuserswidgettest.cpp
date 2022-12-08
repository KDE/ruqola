/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorinviteuserswidgettest.h"
#include "administratordialog/users/administratorinviteuserswidget.h"
#include <QTest>
QTEST_MAIN(AdministratorInviteUsersWidgetTest)
AdministratorInviteUsersWidgetTest::AdministratorInviteUsersWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void AdministratorInviteUsersWidgetTest::shouldHaveDefaultValues()
{
    AdministratorInviteUsersWidget w;
    // TODO
}
