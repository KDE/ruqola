/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamchannelsdialog.h"
#include "teamchannelswidget.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QWindow>
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
    create(); // ensure a window is created
    windowHandle()->resize(QSize(400, 300));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1String(myTeamChannelsDialogConfigGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void TeamChannelsDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1String(myTeamChannelsDialogConfigGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

#include "moc_teamchannelsdialog.cpp"
