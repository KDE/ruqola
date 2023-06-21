/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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

    auto deleteLabel = new QLabel(i18n("Select the Team’s Channels you would like to delete, the ones you do not select will be moved to the Workspace. Notice "
                                       "that public Channels will be public and visible to everyone."),
                                  this);
    deleteLabel->setObjectName(QStringLiteral("deleteLabel"));
    deleteLabel->setWordWrap(true);
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

#include "moc_teamconverttochannelwidget.cpp"
