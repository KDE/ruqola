/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "blockaccessory.h"
using namespace Qt::Literals::StringLiterals;
BlockAccessory::BlockAccessory() = default;

BlockAccessory::~BlockAccessory() = default;

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
    return d;
}

bool BlockAccessory::operator==(const BlockAccessory &other) const
{
    return other.actionId() == actionId() && other.value() == value();
}

void BlockAccessory::parseAccessory(const QJsonObject &o)
{
    mActionId = o["actionId"_L1].toString().toLatin1();
    mValue = o["value"_L1].toString();
}
