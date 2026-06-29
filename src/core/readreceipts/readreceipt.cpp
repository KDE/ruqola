/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "readreceipt.h"

QT_IMPL_METATYPE_EXTERN_TAGGED(ReadReceipt, Ruqola_ReadReceipt)
using namespace Qt::Literals::StringLiterals;

ReadReceipt::ReadReceipt() = default;

ReadReceipt::~ReadReceipt() = default;

QByteArray ReadReceipt::userId() const
{
    return mUserId;
}

void ReadReceipt::setUserId(const QByteArray &newUserId)
{
    mUserId = newUserId;
}

qint64 ReadReceipt::timeStamp() const
{
    return mTimeStamp;
}

void ReadReceipt::setTimeStamp(qint64 newTimeStamp)
{
    mTimeStamp = newTimeStamp;
}

qint64 ReadReceipt::updatedAt() const
{
    return mUpdatedAt;
}

void ReadReceipt::setUpdatedAt(qint64 newUpdatedAt)
{
    mUpdatedAt = newUpdatedAt;
}

QDebug operator<<(QDebug d, const ReadReceipt &t)
{
    d.space() << "UserId:" << t.userId();
    d.space() << "updatedAt:" << t.updatedAt();
    d.space() << "timeStamp:" << t.timeStamp();
    return d;
}

bool ReadReceipt::operator==(const ReadReceipt &other) const = default;
