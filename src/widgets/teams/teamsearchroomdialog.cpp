/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamsearchroomdialog.h"
#include "teamsearchroomwidget.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QWindow>
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
    create(); // ensure a window is created
    windowHandle()->resize(QSize(400, 300));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1String(myTeamSearchRoomDialogConfigGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void TeamSearchRoomDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1String(myTeamSearchRoomDialogConfigGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

#include "moc_teamsearchroomdialog.cpp"
