/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "resetpassworddialogtest.h"
#include "dialogs/reportmessagewidget.h"
#include "dialogs/resetpassworddialog.h"
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QPushButton>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(ResetPasswordDialogTest)
ResetPasswordDialogTest::ResetPasswordDialogTest(QObject *parent)
    : QObject{parent}
{
}

void ResetPasswordDialogTest::shouldHaveDefaultValues()
{
    ResetPasswordDialog w;
    QVERIFY(!w.windowTitle().isEmpty());

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mResetPasswordWidget = w.findChild<ReportMessageWidget *>(QStringLiteral("mResetPasswordWidget"));
    QVERIFY(mResetPasswordWidget);

    auto button = w.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);

    QVERIFY(w.email().isEmpty());

    QPushButton *okButton = button->button(QDialogButtonBox::Ok);
    QVERIFY(!okButton->isEnabled());
}

void ResetPasswordDialogTest::shouldUpdateOkButton()
{
    ResetPasswordDialog w;
    auto mResetPasswordWidget = w.findChild<ReportMessageWidget *>(QStringLiteral("mResetPasswordWidget"));
    auto button = w.findChild<QDialogButtonBox *>(QStringLiteral("button"));

    auto mEmail = mResetPasswordWidget->findChild<QLineEdit *>(QStringLiteral("mEmail"));
    QVERIFY(mEmail);
    mEmail->setText(QStringLiteral("foo@kde.org"));
    QPushButton *okButton = button->button(QDialogButtonBox::Ok);
    QVERIFY(okButton->isEnabled());

    mEmail->setText(QStringLiteral("foo"));
    QVERIFY(!okButton->isEnabled());
}
