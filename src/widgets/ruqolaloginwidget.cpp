/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "ruqolaloginwidget.h"
#include "common/authenticationcombobox.h"
#include "misc/passwordlineeditwidget.h"
#include "misc/twoauthenticationpasswordwidget.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include <KBusyIndicatorWidget>
#include <KColorScheme>
#include <KLocalizedString>
#include <KPasswordLineEdit>
#include <QCheckBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

RuqolaLoginWidget::RuqolaLoginWidget(QWidget *parent)
    : QWidget(parent)
    , mAccountName(new QLineEdit(this))
    , mServerName(new QLineEdit(this))
    , mUserName(new QLineEdit(this))
    , mPasswordLineEditWidget(new PasswordLineEditWidget(this))
    , mLdapCheckBox(new QCheckBox(i18n("Use LDAP"), this))
    , mLoginButton(new QPushButton(i18n("Login"), this))
    , mBusyIndicatorWidget(new KBusyIndicatorWidget(this))
    , mFailedError(new QLabel(this))
    , mTwoFactorAuthenticationPasswordLineEdit(new TwoAuthenticationPasswordWidget(this))
    , mAuthenticationWidget(new QWidget(this))
    , mAuthenticationAccountWidget(new QWidget(this))
    , mAuthenticationCombobox(new AuthenticationComboBox(this))
{
    auto mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mAccountName->setObjectName(QStringLiteral("mAccountName"));
    mainLayout->addRow(i18n("Account Name:"), mAccountName);
    mAccountName->setPlaceholderText(i18n("Account Name"));
    connect(mAccountName, &QLineEdit::textChanged, this, &RuqolaLoginWidget::slotUpdateLoginButton);

    mServerName->setObjectName(QStringLiteral("mServerName"));
    mainLayout->addRow(i18n("Server Name:"), mServerName);

    mUserName->setObjectName(QStringLiteral("mUserName"));
    mainLayout->addRow(i18n("User Name:"), mUserName);

    // Type of account
    mAuthenticationAccountWidget->setObjectName(QStringLiteral("mAuthenticationAccountWidget"));
    mAuthenticationAccountWidget->setVisible(false);

    auto authenticationAccountLayout = new QVBoxLayout(mAuthenticationAccountWidget);
    authenticationAccountLayout->setObjectName(QStringLiteral("authenticationAccountLayout"));

    auto authenticationAccountLabel = new QLabel(i18n("Authentication Method"), this);
    authenticationAccountLabel->setTextFormat(Qt::PlainText);
    authenticationAccountLabel->setObjectName(QStringLiteral("authenticationAccountLabel"));
    authenticationAccountLayout->addWidget(authenticationAccountLabel);

    mAuthenticationCombobox->setObjectName(QStringLiteral("mAuthenticationCombobox"));
    authenticationAccountLayout->addWidget(mAuthenticationCombobox);
    mainLayout->addWidget(mAuthenticationAccountWidget);
    mAuthenticationAccountWidget->setVisible(false);

    // Password
    mPasswordLineEditWidget->setObjectName(QStringLiteral("mPasswordLineEditWidget"));
    connect(mPasswordLineEditWidget->passwordLineEdit()->lineEdit(), &QLineEdit::returnPressed, this, &RuqolaLoginWidget::slotLogin);
    mainLayout->addRow(i18n("Password:"), mPasswordLineEditWidget);
    connect(mPasswordLineEditWidget, &PasswordLineEditWidget::resetPasswordRequested, this, &RuqolaLoginWidget::slotResetPasswordRequested);

    mLdapCheckBox->setObjectName(QStringLiteral("mLdapCheckBox"));
    mainLayout->addWidget(mLdapCheckBox);

    // Two Factor authentication
    mAuthenticationWidget->setObjectName(QStringLiteral("authenticationWidget"));
    mAuthenticationWidget->setVisible(false);

    auto twoFactorAuthenticationLayout = new QVBoxLayout(mAuthenticationWidget);
    twoFactorAuthenticationLayout->setObjectName(QStringLiteral("twoFactorAuthenticationLayout"));

    auto *twoFactorAuthenticationLabel =
        new QLabel(i18n("You have enabled second factor authentication.\nPlease enter the generated code or a backup code."), this);
    twoFactorAuthenticationLabel->setObjectName(QStringLiteral("twoFactorAuthenticationLabel"));
    twoFactorAuthenticationLayout->addWidget(twoFactorAuthenticationLabel);

    mTwoFactorAuthenticationPasswordLineEdit->setObjectName(QStringLiteral("mTwoFactorAuthenticationPasswordLineEdit"));
    twoFactorAuthenticationLayout->addWidget(mTwoFactorAuthenticationPasswordLineEdit);

    mainLayout->addWidget(mAuthenticationWidget);

    mLoginButton->setObjectName(QStringLiteral("mLoginButton"));
    mainLayout->addWidget(mLoginButton);
    connect(mLoginButton, &QPushButton::clicked, this, &RuqolaLoginWidget::slotLogin);

    mBusyIndicatorWidget->setObjectName(QStringLiteral("mBusyIndicatorWidget"));
    mainLayout->addWidget(mBusyIndicatorWidget);
    // Hide by default
    mBusyIndicatorWidget->hide();

    mFailedError->setObjectName(QStringLiteral("mFailedError"));
    QPalette pal = mFailedError->palette();
    const KColorScheme colorScheme{QPalette::Active};
    pal.setColor(foregroundRole(), colorScheme.foreground(KColorScheme::NegativeText).color());
    mFailedError->setPalette(pal);
    QFont font = mFailedError->font();
    font.setBold(true);
    mFailedError->setFont(font);

    mainLayout->addWidget(mFailedError);
    // Hide by default
    mFailedError->hide();
}

