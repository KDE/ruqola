/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratormoderationrangewidgettest.h"
#include "administratordialog/moderationconsole/administratormoderationrangewidget.h"
#include <QTest>
QTEST_MAIN(AdministratorModerationRangeWidgetTest)
AdministratorModerationRangeWidgetTest::AdministratorModerationRangeWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void AdministratorModerationRangeWidgetTest::shouldHaveDefaultValues()
{
    AdministratorModerationRangeWidget w;
    // TODO
}

#include "moc_administratormoderationrangewidgettest.cpp"
