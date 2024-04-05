/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configureactivitieswidgettest.h"
#include "configurenewserver/configureactivitieswidget.h"
#include <QListView>
#include <QTest>
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;
QTEST_MAIN(ConfigureActivitiesWidgetTest)
ConfigureActivitiesWidgetTest::ConfigureActivitiesWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ConfigureActivitiesWidgetTest::shouldHaveDefaultValues()
{
    ConfigureActivitiesWidget w;

    auto mListView = w.findChild<QListView *>("mListView"_L1);
    QVERIFY(mListView);

    auto mainLayout = w.findChild<QVBoxLayout *>("mainLayout"_L1);
    QVERIFY(mainLayout);
}
