/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "authenticationwidgettest.h"
#include "common/authenticationwidget.h"
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(AuthenticationWidgetTest)
AuthenticationWidgetTest::AuthenticationWidgetTest(QWidget *parent)
    : QWidget{parent}
{
}

void AuthenticationWidgetTest::shouldHaveDefaultValues()
{
    AuthenticationWidget w;
    auto mMainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mMainLayout);
    QCOMPARE(mMainLayout->contentsMargins(), QMargins{});
}
