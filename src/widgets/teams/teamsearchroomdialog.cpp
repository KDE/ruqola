/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "teamsearchroomdialog.h"
#include "teamsearchroomwidget.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>
namespace
{
const char myTeamSearchRoomDialogConfigGroupName[] = "TeamSearchRoomDialog";
}
TeamSearchRoomDialog::TeamSearchRoomDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mTeamSearchRoomWidget(new TeamSearchRoomWidget(account, this))
{
    setWindowTitle(i18nc("@title:window", "Search Rooms"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mTeamSearchRoomWidget->setObjectName(QStringLiteral("mTeamSearchRoomWidget"));
    mainLayout->addWidget(mTeamSearchRoomWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &TeamSearchRoomDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &TeamSearchRoomDialog::accept);
    readConfig();
}

TeamSearchRoomDialog::~TeamSearchRoomDialog()
{
    writeConfig();
}

QStringList TeamSearchRoomDialog::roomIds() const
{
    return mTeamSearchRoomWidget->roomIds();
}

void TeamSearchRoomDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myTeamSearchRoomDialogConfigGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(400, 300));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void TeamSearchRoomDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myTeamSearchRoomDialogConfigGroupName);
    group.writeEntry("Size", size());
}
