/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "twoauthenticationpasswordwidgettest.h"
#include "misc/twoauthenticationpasswordwidget.h"
#include <KPasswordLineEdit>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTest>
QTEST_MAIN(TwoAuthenticationPasswordWidgetTest)

TwoAuthenticationPasswordWidgetTest::TwoAuthenticationPasswordWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void TwoAuthenticationPasswordWidgetTest::shouldHaveDefaultValues()
{
    TwoAuthenticationPasswordWidget w;

    auto twoFactorLayout = w.findChild<QHBoxLayout *>(QStringLiteral("twoFactorLayout"));
    QVERIFY(twoFactorLayout);
    QCOMPARE(twoFactorLayout->contentsMargins(), QMargins());

    auto mTwoFactorAuthenticationPasswordLineEdit = w.findChild<KPasswordLineEdit *>(QStringLiteral("mTwoFactorAuthenticationPasswordLineEdit"));
    QVERIFY(mTwoFactorAuthenticationPasswordLineEdit);

    auto sendNewEmailCode = w.findChild<QPushButton *>(QStringLiteral("sendNewEmailCode"));
    QVERIFY(sendNewEmailCode);
    QVERIFY(!sendNewEmailCode->text().isEmpty());
}

#include "moc_twoauthenticationpasswordwidgettest.cpp"
