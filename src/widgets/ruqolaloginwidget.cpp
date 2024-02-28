/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ruqolaloginwidget.h"
#include "colorsandmessageviewstyle.h"
#include "common/authenticationoauthwidget.h"
#include "misc/passwordlineeditwidget.h"
#include "misc/twoauthenticationpasswordwidget.h"
#include "rocketchataccount.h"
#include "ruqola_password_widgets_debug.h"
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

    , mLoginButton(new QPushButton(i18n("Login"), this))
    , mBusyIndicatorWidget(new KBusyIndicatorWidget(this))
    , mFailedError(new QLabel(this))
    , mTwoFactorAuthenticationPasswordLineEdit(new TwoAuthenticationPasswordWidget(this))
    , mTwoFactorAuthenticationWidget(new QWidget(this))
{
    auto mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mServerUrl->setObjectName(QStringLiteral("mServerUrl"));
    mainLayout->addRow(i18n("Server URL:"), mServerUrl);

    mAccountName->setObjectName(QStringLiteral("mAccountName"));
    mainLayout->addRow(i18n("Account Name:"), mAccountName);
    mAccountName->setPlaceholderText(i18n("Account name"));
    connect(mAccountName, &QLineEdit::textChanged, this, &RuqolaLoginWidget::slotUpdateLoginButton);

    mUserName->setObjectName(QStringLiteral("mUserName"));
    mainLayout->addRow(i18n("Username or Email:"), mUserName);
    mUserName->setPlaceholderText(i18n("Username or email"));

    // Password
    mPasswordLineEditWidget->setObjectName(QStringLiteral("mPasswordLineEditWidget"));
    connect(mPasswordLineEditWidget, &PasswordLineEditWidget::returnPressed, this, &RuqolaLoginWidget::slotLogin);
    mainLayout->addRow(i18n("Password:"), mPasswordLineEditWidget);
    connect(mPasswordLineEditWidget, &PasswordLineEditWidget::resetPasswordRequested, this, &RuqolaLoginWidget::slotResetPasswordRequested);

    // Two Factor authentication
    mTwoFactorAuthenticationWidget->setObjectName(QStringLiteral("mTwoFactorAuthenticationWidget"));
    mTwoFactorAuthenticationWidget->setVisible(false);

    auto twoFactorAuthenticationLayout = new QVBoxLayout(mTwoFactorAuthenticationWidget);
    twoFactorAuthenticationLayout->setObjectName(QStringLiteral("twoFactorAuthenticationLayout"));
    twoFactorAuthenticationLayout->setContentsMargins(QMargins{});

    auto twoFactorAuthenticationLabel =
        new QLabel(i18n("You have enabled second factor authentication.\nPlease enter the generated code or a backup code."), this);
    twoFactorAuthenticationLabel->setObjectName(QStringLiteral("twoFactorAuthenticationLabel"));
    twoFactorAuthenticationLayout->addWidget(twoFactorAuthenticationLabel);

    mTwoFactorAuthenticationPasswordLineEdit->setObjectName(QStringLiteral("mTwoFactorAuthenticationPasswordLineEdit"));
    twoFactorAuthenticationLayout->addWidget(mTwoFactorAuthenticationPasswordLineEdit);

    mainLayout->addWidget(mTwoFactorAuthenticationWidget);

    mLoginButton->setObjectName(QStringLiteral("mLoginButton"));
    mainLayout->addWidget(mLoginButton);
    connect(mLoginButton, &QPushButton::clicked, this, &RuqolaLoginWidget::slotLogin);

    mBusyIndicatorWidget->setObjectName(QStringLiteral("mBusyIndicatorWidget"));
    mainLayout->addWidget(mBusyIndicatorWidget);
    // Hide by default
    mBusyIndicatorWidget->hide();

    mFailedError->setObjectName(QStringLiteral("mFailedError"));
    QPalette pal = mFailedError->palette();
    pal.setColor(foregroundRole(), ColorsAndMessageViewStyle::self().schemeView().foreground(KColorScheme::NegativeText).color());
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
    // TODO check authentication method
    mRocketChatAccount = rocketChatAccount;
    mAccountName->setText(mRocketChatAccount->displayName());
    mAccountName->setReadOnly(!mRocketChatAccount->displayName().isEmpty());
    mServerUrl->setText(mRocketChatAccount->serverUrl());
    mServerUrl->setReadOnly(true);

    mUserName->setText(mRocketChatAccount->userName());
    disconnect(mUpdatePasswordConnection);
    mPasswordLineEditWidget->setPassword(mRocketChatAccount->password());
    qCDebug(RUQOLA_PASSWORD_WIDGETS_LOG) << " RuqolaLoginWidget::setRocketChatAccount: password is empty ? " << mRocketChatAccount->password().isEmpty();
    mUpdatePasswordConnection = connect(mRocketChatAccount, &RocketChatAccount::passwordChanged, mPasswordLineEditWidget, [this]() {
        qCDebug(RUQOLA_PASSWORD_WIDGETS_LOG) << " RuqolaLoginWidget::setRocketChatAccount: password has changed => password is empty ? "
                                             << mRocketChatAccount->password().isEmpty();
        mPasswordLineEditWidget->setPassword(mRocketChatAccount->password());
    });

    mTwoFactorAuthenticationPasswordLineEdit->setRocketChatAccount(mRocketChatAccount);
    mPasswordLineEditWidget->setAllowPasswordReset(mRocketChatAccount->allowPasswordReset() && mRocketChatAccount->allowPasswordChange());
}

