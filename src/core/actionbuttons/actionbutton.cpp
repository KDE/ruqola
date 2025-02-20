/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "actionbutton.h"

ActionButton::ActionButton() = default;

ActionButton::~ActionButton() = default;

QString ActionButton::appId() const
{
    return mAppId;
}

void ActionButton::setAppId(const QString &newAppId)
{
    mAppId = newAppId;
}

QString ActionButton::actionId() const
{
    return mActionId;
}

void ActionButton::setActionId(const QString &newActionId)
{
    mActionId = newActionId;
}

QDebug operator<<(QDebug d, const ActionButton &t)
{
    d.space() << "appId:" << t.appId();
    d.space() << "actionId:" << t.actionId();
    return d;
}
