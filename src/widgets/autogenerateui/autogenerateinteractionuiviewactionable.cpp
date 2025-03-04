/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiviewactionable.h"
using namespace Qt::Literals::StringLiterals;
AutoGenerateInteractionUiViewActionable::AutoGenerateInteractionUiViewActionable(QObject *parent)
    : QObject(parent)
{
}

AutoGenerateInteractionUiViewActionable::~AutoGenerateInteractionUiViewActionable() = default;

void AutoGenerateInteractionUiViewActionable::parse(const QJsonObject &json)
{
    mAppId = json["appId"_L1].toString().toLatin1();
    mBlockId = json["blockId"_L1].toString().toLatin1();
    mActionId = json["actionId"_L1].toString().toLatin1();
    mType = json["type"_L1].toString().toLatin1();
    parseElement(json);
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
    return other.appId() == appId() && other.blockId() == blockId() && other.actionId() == actionId() && other.type() == type();
}

QWidget *AutoGenerateInteractionUiViewActionable::generateWidget(RocketChatAccount *account, QWidget *parent)
{
    Q_UNUSED(parent)
    Q_UNUSED(account)
    // TODO
    return nullptr;
}

QByteArray AutoGenerateInteractionUiViewActionable::type() const
{
    return mType;
}

void AutoGenerateInteractionUiViewActionable::setType(const QByteArray &newType)
{
    mType = newType;
}

QJsonObject AutoGenerateInteractionUiViewActionable::serialize() const
{
    QJsonObject o;
    o["type"_L1] = QString::fromLatin1(mType);
    if (!mActionId.isEmpty()) {
        o["actionId"_L1] = QString::fromLatin1(mActionId);
    }
    if (!mAppId.isEmpty()) {
        o["appId"_L1] = QString::fromLatin1(mAppId);
    }
    if (!mBlockId.isEmpty()) {
        o["blockId"_L1] = QString::fromLatin1(mBlockId);
    }
    serializeElement(o);
    return o;
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewActionable &t)
{
    d.space() << "appId:" << t.appId();
    d.space() << "blockId:" << t.blockId();
    d.space() << "actionId:" << t.actionId();
    d.space() << "type:" << t.type();
    return d;
}
