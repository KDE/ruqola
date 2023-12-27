/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "authenticationloginwidget.h"
#include "misc/lineeditcatchreturnkey.h"
#include "misc/passwordlineeditwidget.h"
#include <KLocalizedString>
#include <KPasswordLineEdit>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>

AuthenticationLoginWidget::AuthenticationLoginWidget(QWidget *parent)
    : QWidget{parent}
    , mServerUrl(new QLineEdit(this))
    , mAccountName(new QLineEdit(this))
    , mUserName(new QLineEdit(this))
    , mPasswordLineEditWidget(new PasswordLineEditWidget(this))
    , mRegisterAccount(new QPushButton(i18n("Register Account"), this))
{
    auto mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mServerUrl->setObjectName(QStringLiteral("mServerUrl"));
    mServerUrl->setReadOnly(true);
    mainLayout->addRow(i18n("Server URL:"), mServerUrl);

    mAccountName->setObjectName(QStringLiteral("mAccountName"));
    mAccountName->setClearButtonEnabled(true);
    new LineEditCatchReturnKey(mAccountName, this);
    mainLayout->addRow(i18n("Account name:"), mAccountName);

    mUserName->setObjectName(QStringLiteral("mUserName"));
    mUserName->setClearButtonEnabled(true);
    new LineEditCatchReturnKey(mUserName, this);
    mainLayout->addRow(i18n("Username:"), mUserName);

    mPasswordLineEditWidget->setObjectName(QStringLiteral("mPasswordLineEdit"));
    mainLayout->addRow(i18n("Password:"), mPasswordLineEditWidget);

    mRegisterAccount->setObjectName(QStringLiteral("mRegisterAccount"));
    mainLayout->addRow(QString(), mRegisterAccount);
    connect(mRegisterAccount, &QPushButton::clicked, this, &AuthenticationLoginWidget::slotRegisterAccount);

    connect(mUserName, &QLineEdit::textChanged, this, &AuthenticationLoginWidget::slotChangeOkButtonEnabled);
    connect(mServerUrl, &QLineEdit::textChanged, this, &AuthenticationLoginWidget::slotChangeOkButtonEnabled);
    connect(mAccountName, &QLineEdit::textChanged, this, &AuthenticationLoginWidget::slotChangeOkButtonEnabled);
    // TODO add "forgot password"
}

void AuthenticationLoginWidget::setExistingAccountName(const QStringList &lst)
{
    mNames = lst;
}

AuthenticationLoginWidget::~AuthenticationLoginWidget() = default;

void AuthenticationLoginWidget::slotChangeOkButtonEnabled()
{
    const QString accountName = mAccountName->text().trimmed();
    Q_EMIT updateOkButton(!accountName.isEmpty() && !mNames.contains(accountName) && !mServerUrl->text().trimmed().isEmpty()
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
    mAccountInfo.password = mPasswordLineEditWidget->passwordLineEdit()->password();
    return mAccountInfo;
}

void AuthenticationLoginWidget::setAccountInfo(const AccountManager::AccountManagerInfo &info)
{
    mAccountInfo = info;
    mAccountName->setText(info.displayName.isEmpty() ? info.accountName : info.displayName);
    mUserName->setText(info.userName);
    mServerUrl->setText(info.serverUrl);
    mPasswordLineEditWidget->passwordLineEdit()->setPassword(info.password);
    mPasswordLineEditWidget->setAllowPasswordReset(info.canResetPassword);
    mRegisterAccount->setVisible(info.canRegisterAccount);
}

void AuthenticationLoginWidget::slotRegisterAccount()
{
    // TODO
}

#include "moc_authenticationloginwidget.cpp"
