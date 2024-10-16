/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "registeruserwidgettest.h"
#include "misc/passwordconfirmwidget.h"
#include "registeruser/registeruserwidget.h"
#include <KPasswordLineEdit>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
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

    auto mPasswordConfirmWidget = w.findChild<PasswordConfirmWidget *>(QStringLiteral("mPasswordConfirmWidget"));
    QVERIFY(mPasswordConfirmWidget);

    auto mRegisterButton = w.findChild<QPushButton *>(QStringLiteral("mRegisterButton"));
    QVERIFY(mRegisterButton);
    QVERIFY(!mRegisterButton->isEnabled());
    QVERIFY(!mRegisterButton->text().isEmpty());

    auto mReasonTextEdit = w.findChild<QPlainTextEdit *>(QStringLiteral("mReasonTextEdit"));
    QVERIFY(mReasonTextEdit);
    QVERIFY(mReasonTextEdit->document()->isEmpty());

    auto mReasonLabel = w.findChild<QLabel *>(QStringLiteral("mReasonLabel"));
    QVERIFY(mReasonLabel);
    QVERIFY(!mReasonLabel->text().isEmpty());
}

#include "moc_registeruserwidgettest.cpp"
