/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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

#include <QFormLayout>
#include <KLineEdit>

#include <KLocalizedString>

CreateNewAccountWidget::CreateNewAccountWidget(QWidget *parent)
    : QWidget(parent)
{
    auto *mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins(0, 0, 0, 0);

    mAccountName = new KLineEdit(this);
    mAccountName->setObjectName(QStringLiteral("mAccountName"));
    mAccountName->setClearButtonEnabled(true);
    mAccountName->setTrapReturnKey(true);
    mainLayout->addRow(i18n("Account Name:"), mAccountName);

    mServerName = new KLineEdit(this);
    mServerName->setObjectName(QStringLiteral("mServerName"));
    mServerName->setClearButtonEnabled(true);
    mServerName->setTrapReturnKey(true);
    mainLayout->addRow(i18n("Server Name:"), mServerName);

    mUserName = new KLineEdit(this);
    mUserName->setObjectName(QStringLiteral("mUserName"));
    mUserName->setClearButtonEnabled(true);
    mUserName->setTrapReturnKey(true);
    mainLayout->addRow(i18n("User Name:"), mUserName);

    connect(mUserName, &KLineEdit::textChanged, this, &CreateNewAccountWidget::slotChangeOkButtonEnabled);
    connect(mServerName, &KLineEdit::textChanged, this, &CreateNewAccountWidget::slotChangeOkButtonEnabled);
    connect(mAccountName, &KLineEdit::textChanged, this, &CreateNewAccountWidget::slotChangeOkButtonEnabled);

    //TODO add support for two factor ?
}

CreateNewAccountWidget::~CreateNewAccountWidget()
{
}

QString CreateNewAccountWidget::accountName() const
{
    return mAccountName->text().trimmed();
}

QString CreateNewAccountWidget::serverName() const
{
    return mServerName->text().trimmed();
}

void CreateNewAccountWidget::setAccountName(const QString &name)
{
    mAccountName->setText(name);
    mAccountName->setReadOnly(true);
}

void CreateNewAccountWidget::setUserName(const QString &username)
{
    mUserName->setText(username);
}

void CreateNewAccountWidget::setServerName(const QString &servername)
{
    mServerName->setText(servername);
}

void CreateNewAccountWidget::setExistingAccountName(const QStringList &lst)
{
    mNames = lst;
}

QString CreateNewAccountWidget::userName() const
{
    return mUserName->text().trimmed();
}

void CreateNewAccountWidget::slotChangeOkButtonEnabled()
{
    const QString accountName = mAccountName->text().trimmed();
    Q_EMIT updateOkButton(!accountName.isEmpty() && !mNames.contains(accountName)
                          && !mServerName->text().trimmed().isEmpty()
                          && !mUserName->text().trimmed().isEmpty());
}
