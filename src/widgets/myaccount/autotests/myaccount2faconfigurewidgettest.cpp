/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccount2faconfigurewidgettest.h"
#include "myaccount/myaccount2faconfigurewidget.h"
#include "myaccount/myaccount2fadisabletotpwidget.h"
#include "myaccount/myaccount2fatotpwidget.h"
#include <QCheckBox>
#include <QLabel>
#include <QStackedWidget>
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

    auto mMyAccount2FaTotpWidget = w.findChild<MyAccount2FaTotpWidget *>(QStringLiteral("mMyAccount2FaTotpWidget"));
    QVERIFY(mMyAccount2FaTotpWidget);

    auto mMyAccountDisable2FaTotpWidget = w.findChild<MyAccount2FaDisableTotpWidget *>(QStringLiteral("mMyAccountDisable2FaTotpWidget"));
    QVERIFY(mMyAccountDisable2FaTotpWidget);

    auto mMyAccount2FaEmpty = w.findChild<QWidget *>(QStringLiteral("mMyAccount2FaEmpty"));
    QVERIFY(mMyAccount2FaEmpty);

    auto mStackedWidget = w.findChild<QStackedWidget *>(QStringLiteral("mStackedWidget"));
    QVERIFY(mStackedWidget);
}

#include "moc_myaccount2faconfigurewidgettest.cpp"
