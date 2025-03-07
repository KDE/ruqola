/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "blockaccessoryoption.h"

BlockAccessoryOption::BlockAccessoryOption() = default;

BlockAccessoryOption::~BlockAccessoryOption() = default;

void BlockAccessoryOption::parse(const QJsonObject &obj)
{
    // TODO
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
