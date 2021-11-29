/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "teamconverttochannelwidget.h"
#include "teamselectdeletedroomwidget.h"
#include <KLocalizedString>
#include <QLabel>
#include <QVBoxLayout>

TeamConvertToChannelWidget::TeamConvertToChannelWidget(QWidget *parent)
    : QWidget(parent)
    , mTeamSelectDeletedRoomWidget(new TeamSelectDeletedRoomWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    auto deleteLabel = new QLabel(i18n("Select Room To Delete:"), this);
    deleteLabel->setObjectName(QStringLiteral("deleteLabel"));
    mainLayout->addWidget(deleteLabel);

    mTeamSelectDeletedRoomWidget->setObjectName(QStringLiteral("mTeamSelectDeletedRoomWidget"));
    mainLayout->addWidget(mTeamSelectDeletedRoomWidget);
}

TeamConvertToChannelWidget::~TeamConvertToChannelWidget() = default;

QStringList TeamConvertToChannelWidget::roomIdsToDelete() const
{
    return mTeamSelectDeletedRoomWidget->roomsId();
}

void TeamConvertToChannelWidget::setTeamRooms(const QVector<TeamRoom> &rooms)
{
    mTeamSelectDeletedRoomWidget->setTeamRooms(rooms);
}
