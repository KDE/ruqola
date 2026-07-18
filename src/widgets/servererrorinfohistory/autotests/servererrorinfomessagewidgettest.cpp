/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "servererrorinfomessagewidgettest.h"

#include "servererrorinfohistory/servererrorinfomessagewidget.h"
#include <QTest>
QTEST_MAIN(ServerErrorInfoMessageWidgetTest)
using namespace Qt::Literals::StringLiterals;

ServerErrorInfoMessageWidgetTest::ServerErrorInfoMessageWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ServerErrorInfoMessageWidgetTest::shouldHaveDefaultValues()
{
    ServerErrorInfoMessageWidget w;
    w.show();
    QVERIFY(w.isCloseButtonVisible());
    QCOMPARE(w.messageType(), KMessageWidget::Error);
}

#include "moc_servererrorinfomessagewidgettest.cpp"
