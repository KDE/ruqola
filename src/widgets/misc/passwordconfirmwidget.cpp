/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "passwordconfirmwidget.h"

#include <KAuthorized>
#include <KLocalizedString>
#include <KPasswordLineEdit>
#include <QFormLayout>

PasswordConfirmWidget::PasswordConfirmWidget(QWidget *parent)
    : QWidget(parent)
    , mNewPasswordLineEdit(new KPasswordLineEdit(this))
    , mConfirmPasswordLineEdit(new KPasswordLineEdit(this))
{
    auto mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mNewPasswordLineEdit->setObjectName(QStringLiteral("mNewPasswordLineEdit"));
    mainLayout->addRow(i18n("New Password:"), mNewPasswordLineEdit);
    mNewPasswordLineEdit->setRevealPasswordAvailable(KAuthorized::authorize(QStringLiteral("lineedit_reveal_password")));

    mConfirmPasswordLineEdit->setObjectName(QStringLiteral("mConfirmPasswordLineEdit"));
    mainLayout->addRow(i18n("Confirm Password:"), mConfirmPasswordLineEdit);
    mConfirmPasswordLineEdit->setRevealPasswordAvailable(KAuthorized::authorize(QStringLiteral("lineedit_reveal_password")));
}

PasswordConfirmWidget::~PasswordConfirmWidget() = default;

bool PasswordConfirmWidget::isNewPasswordConfirmed() const
{
    const QString password = mConfirmPasswordLineEdit->password();
    return !password.isEmpty() && (password == mNewPasswordLineEdit->password());
}

QString PasswordConfirmWidget::password() const
{
    return mConfirmPasswordLineEdit->password();
}

#include "moc_passwordconfirmwidget.cpp"
