/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "blockaccessory.h"
#include "ruqola_debug.h"
using namespace Qt::Literals::StringLiterals;
BlockAccessory::BlockAccessory() = default;

BlockAccessory::~BlockAccessory() = default;

bool BlockAccessory::isValid() const
{
    return !mActionId.isEmpty();
}

QString BlockAccessory::type() const
{
    return mType;
}

void BlockAccessory::setType(const QString &newType)
{
    mType = newType;
}

QByteArray BlockAccessory::actionId() const
{
    return mActionId;
}

void BlockAccessory::setActionId(const QByteArray &newActionId)
{
    mActionId = newActionId;
}

QString BlockAccessory::value() const
{
    return mValue;
}

void BlockAccessory::setValue(const QString &newValue)
{
    mValue = newValue;
}

QDebug operator<<(QDebug d, const BlockAccessory &t)
{
    d.space() << "actionId:" << t.actionId();
    d.space() << "value:" << t.value();
    d.space() << "type:" << t.type();
    d.space() << "options:" << t.options();
    return d;
}

bool BlockAccessory::operator==(const BlockAccessory &other) const
{
    return other.actionId() == actionId() && other.value() == value() && other.type() == type() && other.options() == options();
}

void BlockAccessory::parseAccessory(const QJsonObject &o)
{
    // {"accessory":{"actionId":"finish","options":[{"text":{"emoji":false,"text":"Finish
    // poll","type":"plain_text"},"value":"finish"}],"type":"overflow"},"appId":"c33fa1a6-68a7-491e-bf49-9d7b99671c48","blockId":"6c2122f0-faaa-11ef-85ff-4bacdd8b2d67","text":{"emoji":false,"text":"sdfsdf","type":"plain_text"},"type":"section"}

    mActionId = o["actionId"_L1].toString().toLatin1();
    mValue = o["value"_L1].toString();
    mType = o["type"_L1].toString();
    if (o.contains("options"_L1)) {
        // TODO add option
    }
}

QJsonObject BlockAccessory::serialize(const BlockAccessory &block)
{
    QJsonObject o;
    o["actionId"_L1] = QString::fromLatin1(block.actionId());
    o["value"_L1] = block.value();
    // TODO type/options
    return o;
}

BlockAccessory BlockAccessory::deserialize(const QJsonObject &o)
{
    BlockAccessory accessory;
    accessory.setActionId(o["actionId"_L1].toString().toLatin1());
    accessory.setValue(o["value"_L1].toString());
    return accessory;
}

QString BlockAccessory::convertEnumToStr(AccessoryType newBlockType) const
{
    switch (newBlockType) {
    case AccessoryType::Unknown:
        return {};
    case AccessoryType::Button:
        return "button"_L1;
    case AccessoryType::Overflow:
        return "overflow"_L1;
    }
    return {};
}

BlockAccessory::AccessoryType BlockAccessory::convertAccessoryTypeToEnum(const QString &type)
{
    if (type == "button"_L1) {
        return AccessoryType::Button;
    } else if (type == "overflow"_L1) {
        return AccessoryType::Overflow;
    } else {
        qCWarning(RUQOLA_LOG) << "Unknown accessory type " << type;
    }
    return BlockAccessory::AccessoryType::Unknown;
}

QList<BlockAccessoryOption> BlockAccessory::options() const
{
    return mOptions;
}

void BlockAccessory::setOptions(const QList<BlockAccessoryOption> &newOptions)
{
    mOptions = newOptions;
}
