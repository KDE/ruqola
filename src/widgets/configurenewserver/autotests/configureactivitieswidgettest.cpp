/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configureactivitieswidgettest.h"
#include "configurenewserver/configureactivitieswidget.h"
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(ConfigureActivitiesWidgetTest)
ConfigureActivitiesWidgetTest::ConfigureActivitiesWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ConfigureActivitiesWidgetTest::shouldHaveDefaultValues()
{
    ConfigureActivitiesWidget d;
    // TODO
}
