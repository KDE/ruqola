/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "blockaction.h"

BlockAction::BlockAction() = default;

QDebug operator<<(QDebug d, const BlockAction &t)
{
    d.space() << "actionId" << t.actionId();
    d.space() << "text" << t.text();
    d.space() << "type" << t.type();
    d.space() << "blockId" << t.blockId();
    d.space() << "value" << t.value();
    return d;
}

QString BlockAction::value() const
{
    return mValue;
}

void BlockAction::setValue(const QString &newValue)
{
    mValue = newValue;
}

bool BlockAction::isValid() const
{
    return !mActionId.isEmpty() && !mBlockId.isEmpty() && !mType.isEmpty() && !mText.isEmpty();
}

void BlockAction::parseAction(const QJsonObject &o)
{
    mActionId = o[QLatin1String("actionId")].toString();
    mBlockId = o[QLatin1String("blockId")].toString();
    mType = o[QLatin1String("type")].toString();
    mValue = o[QLatin1String("value")].toString();
    const QJsonObject objText = o[QLatin1String("text")].toObject();
    mText = objText[QLatin1String("text")].toString();
}

QString BlockAction::actionId() const
{
    return mActionId;
}

void BlockAction::setActionId(const QString &newActionId)
{
    mActionId = newActionId;
}

bool BlockAction::operator==(const BlockAction &other) const
{
    return mActionId == other.actionId() && mText == other.text() && mType == other.type() && mBlockId == other.blockId() && mValue == other.value();
}

QString BlockAction::text() const
{
    return mText;
}

void BlockAction::setText(const QString &newText)
{
    mText = newText;
}

QString BlockAction::type() const
{
    return mType;
}

void BlockAction::setType(const QString &newType)
{
    mType = newType;
}

QString BlockAction::blockId() const
{
    return mBlockId;
}

void BlockAction::setBlockId(const QString &newBlockId)
{
    mBlockId = newBlockId;
}
