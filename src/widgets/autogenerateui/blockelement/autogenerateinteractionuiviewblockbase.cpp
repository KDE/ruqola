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
    d.space() << "type:" << t.type();
    return d;
}

bool AutoGenerateInteractionUiViewBlockBase::operator==(const AutoGenerateInteractionUiViewBlockBase &other) const
{
    return other.blockId() == blockId() && other.appId() == appId() && other.type() == type();
}

void AutoGenerateInteractionUiViewBlockBase::parse(const QJsonObject &json)
{
    mAppId = json["appId"_L1].toString();
    mBlockId = json["blockId"_L1].toString();
    mType = json["type"_L1].toString();
}

QJsonObject AutoGenerateInteractionUiViewBlockBase::serialize() const
{
    QJsonObject o;
    o["type"_L1] = mType;
    o["appId"_L1] = mAppId;
    o["blockId"_L1] = mBlockId;
    serializeBlock(o);
    return o;
}

QWidget *AutoGenerateInteractionUiViewBlockBase::generateWidget(QWidget *parent) const
{
    return nullptr;
}

QString AutoGenerateInteractionUiViewBlockBase::type() const
{
    return mType;
}

void AutoGenerateInteractionUiViewBlockBase::setType(const QString &newType)
{
    mType = newType;
}
