/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "authenticationwidgettest.h"
#include "common/authenticationoauthwidget.h"
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(AuthenticationWidgetTest)
AuthenticationWidgetTest::AuthenticationWidgetTest(QWidget *parent)
    : QWidget{parent}
{
}

void AuthenticationWidgetTest::shouldHaveDefaultValues()
{
    AuthenticationOauthWidget w;
    auto mMainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mMainLayout);
    QCOMPARE(mMainLayout->contentsMargins(), QMargins{});
}

#include "moc_authenticationwidgettest.cpp"
