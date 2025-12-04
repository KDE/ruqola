/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "blockaccessory.h"
#include "ruqola_debug.h"

#include <QJsonArray>
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
BlockAccessory::BlockAccessory() = default;

BlockAccessory::~BlockAccessory() = default;

bool BlockAccessory::isValid() const
{
    return !mActionId.isEmpty() && (mType != BlockAccessory::AccessoryType::Unknown);
}

BlockAccessory::AccessoryType BlockAccessory::type() const
{
    return mType;
}

void BlockAccessory::setType(BlockAccessory::AccessoryType newType)
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
    d.space() << "text:" << t.text();
    return d;
}

bool BlockAccessory::operator==(const BlockAccessory &other) const
{
    return other.actionId() == actionId() && other.value() == value() && other.type() == type() && other.options() == options() && other.text() == text();
}

void BlockAccessory::parseAccessory(const QJsonObject &o)
{
    // {"accessory":{"actionId":"finish","options":[{"text":{"emoji":false,"text":"Finish
    // poll","type":"plain_text"},"value":"finish"}],"type":"overflow"},"appId":"c33fa1a6-68a7-491e-bf49-9d7b99671c48","blockId":"6c2122f0-faaa-11ef-85ff-4bacdd8b2d67","text":{"emoji":false,"text":"sdfsdf","type":"plain_text"},"type":"section"}

    mActionId = o["actionId"_L1].toString().toLatin1();
    mValue = o["value"_L1].toString();
    mType = convertAccessoryTypeToEnum(o["type"_L1].toString());
    mText = o["text"_L1]["text"_L1].toString();
    const QJsonArray optionsArray = o["options"_L1].toArray();
    for (const auto &r : optionsArray) {
        BlockAccessoryOption option;
        option.parse(r.toObject());
        mOptions.append(option);
    }
}

QJsonObject BlockAccessory::serialize(const BlockAccessory &block)
{
    QJsonObject o;
    o["actionId"_L1] = QString::fromLatin1(block.actionId());
    if (const QString value = block.value(); !value.isEmpty()) {
        o["value"_L1] = value;
    }
    if (!block.text().isEmpty()) {
        o["text"_L1] = block.text();
    }
    o["type"_L1] = BlockAccessory::convertEnumToStr(block.type());
    QJsonArray array;
    for (const auto &opt : block.options()) {
        array.append(BlockAccessoryOption::serialize(opt));
    }
    if (!array.isEmpty()) {
        o["options"_L1] = array;
    }
    return o;
}

BlockAccessory BlockAccessory::deserialize(const QJsonObject &o)
{
    BlockAccessory accessory;
    accessory.setActionId(o["actionId"_L1].toString().toLatin1());
    accessory.setValue(o["value"_L1].toString());
    accessory.setType(convertAccessoryTypeToEnum(o["type"_L1].toString()));
    accessory.setText(o["text"_L1].toString());
    const QJsonArray optionsArray = o["options"_L1].toArray();
    QList<BlockAccessoryOption> options;
    options.reserve(optionsArray.count());
    for (const auto &r : optionsArray) {
        const BlockAccessoryOption option = BlockAccessoryOption::deserialize(r.toObject());
        options.append(option);
    }
    accessory.setOptions(std::move(options));
    return accessory;
}

QString BlockAccessory::convertEnumToStr(AccessoryType newBlockType)
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

QString BlockAccessory::text() const
{
    return mText;
}

void BlockAccessory::setText(const QString &newText)
{
    mText = newText;
}

QList<BlockAccessoryOption> BlockAccessory::options() const
{
    return mOptions;
}

void BlockAccessory::setOptions(const QList<BlockAccessoryOption> &newOptions)
{
    mOptions = newOptions;
}

#include "moc_blockaccessory.cpp"
