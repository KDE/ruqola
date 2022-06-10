/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ruqolaloginwidget.h"
#include "colors.h"
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
    , mServerUrl(new QLineEdit(this))
    , mUserName(new QLineEdit(this))
    , mPasswordLineEditWidget(new PasswordLineEditWidget(this))
    // , mLdapCheckBox(new QCheckBox(i18n("Use LDAP"), this))
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

    mServerUrl->setObjectName(QStringLiteral("mServerUrl"));
    mainLayout->addRow(i18n("Server Url:"), mServerUrl);

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

    //    mLdapCheckBox->setObjectName(QStringLiteral("mLdapCheckBox"));
    //    mainLayout->addWidget(mLdapCheckBox);

    // Two Factor authentication
    mAuthenticationWidget->setObjectName(QStringLiteral("authenticationWidget"));
    mAuthenticationWidget->setVisible(false);

    auto twoFactorAuthenticationLayout = new QVBoxLayout(mAuthenticationWidget);
    twoFactorAuthenticationLayout->setObjectName(QStringLiteral("twoFactorAuthenticationLayout"));
    twoFactorAuthenticationLayout->setContentsMargins(QMargins{});

    auto twoFactorAuthenticationLabel =
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
    pal.setColor(foregroundRole(), Colors::self().schemeView().foreground(KColorScheme::NegativeText).color());
    mFailedError->setPalette(pal);
    QFont font = mFailedError->font();
    font.setBold(true);
    mFailedError->setFont(font);

    mainLayout->addWidget(mFailedError);
    // Hide by default
    mFailedError->hide();
}

RuqolaLoginWidget::~RuqolaLoginWidget() = default;

void RuqolaLoginWidget::slotUpdateLoginButton()
{
    mLoginButton->setEnabled(!mAccountName->text().isEmpty());
}

void RuqolaLoginWidget::setRocketChatAccount(RocketChatAccount *rocketChatAccount)
{
    mRocketChatAccount = rocketChatAccount;
    mAccountName->setText(mRocketChatAccount->displayName());
    mAccountName->setReadOnly(!mRocketChatAccount->displayName().isEmpty());
    mServerUrl->setText(mRocketChatAccount->serverUrl());
    mUserName->setText(mRocketChatAccount->userName());
    // mLdapCheckBox->setChecked(mRocketChatAccount->useLdap());
    disconnect(mUpdatePasswordConnection);
    mPasswordLineEditWidget->passwordLineEdit()->setPassword(mRocketChatAccount->password());
    mUpdatePasswordConnection = connect(mRocketChatAccount, &RocketChatAccount::passwordChanged, mPasswordLineEditWidget, [this]() {
        mPasswordLineEditWidget->passwordLineEdit()->setPassword(mRocketChatAccount->password());
    });
    mAuthenticationCombobox->switchRocketChatAccount(mRocketChatAccount); // Authentication is rocketchatAccount specific
    mAuthenticationCombobox->setVisible(mAuthenticationCombobox->count() > 1);
    // mLdapCheckBox->setVisible(rocketChatAccount->ldapEnabled());
    mTwoFactorAuthenticationPasswordLineEdit->setRocketChatAccount(mRocketChatAccount);
    mPasswordLineEditWidget->setAllowPasswordReset(mRocketChatAccount->allowPasswordReset());
}

void RuqolaLoginWidget::slotLogin()
{
    mRocketChatAccount->setAccountName(mAccountName->isEnabled() ? mAccountName->text() : mRocketChatAccount->accountName());
    mRocketChatAccount->setServerUrl(mServerUrl->text());
    mRocketChatAccount->setUserName(mUserName->text());
    mRocketChatAccount->setPassword(mPasswordLineEditWidget->passwordLineEdit()->password());
    // mRocketChatAccount->setUseLdap(mLdapCheckBox->isChecked());
    if (!mAuthenticationWidget->isHidden()) {
        mRocketChatAccount->setTwoFactorAuthenticationCode(mTwoFactorAuthenticationPasswordLineEdit->code());
    } else {
        mTwoFactorAuthenticationPasswordLineEdit->clear();
    }
    mRocketChatAccount->tryLogin();
}

void RuqolaLoginWidget::changeWidgetStatus(bool enabled)
{
    mServerUrl->setEnabled(enabled);
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
    mRocketChatAccount->requestNewPassword(email);
}
