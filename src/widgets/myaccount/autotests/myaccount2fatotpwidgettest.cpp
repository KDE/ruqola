/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "myaccount2fatotpwidgettest.h"
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
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});
    auto mTotpQrCode = w.findChild<QLabel *>(QStringLiteral("mTotpQrCode"));
    QVERIFY(mTotpQrCode);

    auto mQrCoreLabel = w.findChild<QLabel *>(QStringLiteral("mQrCoreLabel"));
    QVERIFY(mQrCoreLabel);
    QCOMPARE(mQrCoreLabel->textFormat(), Qt::PlainText);
    QCOMPARE(mQrCoreLabel->textInteractionFlags(), Qt::TextSelectableByMouse);

    auto mVerifyQrCode = w.findChild<QLineEdit *>(QStringLiteral("mVerifyQrCode"));
    QVERIFY(mVerifyQrCode);
    QVERIFY(mVerifyQrCode->text().isEmpty());
    QVERIFY(!mVerifyQrCode->placeholderText().isEmpty());

    auto label = w.findChild<QLabel *>(QStringLiteral("label"));
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());

    auto verifyButton = w.findChild<QPushButton *>(QStringLiteral("verifyButton"));
    QVERIFY(verifyButton);
    QVERIFY(!verifyButton->text().isEmpty());
    QVERIFY(!verifyButton->isEnabled());
}

#include "moc_myaccount2fatotpwidgettest.cpp"
