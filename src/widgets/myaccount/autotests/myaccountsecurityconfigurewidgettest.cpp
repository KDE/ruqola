/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccountsecurityconfigurewidgettest.h"
#include "myaccount/myaccountsecurityconfigurewidget.h"
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(MyAccountSecurityConfigureWidgetTest)
MyAccountSecurityConfigureWidgetTest::MyAccountSecurityConfigureWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void MyAccountSecurityConfigureWidgetTest::shouldHaveDefaultValues()
{
    MyAccountSecurityConfigureWidget w;

    auto topLayout = w.findChild<QVBoxLayout *>(QStringLiteral("topLayout"));
    QVERIFY(topLayout);
    QCOMPARE(topLayout->contentsMargins(), {});
}
