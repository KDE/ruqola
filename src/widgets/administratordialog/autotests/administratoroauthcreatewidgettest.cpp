/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratoroauthcreatewidgettest.h"
#include "administratordialog/oauth/administratoroauthcreatewidget.h"
#include <QTest>
QTEST_MAIN(AdministratorOauthCreateWidgetTest)
AdministratorOauthCreateWidgetTest::AdministratorOauthCreateWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void AdministratorOauthCreateWidgetTest::shouldHaveDefaultValues()
{
    AdministratorOauthCreateWidget w;
    // TODO
}
