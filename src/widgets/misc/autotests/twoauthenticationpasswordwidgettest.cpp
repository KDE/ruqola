/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "twoauthenticationpasswordwidgettest.h"
using namespace Qt::Literals::StringLiterals;

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

    auto twoFactorLayout = w.findChild<QHBoxLayout *>(u"twoFactorLayout"_s);
    QVERIFY(twoFactorLayout);
    QCOMPARE(twoFactorLayout->contentsMargins(), QMargins());

    auto mTwoFactorAuthenticationPasswordLineEdit = w.findChild<KPasswordLineEdit *>(u"mTwoFactorAuthenticationPasswordLineEdit"_s);
    QVERIFY(mTwoFactorAuthenticationPasswordLineEdit);

    auto sendNewEmailCode = w.findChild<QPushButton *>(u"sendNewEmailCode"_s);
    QVERIFY(sendNewEmailCode);
    QVERIFY(!sendNewEmailCode->text().isEmpty());
}

#include "moc_twoauthenticationpasswordwidgettest.cpp"
