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

#include "createnewaccountwidget.h"
#include "misc/lineeditcatchreturnkey.h"
#include <QFormLayout>
#include <QLineEdit>

#include <KLocalizedString>
#include <KPasswordLineEdit>

CreateNewAccountWidget::CreateNewAccountWidget(QWidget *parent)
    : QWidget(parent)
{
    auto mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mAccountName = new QLineEdit(this);
    mAccountName->setObjectName(QStringLiteral("mAccountName"));
    mAccountName->setClearButtonEnabled(true);
    new LineEditCatchReturnKey(mAccountName, this);
    mainLayout->addRow(i18n("Account Name:"), mAccountName);

    mServerName = new QLineEdit(this);
    mServerName->setObjectName(QStringLiteral("mServerName"));
    mServerName->setClearButtonEnabled(true);
    new LineEditCatchReturnKey(mServerName, this);
    mainLayout->addRow(i18n("Server Name:"), mServerName);

    mUserName = new QLineEdit(this);
    mUserName->setObjectName(QStringLiteral("mUserName"));
    mUserName->setClearButtonEnabled(true);
    new LineEditCatchReturnKey(mUserName, this);
    mainLayout->addRow(i18n("User Name:"), mUserName);

    mPasswordLineEdit = new KPasswordLineEdit(this);
    mPasswordLineEdit->setObjectName(QStringLiteral("mPasswordLineEdit"));
    mainLayout->addRow(i18n("Password:"), mPasswordLineEdit);

    connect(mUserName, &QLineEdit::textChanged, this, &CreateNewAccountWidget::slotChangeOkButtonEnabled);
    connect(mServerName, &QLineEdit::textChanged, this, &CreateNewAccountWidget::slotChangeOkButtonEnabled);
    connect(mAccountName, &QLineEdit::textChanged, this, &CreateNewAccountWidget::slotChangeOkButtonEnabled);

    //TODO add support for two factor ?
}

CreateNewAccountWidget::~CreateNewAccountWidget()
{
}

AccountManager::AccountManagerInfo CreateNewAccountWidget::accountInfo()
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

void CreateNewAccountWidget::setAccountInfo(const AccountManager::AccountManagerInfo &info)
{
    mAccountInfo = info;
    mAccountName->setText(info.displayName.isEmpty() ? info.accountName : info.displayName);
    mUserName->setText(info.userName);
    mServerName->setText(info.serverUrl);
    mPasswordLineEdit->setPassword(info.password);
}

void CreateNewAccountWidget::setExistingAccountName(const QStringList &lst)
{
    mNames = lst;
}

void CreateNewAccountWidget::slotChangeOkButtonEnabled()
{
    const QString accountName = mAccountName->text().trimmed();
    Q_EMIT updateOkButton(!accountName.isEmpty() && !mNames.contains(accountName)
                          && !mServerName->text().trimmed().isEmpty()
                          && !mUserName->text().trimmed().isEmpty());
}
