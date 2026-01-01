/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamconverttochannelwidget.h"
using namespace Qt::Literals::StringLiterals;

#include "teamselectdeletedroomwidget.h"
#include <KLocalizedString>
#include <QLabel>
#include <QVBoxLayout>

TeamConvertToChannelWidget::TeamConvertToChannelWidget(QWidget *parent)
    : QWidget(parent)
    , mTeamSelectDeletedRoomWidget(new TeamSelectDeletedRoomWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    auto deleteLabel =
        new QLabel(i18nc("@label:textbox",
                         "Select the Team’s Channels you would like to delete, the ones you do not select will be moved to the Workspace. Notice "
                         "that public Channels will be public and visible to everyone."),
                   this);
    deleteLabel->setObjectName(u"deleteLabel"_s);
    deleteLabel->setWordWrap(true);
    mainLayout->addWidget(deleteLabel);

    mTeamSelectDeletedRoomWidget->setObjectName(u"mTeamSelectDeletedRoomWidget"_s);
    mainLayout->addWidget(mTeamSelectDeletedRoomWidget);
}

TeamConvertToChannelWidget::~TeamConvertToChannelWidget() = default;

QList<QByteArray> TeamConvertToChannelWidget::roomIdsToDelete() const
{
    return mTeamSelectDeletedRoomWidget->roomsId();
}

void TeamConvertToChannelWidget::setTeamRooms(const QList<TeamRoom> &rooms)
{
    mTeamSelectDeletedRoomWidget->setTeamRooms(rooms);
}

#include "moc_teamconverttochannelwidget.cpp"
