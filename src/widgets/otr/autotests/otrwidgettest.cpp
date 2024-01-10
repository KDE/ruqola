/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "otrwidgettest.h"
#include "otr/otrwidget.h"
#include <QAction>
#include <QTest>
QTEST_MAIN(OtrWidgetTest)
OtrWidgetTest::OtrWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void OtrWidgetTest::shouldHaveDefaultValues()
{
    OtrWidget w;
    w.show();
    // QVERIFY(!w.isVisible());
    QVERIFY(!w.isCloseButtonVisible());
    QCOMPARE(w.messageType(), KMessageWidget::Information);

    auto endOtrAction = w.findChild<QAction *>(QStringLiteral("endOtrAction"));
    QVERIFY(endOtrAction);
    QVERIFY(!endOtrAction->text().isEmpty());

    auto refreshKeysAction = w.findChild<QAction *>(QStringLiteral("refreshKeysAction"));
    QVERIFY(refreshKeysAction);
    QVERIFY(!refreshKeysAction->text().isEmpty());
}

#include "moc_otrwidgettest.cpp"
