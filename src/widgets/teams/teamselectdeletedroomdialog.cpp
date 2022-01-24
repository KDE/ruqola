/*
   SPDX-FileCopyrightText: 2021-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamselectdeletedroomdialog.h"
#include "teamselectdeletedroomwidget.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>
namespace
{
const char myTeamSelectDeletedRoomDialogConfigGroupName[] = "TeamSelectDeletedRoomDialog";
}
TeamSelectDeletedRoomDialog::TeamSelectDeletedRoomDialog(QWidget *parent)
    : QDialog(parent)
    , mTeamSelectDeletedRoomWidget(new TeamSelectDeletedRoomWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Delete Rooms"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mTeamSelectDeletedRoomWidget->setObjectName(QStringLiteral("mTeamSelectDeletedRoomWidget"));
    mainLayout->addWidget(mTeamSelectDeletedRoomWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &TeamSelectDeletedRoomDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &TeamSelectDeletedRoomDialog::accept);
    readConfig();
}

TeamSelectDeletedRoomDialog::~TeamSelectDeletedRoomDialog()
{
    writeConfig();
}

void TeamSelectDeletedRoomDialog::setTeamRooms(const QVector<TeamRoom> &rooms)
{
    mTeamSelectDeletedRoomWidget->setTeamRooms(rooms);
}

void TeamSelectDeletedRoomDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myTeamSelectDeletedRoomDialogConfigGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(400, 300));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void TeamSelectDeletedRoomDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myTeamSelectDeletedRoomDialogConfigGroupName);
    group.writeEntry("Size", size());
}

QStringList TeamSelectDeletedRoomDialog::roomsId() const
{
    return mTeamSelectDeletedRoomWidget->roomsId();
}
