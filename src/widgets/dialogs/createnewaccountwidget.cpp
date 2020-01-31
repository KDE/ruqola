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
#include <QLineEdit>
#include <QVBoxLayout>

#include <KLocalizedString>

CreateNewAccountWidget::CreateNewAccountWidget(QWidget *parent)
    : QWidget(parent)
{
    QFormLayout *mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins(0, 0, 0, 0);

    mAccountName = new QLineEdit(this);
    mAccountName->setObjectName(QStringLiteral("mAccountName"));
    mAccountName->setClearButtonEnabled(true);
    mainLayout->addRow(i18n("Account Name:"), mAccountName);

    mServerName = new QLineEdit(this);
    mServerName->setObjectName(QStringLiteral("mServerName"));
    mServerName->setClearButtonEnabled(true);
    mainLayout->addRow(i18n("Server Name:"), mServerName);

    mUserName = new QLineEdit(this);
    mUserName->setObjectName(QStringLiteral("mUserName"));
    mUserName->setClearButtonEnabled(true);
    mainLayout->addRow(i18n("User Name:"), mUserName);

    connect(mUserName, &QLineEdit::textChanged, this, &CreateNewAccountWidget::slotChangeOkButtonEnabled);
    connect(mServerName, &QLineEdit::textChanged, this, &CreateNewAccountWidget::slotChangeOkButtonEnabled);
    connect(mAccountName, &QLineEdit::textChanged, this, &CreateNewAccountWidget::slotChangeOkButtonEnabled);

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

QString CreateNewAccountWidget::userName() const
{
    return mUserName->text().trimmed();
}

void CreateNewAccountWidget::slotChangeOkButtonEnabled()
{
    Q_EMIT updateOkButton(!mAccountName->text().trimmed().isEmpty()
                          && !mServerName->text().trimmed().isEmpty()
                          && !mUserName->text().trimmed().isEmpty());
}
