/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccountpersonalaccesstokenconfigurewidgettest.h"
#include "myaccount/myaccountpersonalaccesstokenconfigurewidget.h"
#include <QTest>
QTEST_MAIN(MyAccountPersonalAccessTokenConfigureWidgetTest)
MyAccountPersonalAccessTokenConfigureWidgetTest::MyAccountPersonalAccessTokenConfigureWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void MyAccountPersonalAccessTokenConfigureWidgetTest::shouldHaveDefaultValues()
{
    MyAccountPersonalAccessTokenConfigureWidget w(nullptr);
    // TODO
}
