/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ruqolaloginwidget.h"

#include "colorsandmessageviewstyle.h"
#include "common/authenticationoauthwidget.h"
#include "loginwidget/ruqolaloginbutton.h"
#include "misc/passwordlineeditwidget.h"
#include "misc/twoauthenticationpasswordwidget.h"
#include "rocketchataccount.h"
#include "rocketchataccountsettings.h"
#include "ruqolaloginstackwidget.h"
#include <KBusyIndicatorWidget>
#include <KColorScheme>
#include <KLocalizedString>
#include <QCheckBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
RuqolaLoginWidget::RuqolaLoginWidget(QWidget *parent)
    : QWidget(parent)
    , mRuqolaLoginStackWidget(new RuqolaLoginStackWidget(this))
    , mLoginButton(new RuqolaLoginButton(this))
    , mBusyIndicatorWidget(new KBusyIndicatorWidget(this))
    , mFailedError(new QLabel(this))
    , mTwoFactorAuthenticationPasswordLineEdit(new TwoAuthenticationPasswordWidget(this))
    , mTwoFactorAuthenticationWidget(new QWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mRuqolaLoginStackWidget->setObjectName(u"mRuqolaLoginStackWidget"_s);
    mainLayout->addWidget(mRuqolaLoginStackWidget);

    // Two Factor authentication
    mTwoFactorAuthenticationWidget->setObjectName(u"mTwoFactorAuthenticationWidget"_s);
    mTwoFactorAuthenticationWidget->setVisible(false);

    auto twoFactorAuthenticationLayout = new QVBoxLayout(mTwoFactorAuthenticationWidget);
    twoFactorAuthenticationLayout->setObjectName(u"twoFactorAuthenticationLayout"_s);
    twoFactorAuthenticationLayout->setContentsMargins(QMargins{});

    auto twoFactorAuthenticationLabel =
        new QLabel(i18nc("@label:textbox", "You have enabled second factor authentication.\nPlease enter the generated code or a backup code."), this);
    twoFactorAuthenticationLabel->setObjectName(u"twoFactorAuthenticationLabel"_s);
    twoFactorAuthenticationLayout->addWidget(twoFactorAuthenticationLabel);

    mTwoFactorAuthenticationPasswordLineEdit->setObjectName(u"mTwoFactorAuthenticationPasswordLineEdit"_s);
    twoFactorAuthenticationLayout->addWidget(mTwoFactorAuthenticationPasswordLineEdit);

    mainLayout->addWidget(mTwoFactorAuthenticationWidget);

    mLoginButton->setObjectName(u"mLoginButton"_s);
    mLoginButton->setLoginInProgress(false);
    connect(mLoginButton, &RuqolaLoginButton::loginRequested, this, &RuqolaLoginWidget::slotLogin);
    connect(mLoginButton, &RuqolaLoginButton::cancelLoginRequested, this, &RuqolaLoginWidget::slotCancelLogin);
    connect(mRuqolaLoginStackWidget, &RuqolaLoginStackWidget::tryLogin, this, &RuqolaLoginWidget::slotLogin);
    auto loginButtonLayout = new QHBoxLayout;
    loginButtonLayout->setContentsMargins({});
    loginButtonLayout->addStretch(0);
    loginButtonLayout->addWidget(mLoginButton);
    loginButtonLayout->addStretch(0);

    mainLayout->addLayout(loginButtonLayout);

    mBusyIndicatorWidget->setObjectName(u"mBusyIndicatorWidget"_s);
    mainLayout->addWidget(mBusyIndicatorWidget);
    // Hide by default
    mBusyIndicatorWidget->hide();

    mFailedError->setObjectName(u"mFailedError"_s);
    QPalette pal = mFailedError->palette();
    pal.setColor(foregroundRole(), ColorsAndMessageViewStyle::self().schemeView().foreground(KColorScheme::NegativeText).color());
    mFailedError->setPalette(pal);
    QFont font = mFailedError->font();
    font.setBold(true);
    mFailedError->setFont(font);

    mainLayout->addWidget(mFailedError);
    mainLayout->addStretch(2);
    // Hide by default
    mFailedError->hide();
    mLoginButton->setAutoDefault(true);
    mLoginButton->setEnabled(false);
    connect(mRuqolaLoginStackWidget, &RuqolaLoginStackWidget::settingsIsValid, mLoginButton, &QPushButton::setEnabled);
}

RuqolaLoginWidget::~RuqolaLoginWidget() = default;

void RuqolaLoginWidget::forceLoginFocus()
{
    mLoginButton->setFocus();
}

