/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "verifynewversionwidgetactiontest.h"
#include "verifynewversionwidget/verifynewversionwidgetaction.h"
#include <QTest>
QTEST_MAIN(VerifyNewVersionWidgetActionTest)
VerifyNewVersionWidgetActionTest::VerifyNewVersionWidgetActionTest(QObject *parent)
    : QObject{parent}
{
}

void VerifyNewVersionWidgetActionTest::shouldHaveDefaultValues()
{
    VerifyNewVersionWidgetAction w;
    // TODO
}

#include "moc_verifynewversionwidgetactiontest.cpp"
