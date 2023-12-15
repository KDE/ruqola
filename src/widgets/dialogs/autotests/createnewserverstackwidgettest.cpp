/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "createnewserverstackwidgettest.h"
#include "dialogs/createnewserverstackwidget.h"
#include <QTest>
QTEST_MAIN(CreateNewServerStackWidgetTest)
CreateNewServerStackWidgetTest::CreateNewServerStackWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void CreateNewServerStackWidgetTest::shouldHaveDefaultValues()
{
    CreateNewServerStackWidget w;
    // TODO
}
