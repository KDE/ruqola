/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagemaximumsizewidgettest.h"
#include "messagemaximumsizedialog/messagemaximumsizewidget.h"
#include <QTest>
QTEST_MAIN(MessageMaximumSizeWidgetTest)
MessageMaximumSizeWidgetTest::MessageMaximumSizeWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void MessageMaximumSizeWidgetTest::shouldHaveDefaultValues()
{
    MessageMaximumSizeWidget d;
    // TODO
}
