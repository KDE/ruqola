/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamsearchroomwidget.h"
#include "teamsearchroomforteamwidget.h"
#include <QVBoxLayout>

TeamSearchRoomWidget::TeamSearchRoomWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mTeamSearchRoomForTeamWidget(new TeamSearchRoomForTeamWidget(account, this))
{
    auto vboxLayout = new QVBoxLayout(this);
    vboxLayout->setObjectName(QStringLiteral("vboxLayout"));
    vboxLayout->setContentsMargins({});

    mTeamSearchRoomForTeamWidget->setObjectName(QStringLiteral("mTeamSearchRoomForTeamWidget"));
    vboxLayout->addWidget(mTeamSearchRoomForTeamWidget);
    connect(mTeamSearchRoomForTeamWidget, &TeamSearchRoomForTeamWidget::userListChanged, this, &TeamSearchRoomWidget::enableOkButton);
}

TeamSearchRoomWidget::~TeamSearchRoomWidget() = default;

QList<QByteArray> TeamSearchRoomWidget::roomIds() const
{
    return mTeamSearchRoomForTeamWidget->roomIds();
}

#include "moc_teamsearchroomwidget.cpp"
