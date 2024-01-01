/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "myaccount2fadisabletotpwidgettest.h"
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

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto label = w.findChild<QLabel *>(QStringLiteral("label"));
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());

    auto hboxLayout = w.findChild<QHBoxLayout *>(QStringLiteral("hboxLayout"));
    QVERIFY(hboxLayout);
    QCOMPARE(hboxLayout->contentsMargins(), QMargins{});

    auto mDisableCodeLineEdit = w.findChild<QLineEdit *>(QStringLiteral("mDisableCodeLineEdit"));
    QVERIFY(mDisableCodeLineEdit);
    QVERIFY(!mDisableCodeLineEdit->placeholderText().isEmpty());

    auto verifyButton = w.findChild<QPushButton *>(QStringLiteral("verifyButton"));
    QVERIFY(verifyButton);
    QVERIFY(!verifyButton->text().isEmpty());
    QVERIFY(!verifyButton->isEnabled());

    auto regenerateCode = w.findChild<QPushButton *>(QStringLiteral("regenerateCode"));
    QVERIFY(regenerateCode);
    QVERIFY(!regenerateCode->text().isEmpty());
}

#include "moc_myaccount2fadisabletotpwidgettest.cpp"
