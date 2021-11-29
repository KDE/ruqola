/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
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
InviteUsersDialog::InviteUsersDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mInviteUsersWidget(new InviteUsersWidget(account, this))
{
    setWindowTitle(i18nc("@title:window", "Invite Users"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

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
    KConfigGroup group(KSharedConfig::openStateConfig(), myInviteUsersDialogGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(300, 300));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void InviteUsersDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myInviteUsersDialogGroupName);
    group.writeEntry("Size", size());
}
