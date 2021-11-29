/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "teamchannelsdialog.h"
#include "teamchannelswidget.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>
namespace
{
const char myTeamChannelsDialogConfigGroupName[] = "TeamChannelsDialog";
}
TeamChannelsDialog::TeamChannelsDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mTeamChannelsWidget(new TeamChannelsWidget(account, this))
{
    setWindowTitle(i18nc("@title:window", "Team Channels"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mTeamChannelsWidget->setObjectName(QStringLiteral("mTeamChannelsWidget"));
    mainLayout->addWidget(mTeamChannelsWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &TeamChannelsDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &TeamChannelsDialog::accept);
    readConfig();
}

TeamChannelsDialog::~TeamChannelsDialog()
{
    writeConfig();
}

void TeamChannelsDialog::setRoom(Room *room)
{
    mTeamChannelsWidget->setRoom(room);
}

void TeamChannelsDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myTeamChannelsDialogConfigGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(400, 300));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void TeamChannelsDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myTeamChannelsDialogConfigGroupName);
    group.writeEntry("Size", size());
}
