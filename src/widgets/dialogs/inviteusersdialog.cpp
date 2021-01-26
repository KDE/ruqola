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

#include "inviteusersdialog.h"
#include "inviteuserswidget.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>

namespace
{
static const char myInviteUsersDialogGroupName[] = "InviteUsersDialog";
}
InviteUsersDialog::InviteUsersDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18nc("@title:window", "Invite Users"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mInviteUsersWidget = new InviteUsersWidget(this);
    mInviteUsersWidget->setObjectName(QStringLiteral("mInviteUsersWidget"));
    mainLayout->addWidget(mInviteUsersWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    buttonBox->setObjectName(QStringLiteral("button"));
    connect(buttonBox, &QDialogButtonBox::rejected, this, &InviteUsersDialog::reject);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &InviteUsersDialog::accept);
    mainLayout->addWidget(buttonBox);
    readConfig();
}

InviteUsersDialog::~InviteUsersDialog()
{
    writeConfig();
}

QString InviteUsersDialog::roomId() const
{
    return mInviteUsersWidget->roomId();
}

void InviteUsersDialog::setRoomId(const QString &roomId)
{
    mInviteUsersWidget->setRoomId(roomId);
}

void InviteUsersDialog::generateLink()
{
    mInviteUsersWidget->generateLink();
}

void InviteUsersDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), myInviteUsersDialogGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(300, 300));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void InviteUsersDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), myInviteUsersDialogGroupName);
    group.writeEntry("Size", size());
}
