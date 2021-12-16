/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccount2faconfigurewidgettest.h"
#include "myaccount/myaccount2faconfigurewidget.h"
#include <QCheckBox>
#include <QLabel>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(MyAccount2FaConfigureWidgetTest)
MyAccount2FaConfigureWidgetTest::MyAccount2FaConfigureWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void MyAccount2FaConfigureWidgetTest::shouldHaveDefaultValues()
{
    MyAccount2FaConfigureWidget w(nullptr);
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mActivate2FAViaEmailCheckbox = w.findChild<QCheckBox *>(QStringLiteral("mActivate2FAViaEmailCheckbox"));
    QVERIFY(mActivate2FAViaEmailCheckbox);
    QVERIFY(!mActivate2FAViaEmailCheckbox->isChecked());
    QVERIFY(!mActivate2FAViaEmailCheckbox->text().isEmpty());

    auto mActivate2FAViaTOTPCheckbox = w.findChild<QCheckBox *>(QStringLiteral("mActivate2FAViaTOTPCheckbox"));
    QVERIFY(mActivate2FAViaTOTPCheckbox);
    QVERIFY(!mActivate2FAViaTOTPCheckbox->isChecked());
    QVERIFY(!mActivate2FAViaTOTPCheckbox->text().isEmpty());

    auto mTotpQrCode = w.findChild<QLabel *>(QStringLiteral("mTotpQrCode"));
    QVERIFY(mTotpQrCode);
    QVERIFY(mTotpQrCode->text().isEmpty());
}
