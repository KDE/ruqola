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

#include "myaccountprofileconfigurewidget.h"
#include "misc/lineeditcatchreturnkey.h"
#include "ruqola.h"
#include "rocketchataccount.h"
#include <KLocalizedString>
#include <QFormLayout>
#include <QLineEdit>

MyAccountProfileConfigureWidget::MyAccountProfileConfigureWidget(QWidget *parent)
    : QWidget(parent)
{
    QFormLayout *mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mName = new QLineEdit(this);
    mName->setObjectName(QStringLiteral("mName"));
    new LineEditCatchReturnKey(mName, this);
    mainLayout->addRow(i18n("Name:"), mName);

    mUserName = new QLineEdit(this);
    mUserName->setObjectName(QStringLiteral("mUserName"));
    new LineEditCatchReturnKey(mUserName, this);
    mainLayout->addRow(i18n("UserName:"), mUserName);

    mEmail = new QLineEdit(this);
    mEmail->setObjectName(QStringLiteral("mEmail"));
    new LineEditCatchReturnKey(mEmail, this);
    mainLayout->addRow(i18n("Email:"), mEmail);

    mNickName = new QLineEdit(this);
    mNickName->setObjectName(QStringLiteral("mNickName"));
    new LineEditCatchReturnKey(mNickName, this);
    mainLayout->addRow(i18n("NickName:"), mNickName);

    mStatusText = new QLineEdit(this);
    new LineEditCatchReturnKey(mStatusText, this);
    mStatusText->setObjectName(QStringLiteral("mStatusText"));
    mainLayout->addRow(i18n("Status Text:"), mStatusText);

    //TODO add password.
    init();
}

MyAccountProfileConfigureWidget::~MyAccountProfileConfigureWidget()
{
}

void MyAccountProfileConfigureWidget::init()
{

    mUserName->setReadOnly(!Ruqola::self()->rocketChatAccount()->allowUsernameChange());
    mEmail->setReadOnly(!Ruqola::self()->rocketChatAccount()->allowEmailChange());
    //TODO !Ruqola::rocketChatAccount()->allowPasswordChange();
}

void MyAccountProfileConfigureWidget::load()
{
    mOwnUser = Ruqola::self()->rocketChatAccount()->ownUser();
    mEmail->setText(mOwnUser.email());
    mName->setText(mOwnUser.name());
    mUserName->setText(mOwnUser.userName());
    mNickName->setText(mOwnUser.nickName());
    mStatusText->setText(mOwnUser.statusText());
}

void MyAccountProfileConfigureWidget::save()
{
    RocketChatRestApi::UsersUpdateOwnBasicInfoJob::UpdateOwnBasicInfo updateInfo;
    if (!mEmail->isReadOnly() && (mOwnUser.email() != mEmail->text())) {
        updateInfo.email = mEmail->text();
    }
    if (!mNickName->isReadOnly() && (mOwnUser.nickName() != mNickName->text())) {
        updateInfo.nickName = mNickName->text();
    }
    if (!mUserName->isReadOnly() && (mOwnUser.userName() != mUserName->text())) {
        updateInfo.userName = mUserName->text();
    }
    if (!mStatusText->isReadOnly() && (mOwnUser.statusText() != mStatusText->text())) {
        updateInfo.statusText = mStatusText->text();
    }
    if (!mName->isReadOnly() && (mOwnUser.name() != mName->text())) {
        updateInfo.name = mName->text();
    }
    //TODO add more.
    if (updateInfo.isValid()) {
        Ruqola::self()->rocketChatAccount()->updateOwnBasicInfo(updateInfo);
    }
}
