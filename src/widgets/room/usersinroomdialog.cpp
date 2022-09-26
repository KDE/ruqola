/*
   SPDX-FileCopyrightText: 2021-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usersinroomdialog.h"
#include "usersinroomwidget.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QWindow>

namespace
{
static const char myUsersInRoomDialogGroupName[] = "UsersInRoomDialog";
}

UsersInRoomDialog::UsersInRoomDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mUsersInRoomWidget(new UsersInRoomWidget(account, this))
{
    setWindowTitle(i18nc("@title:window", "Users"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mUsersInRoomWidget->setObjectName(QStringLiteral("mUsersInRoomWidget"));
    mainLayout->addWidget(mUsersInRoomWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    buttonBox->setObjectName(QStringLiteral("button"));
    connect(buttonBox, &QDialogButtonBox::rejected, this, &UsersInRoomDialog::reject);
    mainLayout->addWidget(buttonBox);
    readConfig();
}

UsersInRoomDialog::~UsersInRoomDialog()
{
    writeConfig();
}

void UsersInRoomDialog::setRoom(Room *room)
{
    mUsersInRoomWidget->setRoom(room);
}

void UsersInRoomDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myUsersInRoomDialogGroupName);
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

void UsersInRoomDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(800, 600));
    KConfigGroup group(KSharedConfig::openStateConfig(), myUsersInRoomDialogGroupName);
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}
