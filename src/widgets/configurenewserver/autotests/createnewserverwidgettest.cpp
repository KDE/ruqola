/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createnewserverwidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "common/authenticationloginwidget.h"
#include "common/authenticationoauthwidget.h"
#include "configurenewserver/createnewserverwidget.h"
#include <QFormLayout>
#include <QTest>
QTEST_MAIN(CreateNewServerWidgetTest)
CreateNewServerWidgetTest::CreateNewServerWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void CreateNewServerWidgetTest::shouldHaveDefaultValues()
{
    CreateNewServerWidget w;

    auto mainLayout = w.findChild<QFormLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    QVERIFY(w.accountInfo().userName.isEmpty());
    QVERIFY(w.accountInfo().serverUrl.isEmpty());
    QVERIFY(w.accountInfo().accountName.isEmpty());

    auto mAuthenticationWidget = w.findChild<AuthenticationOauthWidget *>(u"mAuthenticationWidget"_s);
    QVERIFY(mAuthenticationWidget);

    auto mAuthenticationLoginWidget = w.findChild<AuthenticationLoginWidget *>(u"mAuthenticationLoginWidget"_s);
    QVERIFY(mAuthenticationLoginWidget);
}

#include "moc_createnewserverwidgettest.cpp"
