/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "asktwoauthenticationpasswordwidgettest.h"
using namespace Qt::Literals::StringLiterals;

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

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins());

    auto mTwoFactorAuthenticationPasswordLineEdit = w.findChild<TwoAuthenticationPasswordWidget *>(u"mTwoFactorAuthenticationPasswordLineEdit"_s);
    QVERIFY(mTwoFactorAuthenticationPasswordLineEdit);
    QVERIFY(mTwoFactorAuthenticationPasswordLineEdit->code().isEmpty());
    QVERIFY(!mTwoFactorAuthenticationPasswordLineEdit->rocketChatAccount());
    QVERIFY(!w.rocketChatAccount());
}

#include "moc_asktwoauthenticationpasswordwidgettest.cpp"
