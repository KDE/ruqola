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

#include "registeruserwidget.h"
#include "ruqola.h"
#include "rocketchataccount.h"
#include <KLocalizedString>
#include <KPasswordLineEdit>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>

RegisterUserWidget::RegisterUserWidget(QWidget *parent)
    : QWidget(parent)
{
    auto mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mUserName = new QLineEdit(this);
    mUserName->setObjectName(QStringLiteral("mUserName"));
    mainLayout->addRow(i18n("UserName:"), mUserName);
    connect(mUserName, &QLineEdit::textChanged, this, &RegisterUserWidget::slotUpdateRegisterButton);

    mEmail = new QLineEdit(this);
    mEmail->setObjectName(QStringLiteral("mEmail"));
    mainLayout->addRow(i18n("Email:"), mEmail);
    connect(mEmail, &QLineEdit::textChanged, this, &RegisterUserWidget::slotUpdateRegisterButton);

    mPasswordLineEdit = new KPasswordLineEdit(this);
    mPasswordLineEdit->setObjectName(QStringLiteral("mPasswordLineEdit"));
    mainLayout->addRow(i18n("Password:"), mPasswordLineEdit);
    connect(mPasswordLineEdit, &KPasswordLineEdit::passwordChanged, this, &RegisterUserWidget::slotUpdateRegisterButton);

    mConfirmPasswordLineEdit = new KPasswordLineEdit(this);
    mConfirmPasswordLineEdit->setObjectName(QStringLiteral("mConfirmPasswordLineEdit"));
    mainLayout->addRow(i18n("Confirm Password:"), mConfirmPasswordLineEdit);
    connect(mConfirmPasswordLineEdit, &KPasswordLineEdit::passwordChanged, this, &RegisterUserWidget::slotUpdateRegisterButton);

    mRegisterButton = new QPushButton(i18n("Register"), this);
    mRegisterButton->setObjectName(QStringLiteral("mRegisterButton"));
    connect(mRegisterButton, &QPushButton::clicked, this, &RegisterUserWidget::slotRegisterNewUser);
    mainLayout->addWidget(mRegisterButton);
    mRegisterButton->setEnabled(false);
}

RegisterUserWidget::~RegisterUserWidget()
{
}

void RegisterUserWidget::slotUpdateRegisterButton()
{
    const bool enableRegisterButton
        = !mUserName->text().trimmed().isEmpty()
          && !mEmail->text().trimmed().isEmpty()
          && !mPasswordLineEdit->password().isEmpty()
          && (mPasswordLineEdit->password() == mConfirmPasswordLineEdit->password());
    mRegisterButton->setEnabled(enableRegisterButton);
}

void RegisterUserWidget::slotRegisterNewUser()
{
    Ruqola::self()->rocketChatAccount()->registerNewUser(registerUserInfo());
}

RocketChatRestApi::RegisterUserJob::RegisterUserInfo RegisterUserWidget::registerUserInfo() const
{
    RocketChatRestApi::RegisterUserJob::RegisterUserInfo info;
    info.email = mEmail->text();
    info.name = mUserName->text();
    info.username = mUserName->text().remove(QLatin1Char(' '));
    info.password = mPasswordLineEdit->password();
    return info;
}