RuqolaLoginWidget::~RuqolaLoginWidget()
{
}

void RuqolaLoginWidget::slotUpdateLoginButton()
{
    mLoginButton->setEnabled(!mAccountName->text().isEmpty());
}

void RuqolaLoginWidget::initialize()
{
    auto *rocketChatAccount = Ruqola::self()->rocketChatAccount();
    mAccountName->setText(rocketChatAccount->displayName());
    mAccountName->setReadOnly(!rocketChatAccount->displayName().isEmpty());
    mServerName->setText(rocketChatAccount->serverUrl());
    mUserName->setText(rocketChatAccount->userName());
    mLdapCheckBox->setChecked(rocketChatAccount->useLdap());
    mPasswordLineEditWidget->passwordLineEdit()->setPassword(rocketChatAccount->password());
    mAuthenticationCombobox->setVisible(mAuthenticationCombobox->count() > 1);
    mLdapCheckBox->setVisible(rocketChatAccount->ldapEnabled());
}

void RuqolaLoginWidget::slotLogin()
{
    auto *rocketChatAccount = Ruqola::self()->rocketChatAccount();
    rocketChatAccount->setAccountName(mAccountName->isEnabled() ? mAccountName->text() : rocketChatAccount->accountName());
    rocketChatAccount->setServerUrl(mServerName->text());
    rocketChatAccount->setUserName(mUserName->text());
    rocketChatAccount->setPassword(mPasswordLineEditWidget->passwordLineEdit()->password());
    rocketChatAccount->setUseLdap(mLdapCheckBox->isChecked());
    if (!mAuthenticationWidget->isHidden()) {
        rocketChatAccount->setTwoFactorAuthenticationCode(mTwoFactorAuthenticationPasswordLineEdit->code());
    } else {
        mTwoFactorAuthenticationPasswordLineEdit->clear();
    }
    rocketChatAccount->tryLogin();
}

void RuqolaLoginWidget::changeWidgetStatus(bool enabled)
{
    mServerName->setEnabled(enabled);
    mUserName->setEnabled(enabled);
    mPasswordLineEditWidget->setEnabled(enabled);
    mLoginButton->setEnabled(enabled && !mAccountName->text().isEmpty());
}

void RuqolaLoginWidget::setLoginStatus(DDPAuthenticationManager::LoginStatus status)
{
    mFailedError->setHidden(true);
    switch (status) {
    case DDPAuthenticationManager::LoginStatus::Connecting:
    case DDPAuthenticationManager::LoginStatus::LoginOngoing:
        mBusyIndicatorWidget->show();
        changeWidgetStatus(false);
        mAuthenticationWidget->setVisible(false);
        break;
    case DDPAuthenticationManager::LoginStatus::LoggedOut:
    case DDPAuthenticationManager::LoginStatus::LoggedIn:
        mBusyIndicatorWidget->hide();
        changeWidgetStatus(true);
        mAuthenticationWidget->setVisible(false);
        break;
    case DDPAuthenticationManager::LoginStatus::LoginFailedInvalidUserOrPassword:
        mBusyIndicatorWidget->hide();
        changeWidgetStatus(true);
        showError(i18n("Login Failed: invalid username or password"));
        mAuthenticationWidget->setVisible(false);
        break;
    case DDPAuthenticationManager::LoginFailedInvalidOtp:
        showError(i18n("Login failed: Invalid OTP code."));
        Q_FALLTHROUGH();
    case DDPAuthenticationManager::LoginStatus::LoginOtpRequired:
        mBusyIndicatorWidget->hide();
        changeWidgetStatus(true);
        mAuthenticationWidget->setVisible(true);
        break;
    case DDPAuthenticationManager::LoginStatus::FailedToLoginPluginProblem:
        mBusyIndicatorWidget->hide();
        changeWidgetStatus(true);
        showError(i18n("Installation Problem found. No plugins found here."));
        mAuthenticationWidget->setVisible(false);
        break;
    case DDPAuthenticationManager::LoginStatus::GenericError:
        mBusyIndicatorWidget->hide();
        changeWidgetStatus(true);
        showError(i18n("Login Failed: generic error"));
        mAuthenticationWidget->setVisible(false);
        break;
    case DDPAuthenticationManager::LoginOtpAuthOngoing:
    case DDPAuthenticationManager::LogoutOngoing:
    case DDPAuthenticationManager::LogoutCleanUpOngoing:
    case DDPAuthenticationManager::LoggedOutAndCleanedUp:
        // TODO
        mAuthenticationWidget->setVisible(false);
        mBusyIndicatorWidget->hide();
        changeWidgetStatus(true);
        break;
    }
}

void RuqolaLoginWidget::showError(const QString &text)
{
    mFailedError->setVisible(true);
    mFailedError->setText(text);
}

void RuqolaLoginWidget::slotResetPasswordRequested(const QString &email)
{
    auto *rocketChatAccount = Ruqola::self()->rocketChatAccount();
    rocketChatAccount->requestNewPassword(email);
}
