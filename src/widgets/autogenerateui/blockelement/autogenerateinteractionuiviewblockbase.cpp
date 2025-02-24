/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiviewblockbase.h"
using namespace Qt::Literals::StringLiterals;
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

bool AutoGenerateInteractionUiViewBlockBase::operator==(const AutoGenerateInteractionUiViewBlockBase &other) const
{
    return other.blockId() == blockId() && other.appId() == appId();
}

void AutoGenerateInteractionUiViewBlockBase::parse(const QJsonObject &json)
{
    mAppId = json["appId"_L1].toString();
    mBlockId = json["blockId"_L1].toString();
}

void AutoGenerateInteractionUiViewBlockBase::generateWidget(QWidget *parent)
{
    // TODO
}
