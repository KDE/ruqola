/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamselectdeletedroomdialog.h"
using namespace Qt::Literals::StringLiterals;

#include "teamselectdeletedroomwidget.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QWindow>
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

TeamSelectDeletedRoomDialog::~TeamSelectDeletedRoomDialog()
{
    writeConfig();
}

void TeamSelectDeletedRoomDialog::setTeamRooms(const QList<TeamRoom> &rooms)
{
    mTeamSelectDeletedRoomWidget->setTeamRooms(rooms);
}

void TeamSelectDeletedRoomDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(400, 300));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myTeamSelectDeletedRoomDialogConfigGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void TeamSelectDeletedRoomDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myTeamSelectDeletedRoomDialogConfigGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

QList<QByteArray> TeamSelectDeletedRoomDialog::roomsId() const
{
    return mTeamSelectDeletedRoomWidget->roomsId();
}

#include "moc_teamselectdeletedroomdialog.cpp"
