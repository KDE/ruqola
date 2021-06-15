/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

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
#include "administratoradduserwidget.h"
#include "rolescombobox.h"
#include "ruqolawidgets_debug.h"
#include "user.h"
#include <KLocalizedString>
#include <KPasswordLineEdit>
#include <QCheckBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>

AdministratorAddUserWidget::AdministratorAddUserWidget(QWidget *parent)
    : QWidget(parent)
    , mName(new QLineEdit(this))
    , mUserName(new QLineEdit(this))
    , mEmail(new QLineEdit(this))
    , mJoinDefaultChannels(new QCheckBox(i18n("Join Default Channels"), this))
    , mSendWelcomeEmails(new QCheckBox(i18n("Send Welcome Email"), this))
    , mPasswordLineEdit(new KPasswordLineEdit(this))
    , mRolesComboBox(new RolesComboBox(this))
{
    auto formLayout = new QFormLayout(this);
    formLayout->setObjectName(QStringLiteral("formLayout"));
    mName->setObjectName(QStringLiteral("mName"));
    mUserName->setObjectName(QStringLiteral("mUserName"));
    mEmail->setObjectName(QStringLiteral("mEmail"));
    mJoinDefaultChannels->setObjectName(QStringLiteral("mJoinDefaultChannels"));
    mSendWelcomeEmails->setObjectName(QStringLiteral("mSendWelcomeEmails"));
    mPasswordLineEdit->setObjectName(QStringLiteral("mPasswordLineEdit"));
    mRolesComboBox->setObjectName(QStringLiteral("mRolesComboBox"));
    formLayout->addRow(i18n("Name"), mName);
    formLayout->addRow(i18n("Username"), mUserName);
    formLayout->addRow(i18n("Email"), mEmail);
    formLayout->addRow(i18n("Password"), mPasswordLineEdit);
    formLayout->addWidget(mJoinDefaultChannels);
    formLayout->addWidget(mSendWelcomeEmails);
    formLayout->addRow(i18n("Roles"), mRolesComboBox);
    mRolesComboBox->initialize();
    connect(mName, &QLineEdit::textChanged, this, &AdministratorAddUserWidget::slotUpdateOkButton);
    connect(mUserName, &QLineEdit::textChanged, this, &AdministratorAddUserWidget::slotUpdateOkButton);
    connect(mEmail, &QLineEdit::textChanged, this, &AdministratorAddUserWidget::slotUpdateOkButton);
    connect(mPasswordLineEdit, &KPasswordLineEdit::passwordChanged, this, &AdministratorAddUserWidget::slotUpdateOkButton);
}

AdministratorAddUserWidget::~AdministratorAddUserWidget()
{
}

void AdministratorAddUserWidget::slotUpdateOkButton()
{
    const bool enableOkButton = !mName->text().trimmed().isEmpty() && !mUserName->text().trimmed().isEmpty() && !mEmail->text().trimmed().isEmpty()
        && !mPasswordLineEdit->password().isEmpty();
    Q_EMIT updateButtonOk(enableOkButton);
}

RocketChatRestApi::UpdateUserInfo AdministratorAddUserWidget::updateInfo() const
{
    RocketChatRestApi::UpdateUserInfo info;
    info.mName = mName->text().trimmed();
    info.mEmail = mEmail->text().trimmed();
    info.mUserName = mUserName->text();
    info.mSendWelcomeEmail = mSendWelcomeEmails->isChecked();
    info.mJoinDefaultChannels = mJoinDefaultChannels->isChecked();
    info.mPassword = mPasswordLineEdit->password();
    // TODO add more
    return info;
}

RocketChatRestApi::CreateUpdateUserInfo AdministratorAddUserWidget::createInfo() const
{
    RocketChatRestApi::CreateUpdateUserInfo info;
    info.mName = mName->text().trimmed();
    info.mEmail = mEmail->text().trimmed();
    info.mUserName = mUserName->text();
    info.mSendWelcomeEmail = mSendWelcomeEmails->isChecked();
    info.mJoinDefaultChannels = mJoinDefaultChannels->isChecked();
    info.mPassword = mPasswordLineEdit->password();
    return info;
}

void AdministratorAddUserWidget::setUser(const User &user)
{
    mName->setText(user.name());
    mUserName->setText(user.userName());
    mEmail->setText(user.userEmailsInfo().email);
    mRolesComboBox->setRoles(user.roles());
    // mJoinDefaultChannels->setChecked(user.jo)

    // TODO
}
