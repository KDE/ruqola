/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "actionbutton.h"

ActionButton::ActionButton() = default;

ActionButton::~ActionButton() = default;

QByteArray ActionButton::appId() const
{
    return mAppId;
}

void ActionButton::setAppId(const QByteArray &newAppId)
{
    mAppId = newAppId;
}

QByteArray ActionButton::actionId() const
{
    return mActionId;
}

void ActionButton::setActionId(const QByteArray &newActionId)
{
    mActionId = newActionId;
}

QString ActionButton::labelI18n() const
{
    return mLabelI18n;
}

void ActionButton::setLabelI18n(const QString &newLabelI18n)
{
    mLabelI18n = newLabelI18n;
}

ActionButton::RoomTypeFilter ActionButton::roomTypeFilter() const
{
    return mRoomTypeFilter;
}

void ActionButton::setRoomTypeFilter(RoomTypeFilter newRoomTypeFilter)
{
    mRoomTypeFilter = newRoomTypeFilter;
}

QDebug operator<<(QDebug d, const ActionButton &t)
{
    d.space() << "appId:" << t.appId();
    d.space() << "actionId:" << t.actionId();
    d.space() << "labelI18n:" << t.labelI18n();
    d.space() << "roomTypeFilter:" << static_cast<int>(t.roomTypeFilter());
    return d;
}
