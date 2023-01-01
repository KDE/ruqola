/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createvideomessagewidgettest.h"
#include "dialogs/createvideomessagewidget.h"
#include <QTest>
QTEST_MAIN(CreateVideoMessageWidgetTest)

CreateVideoMessageWidgetTest::CreateVideoMessageWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void CreateVideoMessageWidgetTest::shouldHaveDefaultValues()
{
    CreateVideoMessageWidget w;
}
