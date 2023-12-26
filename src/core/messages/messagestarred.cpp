/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagestarred.h"
#include <QJsonArray>
#include <QJsonObject>
MessageStarred::MessageStarred() = default;

bool MessageStarred::isStarred() const
{
    return mIsStarred;
}

void MessageStarred::parse(const QJsonObject &o)
{
    if (o.contains(QLatin1String("starred"))) {
        mIsStarred = !o.value(QStringLiteral("starred")).toArray().isEmpty();
    } else {
        mIsStarred = false;
    }
}

void MessageStarred::setIsStarred(bool isStarred)
{
    mIsStarred = isStarred;
}

bool MessageStarred::operator==(MessageStarred other) const
{
    return mIsStarred == other.isStarred();
}

QDebug operator<<(QDebug d, MessageStarred t)
{
    d.space() << "isStarred" << t.isStarred();
    return d;
}
