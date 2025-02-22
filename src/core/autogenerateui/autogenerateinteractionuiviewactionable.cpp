/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiviewactionable.h"

AutoGenerateInteractionUiViewActionable::AutoGenerateInteractionUiViewActionable() = default;

AutoGenerateInteractionUiViewActionable::~AutoGenerateInteractionUiViewActionable() = default;

void AutoGenerateInteractionUiViewActionable::parse(const QJsonObject &json)
{
}

QString AutoGenerateInteractionUiViewActionable::appId() const
{
    return mAppId;
}

void AutoGenerateInteractionUiViewActionable::setAppId(const QString &newAppId)
{
    mAppId = newAppId;
}

QString AutoGenerateInteractionUiViewActionable::blockId() const
{
    return mBlockId;
}

void AutoGenerateInteractionUiViewActionable::setBlockId(const QString &newBlockId)
{
    mBlockId = newBlockId;
}

QString AutoGenerateInteractionUiViewActionable::actionId() const
{
    return mActionId;
}

void AutoGenerateInteractionUiViewActionable::setActionId(const QString &newActionId)
{
    mActionId = newActionId;
}
