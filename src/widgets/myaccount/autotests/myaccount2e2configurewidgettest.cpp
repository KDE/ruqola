/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccount2e2configurewidgettest.h"
#include "myaccount/myaccount2e2configurewidget.h"
#include <QTest>
QTEST_MAIN(MyAccount2e2ConfigureWidgetTest)

MyAccount2e2ConfigureWidgetTest::MyAccount2e2ConfigureWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void MyAccount2e2ConfigureWidgetTest::shouldHaveDefaultValues()
{
    MyAccount2e2ConfigureWidget w(nullptr);
    // TODO
}
