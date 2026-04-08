/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ruqolaswitchofflinemodewidgettest.h"
#include "loginwidget/ruqolaswitchofflinemodewidget.h"
#include <QTest>
QTEST_MAIN(RuqolaSwitchOfflineModeWidgetTest)

RuqolaSwitchOfflineModeWidgetTest::RuqolaSwitchOfflineModeWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void RuqolaSwitchOfflineModeWidgetTest::shouldHaveDefaultValues()
{
    const RuqolaSwitchOfflineModeWidget w;
    // TODO
}

#include "moc_ruqolaswitchofflinemodewidgettest.cpp"
