/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "authenticationloginwidgettest.h"
#include "common/authenticationloginwidget.h"
#include <KPasswordLineEdit>
#include <QFormLayout>
#include <QLineEdit>
#include <QTest>
QTEST_MAIN(AuthenticationLoginWidgetTest)

AuthenticationLoginWidgetTest::AuthenticationLoginWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void AuthenticationLoginWidgetTest::shouldHaveDefaultValues()
{
    AuthenticationLoginWidget w;

    auto mainLayout = w.findChild<QFormLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mAccountName = w.findChild<QLineEdit *>(QStringLiteral("mAccountName"));
    QVERIFY(mAccountName);
    QVERIFY(mAccountName->text().isEmpty());
    QVERIFY(mAccountName->isClearButtonEnabled());

    auto mServerUrl = w.findChild<QLineEdit *>(QStringLiteral("mServerUrl"));
    QVERIFY(mServerUrl);
    QVERIFY(mServerUrl->text().isEmpty());

    auto mUserName = w.findChild<QLineEdit *>(QStringLiteral("mUserName"));
    QVERIFY(mUserName);
    QVERIFY(mUserName->text().isEmpty());
    QVERIFY(mUserName->isClearButtonEnabled());

    auto mPasswordLineEdit = w.findChild<KPasswordLineEdit *>(QStringLiteral("mPasswordLineEdit"));
    QVERIFY(mPasswordLineEdit);
}

#include "moc_authenticationloginwidgettest.cpp"
