/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "myaccount2fadisabletotpwidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "myaccount/myaccount2fadisabletotpwidget.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(MyAccount2FaDisableTotpWidgetTest)
MyAccount2FaDisableTotpWidgetTest::MyAccount2FaDisableTotpWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void MyAccount2FaDisableTotpWidgetTest::shouldHaveDefaultValues()
{
    MyAccount2FaDisableTotpWidget w(nullptr);

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto label = w.findChild<QLabel *>(u"label"_s);
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());

    auto hboxLayout = w.findChild<QHBoxLayout *>(u"hboxLayout"_s);
    QVERIFY(hboxLayout);
    QCOMPARE(hboxLayout->contentsMargins(), QMargins{});

    auto mDisableCodeLineEdit = w.findChild<QLineEdit *>(u"mDisableCodeLineEdit"_s);
    QVERIFY(mDisableCodeLineEdit);
    QVERIFY(!mDisableCodeLineEdit->placeholderText().isEmpty());

    auto verifyButton = w.findChild<QPushButton *>(u"verifyButton"_s);
    QVERIFY(verifyButton);
    QVERIFY(!verifyButton->text().isEmpty());
    QVERIFY(!verifyButton->isEnabled());

    auto regenerateCode = w.findChild<QPushButton *>(u"regenerateCode"_s);
    QVERIFY(regenerateCode);
    QVERIFY(!regenerateCode->text().isEmpty());
}

#include "moc_myaccount2fadisabletotpwidgettest.cpp"
