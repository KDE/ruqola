/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "usersinroomdialog.h"
#include "usersinroomwidget.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>

namespace
{
static const char myUsersInRoomDialogGroupName[] = "UsersInRoomDialog";
}

UsersInRoomDialog::UsersInRoomDialog(QWidget *parent)
    : QDialog(parent)
    , mUsersInRoomWidget(new UsersInRoomWidget(this))
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
    group.writeEntry("Size", size());
}

void UsersInRoomDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myUsersInRoomDialogGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(800, 600));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}
