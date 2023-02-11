/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exploredatabasewidgettest.h"
#include "databasedialog/exploredatabasewidget.h"
#include <QTest>
QTEST_MAIN(ExploreDatabaseWidgetTest)
ExploreDatabaseWidgetTest::ExploreDatabaseWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ExploreDatabaseWidgetTest::shouldHaveDefaultValues()
{
    ExploreDatabaseWidget w(nullptr);
}
