/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiviewblockbase.h"

AutoGenerateInteractionUiViewBlockBase::AutoGenerateInteractionUiViewBlockBase() = default;

AutoGenerateInteractionUiViewBlockBase::~AutoGenerateInteractionUiViewBlockBase() = default;

QString AutoGenerateInteractionUiViewBlockBase::appId() const
{
    return mAppId;
}

void AutoGenerateInteractionUiViewBlockBase::setAppId(const QString &newAppId)
{
    mAppId = newAppId;
}

QString AutoGenerateInteractionUiViewBlockBase::blockId() const
{
    return mBlockId;
}

void AutoGenerateInteractionUiViewBlockBase::setBlockId(const QString &newBlockId)
{
    mBlockId = newBlockId;
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewBlockBase &t)
{
    d.space() << "blockId:" << t.blockId();
    d.space() << "appId:" << t.appId();
    return d;
}
