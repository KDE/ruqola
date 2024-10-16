/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "registeruserwidget.h"
#include "misc/passwordconfirmwidget.h"

#include <KAuthorized>
#include <KLocalizedString>
#include <KPasswordLineEdit>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>

RegisterUserWidget::RegisterUserWidget(QWidget *parent)
    : QWidget(parent)
    , mRegisterButton(new QPushButton(i18nc("@action:button", "Register"), this))
    , mUserName(new QLineEdit(this))
    , mEmail(new QLineEdit(this))
    , mPasswordConfirmWidget(new PasswordConfirmWidget(this))
{
    auto mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mUserName->setObjectName(QStringLiteral("mUserName"));
    mainLayout->addRow(i18n("Username:"), mUserName);
    connect(mUserName, &QLineEdit::textChanged, this, &RegisterUserWidget::slotUpdateRegisterButton);

    mEmail->setObjectName(QStringLiteral("mEmail"));
    mainLayout->addRow(i18n("Email:"), mEmail);
    connect(mEmail, &QLineEdit::textChanged, this, &RegisterUserWidget::slotUpdateRegisterButton);

    mPasswordConfirmWidget->setObjectName(QStringLiteral("mPasswordConfirmWidget"));
    mainLayout->addRow(mPasswordConfirmWidget);
    connect(mPasswordConfirmWidget, &PasswordConfirmWidget::passwordValidated, this, &RegisterUserWidget::slotUpdateRegisterButton);

    mRegisterButton->setObjectName(QStringLiteral("mRegisterButton"));
    connect(mRegisterButton, &QPushButton::clicked, this, &RegisterUserWidget::slotRegisterNewUser);
    mainLayout->addWidget(mRegisterButton);
    mRegisterButton->setEnabled(false);
}

RegisterUserWidget::~RegisterUserWidget() = default;

void RegisterUserWidget::setPasswordValidChecks(const RuqolaServerConfig::PasswordSettings &passwordSettings)
{
    mPasswordConfirmWidget->setPasswordValidChecks(passwordSettings);
}

void RegisterUserWidget::slotUpdateRegisterButton()
{
    const bool enableRegisterButton =
        !mUserName->text().trimmed().isEmpty() && !mEmail->text().trimmed().isEmpty() && mPasswordConfirmWidget->isNewPasswordConfirmed();
    mRegisterButton->setEnabled(enableRegisterButton);
}

void RegisterUserWidget::slotRegisterNewUser()
{
    mRegisterButton->setEnabled(false);
    Q_EMIT registerNewAccount();
}

RocketChatRestApi::RegisterUserJob::RegisterUserInfo RegisterUserWidget::registerUserInfo() const
{
    RocketChatRestApi::RegisterUserJob::RegisterUserInfo info;
    info.email = mEmail->text();
    info.name = mUserName->text();
    info.username = mUserName->text().remove(QLatin1Char(' '));
    info.password = mPasswordConfirmWidget->password();
    return info;
}

#include "moc_registeruserwidget.cpp"
