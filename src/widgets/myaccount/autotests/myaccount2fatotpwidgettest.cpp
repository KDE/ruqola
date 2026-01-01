/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "myaccount2fatotpwidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "myaccount/myaccount2fatotpwidget.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(MyAccount2FaTotpWidgetTest)
MyAccount2FaTotpWidgetTest::MyAccount2FaTotpWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void MyAccount2FaTotpWidgetTest::shouldHaveDefaultValues()
{
    MyAccount2FaTotpWidget w(nullptr);
    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});
    auto mTotpQrCode = w.findChild<QLabel *>(u"mTotpQrCode"_s);
    QVERIFY(mTotpQrCode);

    auto mQrCoreLabel = w.findChild<QLabel *>(u"mQrCoreLabel"_s);
    QVERIFY(mQrCoreLabel);
    QCOMPARE(mQrCoreLabel->textFormat(), Qt::PlainText);
    QCOMPARE(mQrCoreLabel->textInteractionFlags(), Qt::TextSelectableByMouse);

    auto mVerifyQrCode = w.findChild<QLineEdit *>(u"mVerifyQrCode"_s);
    QVERIFY(mVerifyQrCode);
    QVERIFY(mVerifyQrCode->text().isEmpty());
    QVERIFY(!mVerifyQrCode->placeholderText().isEmpty());

    auto label = w.findChild<QLabel *>(u"label"_s);
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());

    auto verifyButton = w.findChild<QPushButton *>(u"verifyButton"_s);
    QVERIFY(verifyButton);
    QVERIFY(!verifyButton->text().isEmpty());
    QVERIFY(!verifyButton->isEnabled());
}

#include "moc_myaccount2fatotpwidgettest.cpp"
