/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelrolesinfowidget.h"

#include "room.h"

#include <QFormLayout>

ChannelRolesInfoWidget::ChannelRolesInfoWidget(QWidget *parent)
    : QWidget{parent}
    , mFormLayout(new QFormLayout(this))
{
    mFormLayout->setObjectName(QStringLiteral("mFormLayout"));
    mFormLayout->setContentsMargins({});
}

ChannelRolesInfoWidget::~ChannelRolesInfoWidget() = default;

void ChannelRolesInfoWidget::setRoom(Room *room)
{
    // TODO
}
