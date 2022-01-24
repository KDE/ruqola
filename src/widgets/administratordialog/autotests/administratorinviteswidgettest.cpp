/*
   SPDX-FileCopyrightText: 2021-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorinviteswidgettest.h"
#include "administratordialog/invites/administratorinviteswidget.h"
#include <QTest>
QTEST_MAIN(AdministratorInvitesWidgetTest)
AdministratorInvitesWidgetTest::AdministratorInvitesWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void AdministratorInvitesWidgetTest::shouldHaveDefaultValues()
{
    AdministratorInvitesWidget w(nullptr);
    // TODO
}