void RuqolaLoginWidget::slotLogin()
{
    mRocketChatAccount->setAccountName(mAccountName->isEnabled() ? mAccountName->text() : mRocketChatAccount->accountName());
    mRocketChatAccount->setServerUrl(mServerUrl->text());
    mRocketChatAccount->setUserName(mUserName->text());
    mRocketChatAccount->setPassword(mPasswordLineEditWidget->password());
    if (mTwoFactorAuthenticationPasswordLineEdit->isHidden()) {
        mTwoFactorAuthenticationPasswordLineEdit->clear();
    } else {
        mRocketChatAccount->setTwoFactorAuthenticationCode(mTwoFactorAuthenticationPasswordLineEdit->code());
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

void RuqolaLoginWidget::setLoginStatus(AuthenticationManager::LoginStatus status)
{
    mFailedError->setHidden(true);
    switch (status) {
    case AuthenticationManager::LoginStatus::Connecting:
    case AuthenticationManager::LoginStatus::LoginOngoing:
        mBusyIndicatorWidget->show();
        changeWidgetStatus(false);
        mTwoFactorAuthenticationWidget->setVisible(false);
        break;
    case AuthenticationManager::LoginStatus::LoggedOut:
    case AuthenticationManager::LoginStatus::LoggedIn:
        mBusyIndicatorWidget->hide();
        changeWidgetStatus(true);
        mTwoFactorAuthenticationWidget->setVisible(false);
        break;
    case AuthenticationManager::LoginStatus::LoginFailedInvalidUserOrPassword:
        mBusyIndicatorWidget->hide();
        changeWidgetStatus(true);
        showError(i18n("Login Failed: invalid username or password"));
        mTwoFactorAuthenticationWidget->setVisible(false);
        break;
    case AuthenticationManager::LoginFailedInvalidOtp:
        showError(i18n("Login failed: Invalid OTP code."));
        Q_FALLTHROUGH();
    case AuthenticationManager::LoginStatus::LoginOtpRequired:
        mBusyIndicatorWidget->hide();
        changeWidgetStatus(true);
        mTwoFactorAuthenticationWidget->setVisible(true);
        break;
    case AuthenticationManager::LoginStatus::FailedToLoginPluginProblem:
        mBusyIndicatorWidget->hide();
        changeWidgetStatus(true);
        showError(i18n("Installation Problem found. No plugins found here."));
        mTwoFactorAuthenticationWidget->setVisible(false);
        break;
    case AuthenticationManager::GenericError:
        mBusyIndicatorWidget->hide();
        changeWidgetStatus(true);
        showError(i18n("Login Failed: generic error"));
        mTwoFactorAuthenticationWidget->setVisible(false);
        break;
    case AuthenticationManager::LoginFailedLoginBlockForIp:
        mBusyIndicatorWidget->hide();
        changeWidgetStatus(true);
        showError(i18n("Login Failed: Login has been temporarily blocked For IP."));
        mTwoFactorAuthenticationWidget->setVisible(false);
        break;
    case AuthenticationManager::LoginFailedLoginBlockedForUser:
        mBusyIndicatorWidget->hide();
        changeWidgetStatus(true);
        showError(i18n("Login Failed: Login has been temporarily blocked For User."));
        mTwoFactorAuthenticationWidget->setVisible(false);
        break;
    case AuthenticationManager::LoginFailedUserNotActivated:
        mBusyIndicatorWidget->hide();
        changeWidgetStatus(true);
        showError(i18n("Login Failed: User is not activated."));
        mTwoFactorAuthenticationWidget->setVisible(false);
        break;
    case AuthenticationManager::LoginFailedLoginAppNotAllowedToLogin:
        mBusyIndicatorWidget->hide();
        changeWidgetStatus(true);
        showError(i18n("Login Failed: App user is not allowed to login."));
        mTwoFactorAuthenticationWidget->setVisible(false);
        break;
    case AuthenticationManager::LoginOtpAuthOngoing:
    case AuthenticationManager::LogoutOngoing:
    case AuthenticationManager::LogoutCleanUpOngoing:
    case AuthenticationManager::LoggedOutAndCleanedUp:
        // TODO
        mTwoFactorAuthenticationWidget->setVisible(false);
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

#include "moc_ruqolaloginwidget.cpp"
