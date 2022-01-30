/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "administratoroautheditwidgettest.h"
#include "administratordialog/oauth/administratoroautheditwidget.h"
#include <QTest>
QTEST_MAIN(AdministratorOauthEditWidgetTest)
AdministratorOauthEditWidgetTest::AdministratorOauthEditWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void AdministratorOauthEditWidgetTest::shouldHaveDefaultValues()
{
    AdministratorOauthEditWidget w;
    // TODO
}
