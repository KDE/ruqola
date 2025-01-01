/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "whatsnewmessagewidgettest.h"
#include "whatsnew/whatsnewmessagewidget.h"
#include <QTest>
QTEST_MAIN(WhatsNewMessageWidgetTest)
WhatsNewMessageWidgetTest::WhatsNewMessageWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void WhatsNewMessageWidgetTest::shouldHaveDefaultValues()
{
    WhatsNewMessageWidget w;
    w.show();
    QVERIFY(w.isCloseButtonVisible());
    QCOMPARE(w.messageType(), KMessageWidget::Information);
}

#include "moc_whatsnewmessagewidgettest.cpp"
