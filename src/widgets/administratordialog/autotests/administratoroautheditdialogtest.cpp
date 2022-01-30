/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "administratoroautheditdialogtest.h"
#include "administratordialog/oauth/administratoroautheditdialog.h"
#include <QTest>
QTEST_MAIN(AdministratorOauthEditDialogTest)
AdministratorOauthEditDialogTest::AdministratorOauthEditDialogTest(QObject *parent)
    : QObject{parent}
{
}

void AdministratorOauthEditDialogTest::shouldHaveDefaultValues()
{
    AdministratorOauthEditDialog d;
    // TODO
}
