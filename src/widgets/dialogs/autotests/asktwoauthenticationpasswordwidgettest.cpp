/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "asktwoauthenticationpasswordwidgettest.h"
#include "dialogs/asktwoauthenticationpasswordwidget.h"
#include "misc/twoauthenticationpasswordwidget.h"
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(AskTwoAuthenticationPasswordWidgetTest)

AskTwoAuthenticationPasswordWidgetTest::AskTwoAuthenticationPasswordWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void AskTwoAuthenticationPasswordWidgetTest::shouldHaveDefaultValues()
{
    AskTwoAuthenticationPasswordWidget w;

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins());

    auto mTwoFactorAuthenticationPasswordLineEdit = w.findChild<TwoAuthenticationPasswordWidget *>(QStringLiteral("mTwoFactorAuthenticationPasswordLineEdit"));
    QVERIFY(mTwoFactorAuthenticationPasswordLineEdit);
    QVERIFY(mTwoFactorAuthenticationPasswordLineEdit->code().isEmpty());
    QVERIFY(!mTwoFactorAuthenticationPasswordLineEdit->rocketChatAccount());
    QVERIFY(!w.rocketChatAccount());
}
