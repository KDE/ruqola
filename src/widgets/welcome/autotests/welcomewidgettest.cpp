/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "welcomewidgettest.h"
#include <QTest>
QTEST_MAIN(WelcomeWidgetTest)
WelcomeWidgetTest::WelcomeWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void WelcomeWidgetTest::shouldHaveDefaultValues()
{
    // TODO
}

#include "moc_welcomewidgettest.cpp"
