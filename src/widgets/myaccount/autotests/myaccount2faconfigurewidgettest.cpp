/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccount2faconfigurewidgettest.h"
using namespace Qt::Literals::StringLiterals;

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
    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mActivate2FAViaEmailCheckbox = w.findChild<QCheckBox *>(u"mActivate2FAViaEmailCheckbox"_s);
    QVERIFY(mActivate2FAViaEmailCheckbox);
    QVERIFY(!mActivate2FAViaEmailCheckbox->isChecked());
    QVERIFY(!mActivate2FAViaEmailCheckbox->text().isEmpty());

    auto mActivate2FAViaTOTPCheckbox = w.findChild<QCheckBox *>(u"mActivate2FAViaTOTPCheckbox"_s);
    QVERIFY(mActivate2FAViaTOTPCheckbox);
    QVERIFY(!mActivate2FAViaTOTPCheckbox->isChecked());
    QVERIFY(!mActivate2FAViaTOTPCheckbox->text().isEmpty());

    auto mTotpQrCode = w.findChild<QLabel *>(u"mTotpQrCode"_s);
    QVERIFY(mTotpQrCode);
    QVERIFY(mTotpQrCode->text().isEmpty());

    auto mMyAccount2FaTotpWidget = w.findChild<MyAccount2FaTotpWidget *>(u"mMyAccount2FaTotpWidget"_s);
    QVERIFY(mMyAccount2FaTotpWidget);

    auto mMyAccountDisable2FaTotpWidget = w.findChild<MyAccount2FaDisableTotpWidget *>(u"mMyAccountDisable2FaTotpWidget"_s);
    QVERIFY(mMyAccountDisable2FaTotpWidget);

    auto mMyAccount2FaEmpty = w.findChild<QWidget *>(u"mMyAccount2FaEmpty"_s);
    QVERIFY(mMyAccount2FaEmpty);

    auto mStackedWidget = w.findChild<QStackedWidget *>(u"mStackedWidget"_s);
    QVERIFY(mStackedWidget);
}

#include "moc_myaccount2faconfigurewidgettest.cpp"
