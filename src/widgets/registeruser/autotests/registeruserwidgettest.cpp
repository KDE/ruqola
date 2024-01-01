/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "registeruserwidgettest.h"
#include "registeruser/registeruserwidget.h"
#include <KPasswordLineEdit>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QTest>
QTEST_MAIN(RegisterUserWidgetTest)
RegisterUserWidgetTest::RegisterUserWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void RegisterUserWidgetTest::shouldHaveDefaultValues()
{
    RegisterUserWidget w(nullptr);
    auto mainLayout = w.findChild<QFormLayout *>(QStringLiteral("mainLayout"));
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mUserName = w.findChild<QLineEdit *>(QStringLiteral("mUserName"));
    QVERIFY(mUserName);
    QVERIFY(mUserName->text().isEmpty());

    auto mEmail = w.findChild<QLineEdit *>(QStringLiteral("mEmail"));
    QVERIFY(mEmail);
    QVERIFY(mEmail->text().isEmpty());

    auto mPasswordLineEdit = w.findChild<KPasswordLineEdit *>(QStringLiteral("mPasswordLineEdit"));
    QVERIFY(mPasswordLineEdit);
    QVERIFY(mPasswordLineEdit->password().isEmpty());

    auto mConfirmPasswordLineEdit = w.findChild<KPasswordLineEdit *>(QStringLiteral("mConfirmPasswordLineEdit"));
    QVERIFY(mConfirmPasswordLineEdit);
    QVERIFY(mConfirmPasswordLineEdit->password().isEmpty());

    auto mRegisterButton = w.findChild<QPushButton *>(QStringLiteral("mRegisterButton"));
    QVERIFY(mRegisterButton);
    QVERIFY(!mRegisterButton->isEnabled());
    QVERIFY(!mRegisterButton->text().isEmpty());
}

#include "moc_registeruserwidgettest.cpp"
