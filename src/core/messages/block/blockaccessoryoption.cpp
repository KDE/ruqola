/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "blockaccessoryoption.h"
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
BlockAccessoryOption::BlockAccessoryOption() = default;

BlockAccessoryOption::~BlockAccessoryOption() = default;

void BlockAccessoryOption::parse(const QJsonObject &obj)
{
    mValue = obj["value"_L1].toString();
    mText = obj["text"_L1]["text"_L1].toString();
}

QJsonObject BlockAccessoryOption::serialize(const BlockAccessoryOption &block)
{
    QJsonObject o;
    o["value"_L1] = block.value();
    o["text"_L1] = block.text();
    return o;
}

BlockAccessoryOption BlockAccessoryOption::deserialize(const QJsonObject &o)
{
    BlockAccessoryOption option;
    option.setValue(o["value"_L1].toString());
    option.setText(o["text"_L1].toString());
    return option;
}

QDebug operator<<(QDebug d, const BlockAccessoryOption &t)
{
    d.space() << "value:" << t.value();
    d.space() << "text:" << t.text();
    return d;
}

QString BlockAccessoryOption::value() const
{
    return mValue;
}

void BlockAccessoryOption::setValue(const QString &newValue)
{
    mValue = newValue;
}

bool BlockAccessoryOption::operator==(const BlockAccessoryOption &other) const
{
    return mValue == other.value() && mText == other.text();
}

QString BlockAccessoryOption::text() const
{
    return mText;
}

void BlockAccessoryOption::setText(const QString &newText)
{
    mText = newText;
}
