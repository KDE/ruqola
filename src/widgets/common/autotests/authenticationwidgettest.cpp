/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "authenticationwidgettest.h"

#include "common/authenticationoauthwidget.h"
#include <QTest>
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;

QTEST_MAIN(AuthenticationWidgetTest)
AuthenticationWidgetTest::AuthenticationWidgetTest(QWidget *parent)
    : QWidget{parent}
{
}

void AuthenticationWidgetTest::shouldHaveDefaultValues()
{
    const AuthenticationOauthWidget w;
    auto mMainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mMainLayout);
    QCOMPARE(mMainLayout->contentsMargins(), QMargins{});
}

#include "moc_authenticationwidgettest.cpp"
