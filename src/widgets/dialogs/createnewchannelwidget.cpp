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

#include "createnewchannelwidget.h"

#include <QFormLayout>
#include <QLineEdit>
#include <KLocalizedString>
#include <QCheckBox>
#include <KPasswordLineEdit>

CreateNewChannelWidget::CreateNewChannelWidget(QWidget *parent)
    : QWidget(parent)
{
    QFormLayout *mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins(0, 0, 0, 0);

    mChannelName = new QLineEdit(this);
    mChannelName->setObjectName(QStringLiteral("mChannelName"));
    mainLayout->addRow(i18n("Name:"), mChannelName);

    mUsers = new QLineEdit(this);
    mUsers->setObjectName(QStringLiteral("mUsers"));
    mUsers->setPlaceholderText(i18nc("List of users separated by ','", "User separate with ','"));
    mainLayout->addRow(i18n("Users:"), mUsers);

    mReadOnly = new QCheckBox(this);
    mReadOnly->setObjectName(QStringLiteral("mReadOnly"));
    mReadOnly->setChecked(false);
    mainLayout->addRow(i18n("Read-Only:"), mReadOnly);

    mBroadcast = new QCheckBox(this);
    mBroadcast->setObjectName(QStringLiteral("mBroadcast"));
    mBroadcast->setChecked(false);
    mainLayout->addRow(i18n("Broadcast:"), mBroadcast);

    mPrivate = new QCheckBox(this);
    mPrivate->setObjectName(QStringLiteral("mPrivate"));
    mPrivate->setChecked(false);
    mainLayout->addRow(i18n("Private Room:"), mPrivate);

    mEncryptedRoom = new QCheckBox(this);
    mEncryptedRoom->setObjectName(QStringLiteral("mEncryptedRoom"));
    mEncryptedRoom->setChecked(false);
    mainLayout->addRow(i18n("Encrypted Room:"), mEncryptedRoom);

    mPasswordLineEdit = new KPasswordLineEdit(this);
    mPasswordLineEdit->setObjectName(QStringLiteral("mPasswordLineEdit"));
    mainLayout->addRow(i18n("Password:"), mPasswordLineEdit);

    connect(mChannelName, &QLineEdit::textChanged, this, &CreateNewChannelWidget::slotChangeOkButtonEnabled);
}

CreateNewChannelWidget::~CreateNewChannelWidget()
{
}

void CreateNewChannelWidget::slotChangeOkButtonEnabled()
{
    Q_EMIT updateOkButton(!mChannelName->text().trimmed().isEmpty());
}

QString CreateNewChannelWidget::channelName() const
{
    return mChannelName->text();
}

QString CreateNewChannelWidget::users() const
{
    return mUsers->text();
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

