/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamselectdeletedroomdialog.h"

#include "teamselectdeletedroomwidget.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <TextAddonsWidgets/LoadDialogSizeUtils>
namespace
{
const char myTeamSelectDeletedRoomDialogConfigGroupName[] = "TeamSelectDeletedRoomDialog";
}
using namespace Qt::Literals::StringLiterals;
TeamSelectDeletedRoomDialog::TeamSelectDeletedRoomDialog(QWidget *parent)
    : QDialog(parent)
    , mTeamSelectDeletedRoomWidget(new TeamSelectDeletedRoomWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Delete Rooms"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mTeamSelectDeletedRoomWidget->setObjectName(u"mTeamSelectDeletedRoomWidget"_s);
    mainLayout->addWidget(mTeamSelectDeletedRoomWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(u"button"_s);
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &TeamSelectDeletedRoomDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &TeamSelectDeletedRoomDialog::accept);
    readConfig();
}

TeamSelectDeletedRoomDialog::~TeamSelectDeletedRoomDialog() = default;

void TeamSelectDeletedRoomDialog::setTeamRooms(QList<TeamRoom> rooms)
{
    mTeamSelectDeletedRoomWidget->setTeamRooms(std::move(rooms));
}

void TeamSelectDeletedRoomDialog::readConfig()
{
    TextAddonsWidgets::LoadDialogSizeUtils::manageDialogSize(this, QLatin1StringView(myTeamSelectDeletedRoomDialogConfigGroupName), QSize(400, 300));
}

QList<QByteArray> TeamSelectDeletedRoomDialog::roomsId() const
{
    return mTeamSelectDeletedRoomWidget->roomsId();
}

#include "moc_teamselectdeletedroomdialog.cpp"
