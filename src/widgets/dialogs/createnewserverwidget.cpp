/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createnewserverwidget.h"
#include "misc/lineeditcatchreturnkey.h"
#include <KAuthorized>
#include <QFormLayout>
#include <QLineEdit>

#include <KLocalizedString>
#include <KPasswordLineEdit>

CreateNewServerWidget::CreateNewServerWidget(QWidget *parent)
    : QWidget(parent)
    , mAccountName(new QLineEdit(this))
    , mServerName(new QLineEdit(this))
    , mUserName(new QLineEdit(this))
    , mPasswordLineEdit(new KPasswordLineEdit(this))
{
    auto mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mAccountName->setObjectName(QStringLiteral("mAccountName"));
    mAccountName->setClearButtonEnabled(true);
    new LineEditCatchReturnKey(mAccountName, this);
    mainLayout->addRow(i18n("Account Name:"), mAccountName);

    mServerName->setObjectName(QStringLiteral("mServerName"));
    mServerName->setClearButtonEnabled(true);
    new LineEditCatchReturnKey(mServerName, this);
    mainLayout->addRow(i18n("Server Name:"), mServerName);

    mUserName->setObjectName(QStringLiteral("mUserName"));
    mUserName->setClearButtonEnabled(true);
    new LineEditCatchReturnKey(mUserName, this);
    mainLayout->addRow(i18n("User Name:"), mUserName);

    mPasswordLineEdit->setObjectName(QStringLiteral("mPasswordLineEdit"));
    mPasswordLineEdit->setRevealPasswordAvailable(KAuthorized::authorize(QStringLiteral("lineedit_reveal_password")));
    mainLayout->addRow(i18n("Password:"), mPasswordLineEdit);

    connect(mUserName, &QLineEdit::textChanged, this, &CreateNewServerWidget::slotChangeOkButtonEnabled);
    connect(mServerName, &QLineEdit::textChanged, this, &CreateNewServerWidget::slotChangeOkButtonEnabled);
    connect(mAccountName, &QLineEdit::textChanged, this, &CreateNewServerWidget::slotChangeOkButtonEnabled);

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
    mAccountInfo.serverUrl = mServerName->text().trimmed();
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
    mServerName->setText(info.serverUrl);
    mPasswordLineEdit->setPassword(info.password);
}

void CreateNewServerWidget::setExistingAccountName(const QStringList &lst)
{
    mNames = lst;
}

void CreateNewServerWidget::slotChangeOkButtonEnabled()
{
    const QString accountName = mAccountName->text().trimmed();
    Q_EMIT updateOkButton(!accountName.isEmpty() && !mNames.contains(accountName) && !mServerName->text().trimmed().isEmpty()
                          && !mUserName->text().trimmed().isEmpty());
}
