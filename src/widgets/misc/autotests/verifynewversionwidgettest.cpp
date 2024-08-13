/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "verifynewversionwidgettest.h"
#include "misc/verifynewversionwidget.h"
#include <QTest>
QTEST_MAIN(VerifyNewVersionWidgetTest)
VerifyNewVersionWidgetTest::VerifyNewVersionWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void VerifyNewVersionWidgetTest::shouldHaveDefaultValues()
{
    VerifyNewVersionWidget w;
    // TODO
}
