/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "confirmpasswordwidgettest.h"
#include "misc/confirmpasswordwidget.h"
#include <QTest>
QTEST_MAIN(ConfirmPasswordWidgetTest)

ConfirmPasswordWidgetTest::ConfirmPasswordWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ConfirmPasswordWidgetTest::shouldHaveDefaultValues()
{
    ConfirmPasswordWidget w;
    // TODO
}
