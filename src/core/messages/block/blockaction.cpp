/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "blockaction.h"
using namespace Qt::Literals::StringLiterals;

BlockAction::BlockAction() = default;

QDebug operator<<(QDebug d, const BlockAction &t)
{
    d.space() << "actionId" << t.actionId();
    d.space() << "text" << t.text();
    d.space() << "type" << t.type();
    d.space() << "blockId" << t.blockId();
    d.space() << "value" << t.value();
    d.space() << "url" << t.url();
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

QString BlockAction::url() const
{
    return mUrl;
}

void BlockAction::setUrl(const QString &newUrl)
{
    mUrl = newUrl;
}

QJsonObject BlockAction::serialize(const BlockAction &block)
{
    QJsonObject o;
    o["actionId"_L1] = block.actionId();
    if (const QString blockId = block.blockId(); !blockId.isEmpty()) {
        o["blockId"_L1] = blockId;
    }
    o["type"_L1] = block.type();
    if (const QString value = block.value(); !value.isEmpty()) {
        o["value"_L1] = value;
    }
    if (const QString blockUrl = block.url(); !blockUrl.isEmpty()) {
        o["url"_L1] = blockUrl;
    }
    o["text"_L1] = block.text();
    return o;
}

BlockAction BlockAction::deserialize(const QJsonObject &o)
{
    BlockAction blockAction;
    blockAction.setActionId(o["actionId"_L1].toString());
    blockAction.setBlockId(o["blockId"_L1].toString());
    blockAction.setType(o["type"_L1].toString());
    blockAction.setValue(o["value"_L1].toString());
    blockAction.setUrl(o["url"_L1].toString());
    blockAction.setText(o["text"_L1].toString());

    return blockAction;
}

bool BlockAction::isValid() const
{
    return !mActionId.isEmpty() /*&& !mBlockId.isEmpty()*/ && !mType.isEmpty() && !mText.isEmpty();
}

void BlockAction::parseAction(const QJsonObject &o)
{
    mActionId = o["actionId"_L1].toString();
    mBlockId = o["blockId"_L1].toString();
    mType = o["type"_L1].toString();
    mValue = o["value"_L1].toString();
    mUrl = o["url"_L1].toString();
    const QJsonObject objText = o["text"_L1].toObject();
    if (objText.isEmpty()) {
        mText = o["text"_L1].toString();
    } else {
        mText = objText["text"_L1].toString();
    }
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
    return mActionId == other.actionId() && mText == other.text() && mType == other.type() && mBlockId == other.blockId() && mValue == other.value()
        && mUrl == other.url();
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
