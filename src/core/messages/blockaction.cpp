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
    return d;
}

bool BlockAction::isValid() const
{
    // TODO
    return false;
}

void BlockAction::parseActions()
{
    // TODO
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
    return mActionId == other.actionId() && mText == other.text() && mType == other.type() && mBlockId == other.blockId();
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

QJsonObject BlockAction::serialize(const BlockAction &blockAction)
{
    // TODO
    return {};
}

BlockAction BlockAction::deserialize(const QJsonObject &o)
{
    // TODO
    return {};
}
