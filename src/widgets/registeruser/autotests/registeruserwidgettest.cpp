/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "registeruserwidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "misc/passwordconfirmwidget.h"
#include "registeruser/registeruserwidget.h"
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
    auto mainLayout = w.findChild<QFormLayout *>(u"mainLayout"_s);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mUserName = w.findChild<QLineEdit *>(u"mUserName"_s);
    QVERIFY(mUserName);
    QVERIFY(mUserName->text().isEmpty());

    auto mEmail = w.findChild<QLineEdit *>(u"mEmail"_s);
    QVERIFY(mEmail);
    QVERIFY(mEmail->text().isEmpty());

    auto mPasswordConfirmWidget = w.findChild<PasswordConfirmWidget *>(u"mPasswordConfirmWidget"_s);
    QVERIFY(mPasswordConfirmWidget);

    auto mRegisterButton = w.findChild<QPushButton *>(u"mRegisterButton"_s);
    QVERIFY(mRegisterButton);
    QVERIFY(!mRegisterButton->isEnabled());
    QVERIFY(!mRegisterButton->text().isEmpty());

    auto mReasonTextEdit = w.findChild<QPlainTextEdit *>(u"mReasonTextEdit"_s);
    QVERIFY(mReasonTextEdit);
    QVERIFY(mReasonTextEdit->document()->isEmpty());

    auto mReasonLabel = w.findChild<QLabel *>(u"mReasonLabel"_s);
    QVERIFY(mReasonLabel);
    QVERIFY(!mReasonLabel->text().isEmpty());
}

#include "moc_registeruserwidgettest.cpp"
