/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "readreceipt.h"
#include "utils.h"
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

void ReadReceipt::parseReadReceiptInfo(const QJsonObject &obj)
{
    mUserId = obj["userId"_L1].toString().toLatin1();
    mTimeStamp = Utils::parseDate(u"ts"_s, obj);
    mUpdatedAt = Utils::parseDate(u"_updatedAt"_s, obj);
}

bool ReadReceipt::isValid() const
{
    return !mUserId.isEmpty();
}
