/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "authenticationloginwidget.h"
#include "connection.h"
#include "misc/passwordlineeditwidget.h"
#include "registeruser/registeruserdialog.h"
#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <KMessageBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QPointer>
#include <QPushButton>

AuthenticationLoginWidget::AuthenticationLoginWidget(QWidget *parent)
    : PluginAuthenticationConfigureWidget{parent}
    , mServerUrl(new QLineEdit(this))
    , mAccountName(new QLineEdit(this))
    , mUserName(new QLineEdit(this))
    , mPasswordLineEditWidget(new PasswordLineEditWidget(this))
    , mRegisterAccount(new QPushButton(i18nc("@action:button", "Register Account"), this))
{
    auto mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mServerUrl->setObjectName(QStringLiteral("mServerUrl"));
    mServerUrl->setReadOnly(true);
    mainLayout->addRow(i18n("Server URL:"), mServerUrl);

    mAccountName->setObjectName(QStringLiteral("mAccountName"));
    mAccountName->setClearButtonEnabled(true);
    mAccountName->setPlaceholderText(i18n("My Account"));
    KLineEditEventHandler::catchReturnKey(mAccountName);
    mainLayout->addRow(i18n("Account name:"), mAccountName);

    mUserName->setObjectName(QStringLiteral("mUserName"));
    mUserName->setClearButtonEnabled(true);
    mUserName->setPlaceholderText(i18nc("@info:placeholder", "Username or email"));
    KLineEditEventHandler::catchReturnKey(mUserName);
    mainLayout->addRow(i18n("Username or email:"), mUserName);

    mPasswordLineEditWidget->setObjectName(QStringLiteral("mPasswordLineEdit"));
    mainLayout->addRow(i18n("Password:"), mPasswordLineEditWidget);

    mRegisterAccount->setObjectName(QStringLiteral("mRegisterAccount"));
    mainLayout->addRow(QString(), mRegisterAccount);
    connect(mRegisterAccount, &QPushButton::clicked, this, &AuthenticationLoginWidget::slotRegisterAccount);

    connect(mUserName, &QLineEdit::textChanged, this, &AuthenticationLoginWidget::slotLoginSettingsChanged);
    connect(mServerUrl, &QLineEdit::textChanged, this, &AuthenticationLoginWidget::slotLoginSettingsChanged);
    connect(mAccountName, &QLineEdit::textChanged, this, &AuthenticationLoginWidget::slotLoginSettingsChanged);
    connect(mAccountName, &QLineEdit::textChanged, this, &AuthenticationLoginWidget::slotLoginSettingsChanged);
    connect(mPasswordLineEditWidget, &PasswordLineEditWidget::tryLogin, this, &AuthenticationLoginWidget::tryLogin);
    connect(mPasswordLineEditWidget, &PasswordLineEditWidget::resetPasswordRequested, this, &AuthenticationLoginWidget::slotResetPasswordRequested);
}

AuthenticationLoginWidget::~AuthenticationLoginWidget() = default;

void AuthenticationLoginWidget::setAuthenticationLoginType(AuthenticationLoginType type)
{
    switch (type) {
    case AuthenticationLoginType::Unknown:
        break;
    case AuthenticationLoginType::Create:
        mAccountName->setReadOnly(false);
        mServerUrl->setReadOnly(false);
        break;
    case AuthenticationLoginType::Modify:
        mAccountName->setReadOnly(false);
        break;
    case AuthenticationLoginType::Login:
        mAccountName->setReadOnly(true);
        mAccountName->setClearButtonEnabled(false);
        break;
    }
}

void AuthenticationLoginWidget::changeAuthenticationWidgetStatus(bool enabled)
{
    mServerUrl->setEnabled(enabled);
    mUserName->setEnabled(enabled);
    mPasswordLineEditWidget->setEnabled(enabled);
}

void AuthenticationLoginWidget::slotResetPasswordRequested(const QString &email)
{
    auto restApi = new Connection(this);
    restApi->setServerUrl(mAccountInfo.serverUrl);
    restApi->forgotPassword(email);
    connect(restApi, &Connection::forgotPasswordDone, this, [restApi]() {
        restApi->deleteLater();
    });
}

void AuthenticationLoginWidget::slotLoginSettingsChanged()
{
    const QString accountName = mAccountName->text().trimmed();
    Q_EMIT settingsIsValid(!accountName.isEmpty() && !mExistingAccountNames.contains(accountName) && !mServerUrl->text().trimmed().isEmpty()
                           && !mUserName->text().trimmed().isEmpty());
}

AccountManager::AccountManagerInfo AuthenticationLoginWidget::accountInfo()
{
    const QString accountName = mAccountName->text().trimmed();
    if (mAccountInfo.accountName.isEmpty()) {
        mAccountInfo.accountName = accountName;
    }
    mAccountInfo.displayName = accountName;
    mAccountInfo.serverUrl = mServerUrl->text().trimmed();
    if (mAccountInfo.serverUrl.endsWith(QLatin1Char('/'))) {
        mAccountInfo.serverUrl.chop(1);
    }
    mAccountInfo.userName = mUserName->text().trimmed();
    mAccountInfo.password = mPasswordLineEditWidget->password();
    mAccountInfo.authMethodType = AuthenticationManager::AuthMethodType::Password;
    return mAccountInfo;
}

void AuthenticationLoginWidget::setAccountInfo(const AccountManager::AccountManagerInfo &info)
{
    mAccountInfo = info;
    mAccountName->setText(info.displayName.isEmpty() ? info.accountName : info.displayName);
    mUserName->setText(info.userName);
    mServerUrl->setText(info.serverUrl);
    mPasswordLineEditWidget->setPassword(info.password);
    mPasswordLineEditWidget->setAllowPasswordReset(info.canResetPassword);
    mRegisterAccount->setVisible(info.canRegisterAccount);
    if (mAccountName->text().isEmpty()) {
        mAccountName->setReadOnly(false);
    }
}

void AuthenticationLoginWidget::slotRegisterAccount()
{
    QPointer<RegisterUserDialog> dlg = new RegisterUserDialog(this);
    dlg->setManuallyApproveNewUsersRequired(mAccountInfo.accountsManuallyApproveNewUsers);
    dlg->setPasswordValidChecks(mAccountInfo.passwordSettings);
    connect(dlg, &RegisterUserDialog::registerNewAccount, this, [this, dlg]() {
        auto restApi = new Connection(this);
        connect(restApi, &Connection::registerUserDone, this, [this, restApi]() {
            restApi->deleteLater();
            slotRegisterUserDone();
        });
        restApi->setServerUrl(mAccountInfo.serverUrl);
        restApi->registerNewUser(dlg->registerUserInfo());
    });
    dlg->exec();
    delete dlg;
}

void AuthenticationLoginWidget::slotRegisterUserDone()
{
    KMessageBox::information(
        this,
        i18n("We have sent you an email to confirm your registration.\nIf you do not receive an email shortly, please come back and try again."),
        i18nc("@title:window", "Register New User"));
}

#include "moc_authenticationloginwidget.cpp"
