/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "registeruserwidget.h"
#include "rocketchataccount.h"
#include <KAuthorized>
#include <KLocalizedString>
#include <KPasswordLineEdit>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>

RegisterUserWidget::RegisterUserWidget(QWidget *parent)
    : QWidget(parent)
    , mRegisterButton(new QPushButton(i18n("Register"), this))
    , mUserName(new QLineEdit(this))
    , mEmail(new QLineEdit(this))
    , mPasswordLineEdit(new KPasswordLineEdit(this))
    , mConfirmPasswordLineEdit(new KPasswordLineEdit(this))
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

    mPasswordLineEdit->setObjectName(QStringLiteral("mPasswordLineEdit"));
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    mPasswordLineEdit->setRevealPasswordAvailable(KAuthorized::authorize(QStringLiteral("lineedit_reveal_password")));
#else
    mPasswordLineEdit->setRevealPasswordMode(KAuthorized::authorize(QStringLiteral("lineedit_reveal_password")) ? KPasswordLineEdit::RevealPasswordMode::Normal
                                                                                                                : KPasswordLineEdit::RevealPasswordMode::Never);
#endif;
    mainLayout->addRow(i18n("Password:"), mPasswordLineEdit);
    connect(mPasswordLineEdit, &KPasswordLineEdit::passwordChanged, this, &RegisterUserWidget::slotUpdateRegisterButton);

    mConfirmPasswordLineEdit->setObjectName(QStringLiteral("mConfirmPasswordLineEdit"));
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    mConfirmPasswordLineEdit->setRevealPasswordAvailable(KAuthorized::authorize(QStringLiteral("lineedit_reveal_password")));
#else
    mConfirmPasswordLineEdit->setRevealPasswordMode(KAuthorized::authorize(QStringLiteral("lineedit_reveal_password"))
                                                        ? KPasswordLineEdit::RevealPasswordMode::Normal
                                                        : KPasswordLineEdit::RevealPasswordMode::Never);
#endif;
    mainLayout->addRow(i18n("Confirm Password:"), mConfirmPasswordLineEdit);
    connect(mConfirmPasswordLineEdit, &KPasswordLineEdit::passwordChanged, this, &RegisterUserWidget::slotUpdateRegisterButton);

    mRegisterButton->setObjectName(QStringLiteral("mRegisterButton"));
    connect(mRegisterButton, &QPushButton::clicked, this, &RegisterUserWidget::slotRegisterNewUser);
    mainLayout->addWidget(mRegisterButton);
    mRegisterButton->setEnabled(false);
}

RegisterUserWidget::~RegisterUserWidget() = default;

void RegisterUserWidget::slotUpdateRegisterButton()
{
    const bool enableRegisterButton = !mUserName->text().trimmed().isEmpty() && !mEmail->text().trimmed().isEmpty() && !mPasswordLineEdit->password().isEmpty()
        && (mPasswordLineEdit->password() == mConfirmPasswordLineEdit->password());
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
    info.password = mPasswordLineEdit->password();
    return info;
}

#include "moc_registeruserwidget.cpp"
