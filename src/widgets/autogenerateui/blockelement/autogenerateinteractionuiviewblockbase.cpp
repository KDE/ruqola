/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiviewblockbase.h"
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
AutoGenerateInteractionUiViewBlockBase::AutoGenerateInteractionUiViewBlockBase(QObject *parent)
    : QObject(parent)
{
}

AutoGenerateInteractionUiViewBlockBase::~AutoGenerateInteractionUiViewBlockBase() = default;

QByteArray AutoGenerateInteractionUiViewBlockBase::appId() const
{
    return mAppId;
}

void AutoGenerateInteractionUiViewBlockBase::setAppId(const QByteArray &newAppId)
{
    mAppId = newAppId;
}

QByteArray AutoGenerateInteractionUiViewBlockBase::blockId() const
{
    return mBlockId;
}

void AutoGenerateInteractionUiViewBlockBase::setBlockId(const QByteArray &newBlockId)
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

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewBlockBase::StateInfo &t)
{
    d.space() << "actionId:" << t.actionId;
    d.space() << "value:" << t.value;
    return d;
}

bool AutoGenerateInteractionUiViewBlockBase::operator==(const AutoGenerateInteractionUiViewBlockBase &other) const
{
    return other.blockId() == blockId() && other.appId() == appId() && other.type() == type();
}

void AutoGenerateInteractionUiViewBlockBase::parse(const QJsonObject &json)
{
    mAppId = json["appId"_L1].toString().toLatin1();
    mBlockId = json["blockId"_L1].toString().toLatin1();
    mType = json["type"_L1].toString().toLatin1();
    parseBlock(json);
}

QList<AutoGenerateInteractionUiViewBlockBase::StateInfo> AutoGenerateInteractionUiViewBlockBase::serializeState() const
{
    return serializeBlockState();
}

QJsonObject AutoGenerateInteractionUiViewBlockBase::serialize() const
{
    QJsonObject o;
    o["type"_L1] = QString::fromLatin1(mType);
    o["appId"_L1] = QString::fromLatin1(mAppId);
    o["blockId"_L1] = QString::fromLatin1(mBlockId);
    serializeBlock(o);
    return o;
}

QWidget *AutoGenerateInteractionUiViewBlockBase::generateWidget([[maybe_unused]] QWidget *parent)
{
    return nullptr;
}

QByteArray AutoGenerateInteractionUiViewBlockBase::type() const
{
    return mType;
}

void AutoGenerateInteractionUiViewBlockBase::setType(const QByteArray &newType)
{
    mType = newType;
}

#include "moc_autogenerateinteractionuiviewblockbase.cpp"
