/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "passwordconfirmwidget.h"
#include "misc/passwordvalidatewidget.h"

#include <KAuthorized>
#include <KLocalizedString>
#include <KPasswordLineEdit>
#include <QFormLayout>

PasswordConfirmWidget::PasswordConfirmWidget(QWidget *parent)
    : QWidget(parent)
    , mNewPasswordLineEdit(new KPasswordLineEdit(this))
    , mConfirmPasswordLineEdit(new KPasswordLineEdit(this))
    , mPasswordValidateWidget(new PasswordValidateWidget(this))
{
    auto mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mNewPasswordLineEdit->setObjectName(QStringLiteral("mNewPasswordLineEdit"));
    mainLayout->addRow(i18n("New Password:"), mNewPasswordLineEdit);
    mNewPasswordLineEdit->setRevealPasswordMode(KAuthorized::authorize(QStringLiteral("lineedit_reveal_password")) ? KPassword::RevealMode::OnlyNew
                                                                                                                   : KPassword::RevealMode::Never);
    mPasswordValidateWidget->setObjectName(QStringLiteral("mPasswordValidateWidget"));
    mainLayout->addWidget(mPasswordValidateWidget);
    connect(mNewPasswordLineEdit, &KPasswordLineEdit::passwordChanged, mPasswordValidateWidget, &PasswordValidateWidget::validatePassword);

    mConfirmPasswordLineEdit->setObjectName(QStringLiteral("mConfirmPasswordLineEdit"));
    mainLayout->addRow(i18n("Confirm Password:"), mConfirmPasswordLineEdit);
    mConfirmPasswordLineEdit->setRevealPasswordMode(KAuthorized::authorize(QStringLiteral("lineedit_reveal_password")) ? KPassword::RevealMode::OnlyNew
                                                                                                                       : KPassword::RevealMode::Never);
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

void PasswordConfirmWidget::setPasswordValidChecks(const RuqolaServerConfig::PasswordSettings &passwordSettings)
{
    mPasswordValidateWidget->setPasswordValidChecks(passwordSettings);
}

#include "moc_passwordconfirmwidget.cpp"
