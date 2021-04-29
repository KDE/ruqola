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
