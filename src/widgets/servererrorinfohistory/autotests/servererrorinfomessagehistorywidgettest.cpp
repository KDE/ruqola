/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "servererrorinfomessagehistorywidgettest.h"
#include "servererrorinfohistory/servererrorinfomessagehistorywidget.h"
#include <QTest>

QTEST_MAIN(ServerErrorInfoMessageHistoryWidgetTest)
ServerErrorInfoMessageHistoryWidgetTest::ServerErrorInfoMessageHistoryWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ServerErrorInfoMessageHistoryWidgetTest::shouldHaveDefaultValues()
{
    ServerErrorInfoMessageHistoryWidget w;
    // TODO
}
