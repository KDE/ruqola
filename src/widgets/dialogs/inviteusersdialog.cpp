/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "inviteusersdialog.h"
#include "inviteuserswidget.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QWindow>

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
    create(); // ensure a window is created
    windowHandle()->resize(QSize(300, 300));
    KConfigGroup group(KSharedConfig::openStateConfig(), myInviteUsersDialogGroupName);
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void InviteUsersDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myInviteUsersDialogGroupName);
    KWindowConfig::saveWindowSize(windowHandle(), group);
}
