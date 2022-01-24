/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratoroauthwidgettest.h"
#include "administratordialog/oauth/administratoroauthwidget.h"
#include <QTest>
QTEST_MAIN(AdministratorOauthWidgetTest)
AdministratorOauthWidgetTest::AdministratorOauthWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void AdministratorOauthWidgetTest::shouldHaveDefaultValues()
{
    AdministratorOauthWidget d(nullptr);
    // TODO
}
