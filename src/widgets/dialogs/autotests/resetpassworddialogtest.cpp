/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "resetpassworddialogtest.h"
using namespace Qt::Literals::StringLiterals;

#include "dialogs/resetpassworddialog.h"
#include "dialogs/resetpasswordwidget.h"
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

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mResetPasswordWidget = w.findChild<ResetPasswordWidget *>(u"mResetPasswordWidget"_s);
    QVERIFY(mResetPasswordWidget);

    auto button = w.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(button);

    QVERIFY(w.email().isEmpty());

    QPushButton *okButton = button->button(QDialogButtonBox::Ok);
    QVERIFY(!okButton->isEnabled());
}

void ResetPasswordDialogTest::shouldUpdateOkButton()
{
    ResetPasswordDialog w;
    auto mResetPasswordWidget = w.findChild<ResetPasswordWidget *>(u"mResetPasswordWidget"_s);
    auto button = w.findChild<QDialogButtonBox *>(u"button"_s);

    auto mEmail = mResetPasswordWidget->findChild<QLineEdit *>(u"mEmail"_s);
    QVERIFY(mEmail);
    mEmail->setText(u"foo@kde.org"_s);
    QPushButton *okButton = button->button(QDialogButtonBox::Ok);
    QVERIFY(okButton->isEnabled());

    mEmail->setText(u"foo"_s);
    QVERIFY(!okButton->isEnabled());
}

#include "moc_resetpassworddialogtest.cpp"
