/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "removeaccountwidgettest.h"
#include "configuredialog/removeaccountwidget.h"
#include <QTest>
QTEST_MAIN(RemoveAccountWidgetTest)
RemoveAccountWidgetTest::RemoveAccountWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void RemoveAccountWidgetTest::shouldHaveDefaultValues()
{
    RemoveAccountWidget w;
    // TODO
}
