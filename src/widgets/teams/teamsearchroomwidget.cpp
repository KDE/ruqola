/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamsearchroomwidget.h"
using namespace Qt::Literals::StringLiterals;

#include "teamsearchroomforteamwidget.h"
#include <QVBoxLayout>

TeamSearchRoomWidget::TeamSearchRoomWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mTeamSearchRoomForTeamWidget(new TeamSearchRoomForTeamWidget(account, this))
{
    auto vboxLayout = new QVBoxLayout(this);
    vboxLayout->setObjectName(u"vboxLayout"_s);
    vboxLayout->setContentsMargins({});

    mTeamSearchRoomForTeamWidget->setObjectName(u"mTeamSearchRoomForTeamWidget"_s);
    vboxLayout->addWidget(mTeamSearchRoomForTeamWidget);
    connect(mTeamSearchRoomForTeamWidget, &TeamSearchRoomForTeamWidget::roomListChanged, this, &TeamSearchRoomWidget::enableOkButton);
}

TeamSearchRoomWidget::~TeamSearchRoomWidget() = default;

QList<QByteArray> TeamSearchRoomWidget::roomIds() const
{
    return mTeamSearchRoomForTeamWidget->roomIds();
}

#include "moc_teamsearchroomwidget.cpp"
