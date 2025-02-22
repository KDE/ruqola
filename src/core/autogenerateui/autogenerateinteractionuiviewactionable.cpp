/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiviewactionable.h"
using namespace Qt::Literals::StringLiterals;
AutoGenerateInteractionUiViewActionable::AutoGenerateInteractionUiViewActionable() = default;

AutoGenerateInteractionUiViewActionable::~AutoGenerateInteractionUiViewActionable() = default;

void AutoGenerateInteractionUiViewActionable::parse(const QJsonObject &json)
{
    mAppId = json["appId"_L1].toString().toLatin1();
    mBlockId = json["blockId"_L1].toString().toLatin1();
    mActionId = json["actionId"_L1].toString().toLatin1();
}

QByteArray AutoGenerateInteractionUiViewActionable::appId() const
{
    return mAppId;
}

void AutoGenerateInteractionUiViewActionable::setAppId(const QByteArray &newAppId)
{
    mAppId = newAppId;
}

QByteArray AutoGenerateInteractionUiViewActionable::blockId() const
{
    return mBlockId;
}

void AutoGenerateInteractionUiViewActionable::setBlockId(const QByteArray &newBlockId)
{
    mBlockId = newBlockId;
}

QByteArray AutoGenerateInteractionUiViewActionable::actionId() const
{
    return mActionId;
}

void AutoGenerateInteractionUiViewActionable::setActionId(const QByteArray &newActionId)
{
    mActionId = newActionId;
}

bool AutoGenerateInteractionUiViewActionable::operator==(const AutoGenerateInteractionUiViewActionable &other) const
{
    return other.appId() == appId() && other.blockId() == blockId() && other.actionId() == other.actionId();
}
