/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "resetpasswordwidgettest.h"
#include "dialogs/reportmessagewidget.h"
#include <QTest>
QTEST_MAIN(ResetPasswordWidgetTest)
ResetPasswordWidgetTest::ResetPasswordWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ResetPasswordWidgetTest::shouldHaveDefaultValues()
{
    ReportMessageWidget w;
    // TODO
}
