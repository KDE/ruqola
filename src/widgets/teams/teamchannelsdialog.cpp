/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

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
TeamChannelsDialog::TeamChannelsDialog(QWidget *parent)
    : QDialog(parent)
    , mTeamChannelsWidget(new TeamChannelsWidget(this))
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

void TeamChannelsDialog::setTeamId(const QString &teamId)
{
    mTeamChannelsWidget->setTeamId(teamId);
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
