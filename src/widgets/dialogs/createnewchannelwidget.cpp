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

#include "createnewchannelwidget.h"
#include "channelnamevalidlineedit.h"
#include "misc/adduserswidget.h"
#include <KAuthorized>
#include <KLocalizedString>
#include <KPasswordLineEdit>
#include <QCheckBox>
#include <QFormLayout>
#include <QLineEdit>

CreateNewChannelWidget::CreateNewChannelWidget(QWidget *parent)
    : QWidget(parent)
    , mChannelName(new ChannelNameValidLineEdit(this))
    , mUsers(new AddUsersWidget(this))
    , mReadOnly(new QCheckBox(this))
    , mBroadcast(new QCheckBox(this))
    , mPrivate(new QCheckBox(this))
    , mEncryptedRoom(new QCheckBox(this))
    , mPasswordLineEdit(new KPasswordLineEdit(this))
    , mTopicLineEdit(new QLineEdit(this))
    , mMainLayout(new QFormLayout(this))
{
    mMainLayout->setObjectName(QStringLiteral("mainLayout"));
    mMainLayout->setContentsMargins({});

    mChannelName->setObjectName(QStringLiteral("mChannelName"));
    mMainLayout->addRow(i18n("Name:"), mChannelName);

    mTopicLineEdit->setObjectName(QStringLiteral("mTopicLineEdit"));
    mMainLayout->addRow(i18n("Topic:"), mTopicLineEdit);

    mUsers->setObjectName(QStringLiteral("mUsers"));
    mUsers->setPlaceholderText(i18n("Invite Users..."));
    mMainLayout->addRow(i18n("Users:"), mUsers);

    mReadOnly->setObjectName(QStringLiteral("mReadOnly"));
    mReadOnly->setChecked(false);
    mMainLayout->addRow(i18n("Read-Only:"), mReadOnly);

    mBroadcast->setObjectName(QStringLiteral("mBroadcast"));
    mBroadcast->setChecked(false);
    mMainLayout->addRow(i18n("Broadcast:"), mBroadcast);

    mPrivate->setObjectName(QStringLiteral("mPrivate"));
    mPrivate->setChecked(false);
    mMainLayout->addRow(i18n("Private Room:"), mPrivate);

    mEncryptedRoom->setObjectName(QStringLiteral("mEncryptedRoom"));
    mEncryptedRoom->setChecked(false);
    mMainLayout->addRow(i18n("Encrypted Room:"), mEncryptedRoom);

    mPasswordLineEdit->setObjectName(QStringLiteral("mPasswordLineEdit"));
    mPasswordLineEdit->setRevealPasswordAvailable(KAuthorized::authorize(QStringLiteral("lineedit_reveal_password")));
    mMainLayout->addRow(i18n("Password:"), mPasswordLineEdit);

    connect(mChannelName, &ChannelNameValidLineEdit::channelIsValid, this, &CreateNewChannelWidget::slotChangeOkButtonEnabled);
}

CreateNewChannelWidget::~CreateNewChannelWidget()
{
}

void CreateNewChannelWidget::slotChangeOkButtonEnabled(bool state)
{
    Q_EMIT updateOkButton(/*!mChannelName->text().trimmed().isEmpty()*/ state);
}

QString CreateNewChannelWidget::channelName() const
{
    return mChannelName->text();
}

QStringList CreateNewChannelWidget::users() const
{
    return mUsers->userIds();
}

bool CreateNewChannelWidget::readOnly() const
{
    return mReadOnly->isChecked();
}

bool CreateNewChannelWidget::broadCast() const
{
    return mBroadcast->isChecked();
}

bool CreateNewChannelWidget::privateChannel() const
{
    return mPrivate->isChecked();
}

bool CreateNewChannelWidget::encryptedRoom() const
{
    return mEncryptedRoom->isChecked();
}

QString CreateNewChannelWidget::password() const
{
    return mPasswordLineEdit->password();
}

QString CreateNewChannelWidget::topic() const
{
    return mTopicLineEdit->text();
}

void CreateNewChannelWidget::setFeatures(CreateNewChannelWidget::Features features)
{
    bool visible = features & CreateNewChannelWidget::Feature::BroadCast;

    mBroadcast->setVisible(visible);
    mMainLayout->labelForField(mBroadcast)->setVisible(visible);

    visible = features & CreateNewChannelWidget::Feature::Encrypted;

    mEncryptedRoom->setVisible(visible);
    mMainLayout->labelForField(mEncryptedRoom)->setVisible(visible);
}
