/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createnewserverwidgettest.h"
#include "common/authenticationloginwidget.h"
#include "common/authenticationoauthwidget.h"
#include "createnewserver/createnewserverwidget.h"
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

    auto mainLayout = w.findChild<QFormLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    QVERIFY(w.accountInfo().userName.isEmpty());
    QVERIFY(w.accountInfo().serverUrl.isEmpty());
    QVERIFY(w.accountInfo().accountName.isEmpty());

    auto mAuthenticationWidget = w.findChild<AuthenticationOauthWidget *>(QStringLiteral("mAuthenticationWidget"));
    QVERIFY(mAuthenticationWidget);

    auto mAuthenticationLoginWidget = w.findChild<AuthenticationLoginWidget *>(QStringLiteral("mAuthenticationLoginWidget"));
    QVERIFY(mAuthenticationLoginWidget);
}

#include "moc_createnewserverwidgettest.cpp"
