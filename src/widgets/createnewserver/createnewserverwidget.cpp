/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createnewserverwidget.h"
#include "common/authenticationwidget.h"
#include "misc/lineeditcatchreturnkey.h"
#include <KAuthorized>
#include <QFormLayout>
#include <QLineEdit>

#include <KLocalizedString>
#include <KPasswordLineEdit>

CreateNewServerWidget::CreateNewServerWidget(QWidget *parent)
    : QWidget(parent)
    , mAccountName(new QLineEdit(this))
    , mServerUrl(new QLineEdit(this))
    , mUserName(new QLineEdit(this))
    , mPasswordLineEdit(new KPasswordLineEdit(this))
    , mAuthenticationWidget(new AuthenticationWidget(this))
{
    auto mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mServerUrl->setObjectName(QStringLiteral("mServerUrl"));
    mServerUrl->setClearButtonEnabled(true);
    new LineEditCatchReturnKey(mServerUrl, this);
    mainLayout->addRow(i18n("Server URL:"), mServerUrl);

    mAccountName->setObjectName(QStringLiteral("mAccountName"));
    mAccountName->setClearButtonEnabled(true);
    new LineEditCatchReturnKey(mAccountName, this);
    mainLayout->addRow(i18n("Account name:"), mAccountName);

    mUserName->setObjectName(QStringLiteral("mUserName"));
    mUserName->setClearButtonEnabled(true);
    new LineEditCatchReturnKey(mUserName, this);
    mainLayout->addRow(i18n("Username:"), mUserName);

    mPasswordLineEdit->setObjectName(QStringLiteral("mPasswordLineEdit"));
    mPasswordLineEdit->setRevealPasswordAvailable(KAuthorized::authorize(QStringLiteral("lineedit_reveal_password")));
    mainLayout->addRow(i18n("Password:"), mPasswordLineEdit);

    connect(mUserName, &QLineEdit::textChanged, this, &CreateNewServerWidget::slotChangeOkButtonEnabled);
    connect(mServerUrl, &QLineEdit::textChanged, this, &CreateNewServerWidget::slotChangeOkButtonEnabled);
    connect(mAccountName, &QLineEdit::textChanged, this, &CreateNewServerWidget::slotChangeOkButtonEnabled);

    mAuthenticationWidget->setObjectName(QStringLiteral("mAuthenticationWidget"));
    mainLayout->addWidget(mAuthenticationWidget);

    // TODO add support for two factor ?
}

CreateNewServerWidget::~CreateNewServerWidget() = default;

AccountManager::AccountManagerInfo CreateNewServerWidget::accountInfo()
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
    mAccountInfo.password = mPasswordLineEdit->password();
    return mAccountInfo;
}

void CreateNewServerWidget::setAccountInfo(const AccountManager::AccountManagerInfo &info)
{
    mAccountInfo = info;
    mAccountName->setText(info.displayName.isEmpty() ? info.accountName : info.displayName);
    mUserName->setText(info.userName);
    mServerUrl->setText(info.serverUrl);
    mPasswordLineEdit->setPassword(info.password);
}

void CreateNewServerWidget::setExistingAccountName(const QStringList &lst)
{
    mNames = lst;
}

void CreateNewServerWidget::slotChangeOkButtonEnabled()
{
    const QString accountName = mAccountName->text().trimmed();
    Q_EMIT updateOkButton(!accountName.isEmpty() && !mNames.contains(accountName) && !mServerUrl->text().trimmed().isEmpty()
                          && !mUserName->text().trimmed().isEmpty());
}

#include "moc_createnewserverwidget.cpp"
