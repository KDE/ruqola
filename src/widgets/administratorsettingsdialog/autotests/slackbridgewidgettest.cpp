/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "slackbridgewidgettest.h"
#include "administratorsettingsdialog/slackbridge/slackbridgewidget.h"
#include <QTest>
QTEST_MAIN(SlackBridgeWidgetTest)
SlackBridgeWidgetTest::SlackBridgeWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void SlackBridgeWidgetTest::shouldHaveDefaultValues()
{
    SlackBridgeWidget w(nullptr);
    // TODO
}