void RuqolaLoginWidget::setRocketChatAccount(RocketChatAccount *rocketChatAccount)
{
    // TODO check authentication method
    mRocketChatAccount = rocketChatAccount;

    AccountManager::AccountManagerInfo info;
    info.accountName = mRocketChatAccount->settings()->displayName();
    info.serverUrl = mRocketChatAccount->serverUrl();
    info.userName = mRocketChatAccount->userName();
    info.password = mRocketChatAccount->settings()->password();
    info.canRegisterAccount = mRocketChatAccount->ruqolaServerConfig()->registrationFormEnabled();
    info.canResetPassword = (mRocketChatAccount->ruqolaServerConfig()->allowPasswordReset() && mRocketChatAccount->ruqolaServerConfig()->allowPasswordChange());
    info.authMethodType = mRocketChatAccount->settings()->authMethodType();
    info.authenticationInfos = mRocketChatAccount->authenticationMethodInfos();
    // qDebug() << " info " << info;
    mRuqolaLoginStackWidget->setAccountInfo(std::move(info));

#if 0
    disconnect(mUpdatePasswordConnection);
    mPasswordLineEditWidget->setPassword(mRocketChatAccount->password());
    qCDebug(RUQOLA_PASSWORD_WIDGETS_LOG) << " RuqolaLoginWidget::setRocketChatAccount: password is empty ? " << mRocketChatAccount->password().isEmpty();
    mUpdatePasswordConnection = connect(mRocketChatAccount, &RocketChatAccount::passwordChanged, mPasswordLineEditWidget, [this]() {
        qCDebug(RUQOLA_PASSWORD_WIDGETS_LOG) << " RuqolaLoginWidget::setRocketChatAccount: password has changed => password is empty ? "
                                             << mRocketChatAccount->password().isEmpty();
        mPasswordLineEditWidget->setPassword(mRocketChatAccount->password());
    });
#endif
    mTwoFactorAuthenticationPasswordLineEdit->setRocketChatAccount(mRocketChatAccount);
}

void RuqolaLoginWidget::slotCancelLogin()
{
}

void RuqolaLoginWidget::slotLogin()
{
    const AccountManager::AccountManagerInfo info = mRuqolaLoginStackWidget->accountInfo();
    mRocketChatAccount->settings()->setUserName(info.userName);
    mRocketChatAccount->settings()->setPassword(info.password);
    if (mTwoFactorAuthenticationPasswordLineEdit->isHidden()) {
        mTwoFactorAuthenticationPasswordLineEdit->clear();
    } else {
        mRocketChatAccount->settings()->setTwoFactorAuthenticationCode(mTwoFactorAuthenticationPasswordLineEdit->code());
    }
    mRocketChatAccount->tryLogin();
}

void RuqolaLoginWidget::changeWidgetStatus(bool enabled)
{
    mRuqolaLoginStackWidget->changeAuthenticationWidgetStatus(enabled);
    mLoginButton->setEnabled(enabled);
}

void RuqolaLoginWidget::setLoginStatus(AuthenticationManager::LoginStatus status)
{
    mFailedError->setHidden(true);
    switch (status) {
    case AuthenticationManager::LoginStatus::Connecting:
    case AuthenticationManager::LoginStatus::LoginOngoing:
        mBusyIndicatorWidget->show();
        mLoginButton->setLoginInProgress(true);
        changeWidgetStatus(false);
        mTwoFactorAuthenticationWidget->setVisible(false);
        break;
    case AuthenticationManager::LoginStatus::LoggedOut:
    case AuthenticationManager::LoginStatus::LoggedIn:
        mBusyIndicatorWidget->hide();
        mLoginButton->setLoginInProgress(false);
        changeWidgetStatus(true);
        mTwoFactorAuthenticationWidget->setVisible(false);
        break;
    case AuthenticationManager::LoginStatus::LoginFailedInvalidUserOrPassword:
        mBusyIndicatorWidget->hide();
        mLoginButton->setLoginInProgress(false);
        changeWidgetStatus(true);
        showError(i18n("Login Failed: invalid username or password"));
        mTwoFactorAuthenticationWidget->setVisible(false);
        break;
    case AuthenticationManager::LoginFailedInvalidOtp:
        showError(i18n("Login failed: Invalid OTP code."));
        Q_FALLTHROUGH();
    case AuthenticationManager::LoginStatus::LoginOtpRequired:
        mBusyIndicatorWidget->hide();
        mLoginButton->setLoginInProgress(false);
        changeWidgetStatus(true);
        mTwoFactorAuthenticationWidget->setVisible(true);
        break;
    case AuthenticationManager::LoginStatus::FailedToLoginPluginProblem:
        mBusyIndicatorWidget->hide();
        mLoginButton->setLoginInProgress(false);
        changeWidgetStatus(true);
        showError(i18n("Installation Problem found. No plugins found here."));
        mTwoFactorAuthenticationWidget->setVisible(false);
        break;
    case AuthenticationManager::GenericError:
        mBusyIndicatorWidget->hide();
        mLoginButton->setLoginInProgress(false);
        changeWidgetStatus(true);
        showError(i18n("Login Failed: generic error"));
        mTwoFactorAuthenticationWidget->setVisible(false);
        break;
    case AuthenticationManager::LoginFailedLoginBlockForIp:
        mBusyIndicatorWidget->hide();
        mLoginButton->setLoginInProgress(false);
        changeWidgetStatus(true);
        showError(i18n("Login Failed: Login has been temporarily blocked For IP."));
        mTwoFactorAuthenticationWidget->setVisible(false);
        break;
    case AuthenticationManager::LoginFailedLoginBlockedForUser:
        mBusyIndicatorWidget->hide();
        mLoginButton->setLoginInProgress(false);
        changeWidgetStatus(true);
        showError(i18n("Login Failed: Login has been temporarily blocked For User."));
        mTwoFactorAuthenticationWidget->setVisible(false);
        break;
    case AuthenticationManager::LoginFailedUserNotActivated:
        mBusyIndicatorWidget->hide();
        mLoginButton->setLoginInProgress(false);
        changeWidgetStatus(true);
        showError(i18n("Login Failed: User is not activated."));
        mTwoFactorAuthenticationWidget->setVisible(false);
        break;
    case AuthenticationManager::LoginFailedLoginAppNotAllowedToLogin:
        mBusyIndicatorWidget->hide();
        mLoginButton->setLoginInProgress(false);
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
        mLoginButton->setLoginInProgress(false);
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

#include "moc_ruqolaloginwidget.cpp"
