/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationinfos.h"
#include "ruqola_debug.h"

#include <QJsonArray>

ModerationInfos::ModerationInfos() = default;

QDebug operator<<(QDebug d, const ModerationInfos &t)
{
    d << "total " << t.total();
    d << "offset " << t.offset();
    d << "ModerationInfosCount " << t.ModerationInfosCount() << "\n";
    for (int i = 0, total = t.ModerationInfosList().count(); i < total; ++i) {
        d << t.ModerationInfosList().at(i) << "\n";
    }
    return d;
}

int ModerationInfos::offset() const
{
    return mOffset;
}

void ModerationInfos::setOffset(int newOffset)
{
    mOffset = newOffset;
}

int ModerationInfos::total() const
{
    return mTotal;
}

void ModerationInfos::setTotal(int newTotal)
{
    mTotal = newTotal;
}

int ModerationInfos::ModerationInfosCount() const
{
    return mModerationInfosCount;
}

void ModerationInfos::setModerationInfosCount(int newModerationInfosCount)
{
    mModerationInfosCount = newModerationInfosCount;
}

const QVector<ModerationInfo> &ModerationInfos::ModerationInfosList() const
{
    return mModerationInfosList;
}

void ModerationInfos::setModerationInfosList(const QVector<ModerationInfo> &newModerationInfosList)
{
    mModerationInfosList = newModerationInfosList;
}

bool ModerationInfos::isEmpty() const
{
    return mModerationInfosList.isEmpty();
}

void ModerationInfos::clear()
{
    mModerationInfosList.clear();
}

int ModerationInfos::count() const
{
    return mModerationInfosList.count();
}

ModerationInfo ModerationInfos::at(int index) const
{
    if (index < 0 || index > mModerationInfosList.count()) {
        qCWarning(RUQOLA_LOG) << "Invalid index " << index;
        return {};
    }
    return mModerationInfosList.at(index);
}

void ModerationInfos::parseModerationInfos(const QJsonObject &ModerationInfosObj)
{
    mModerationInfosList.clear();
    mModerationInfosCount = ModerationInfosObj[QLatin1String("count")].toInt();
    mOffset = ModerationInfosObj[QLatin1String("offset")].toInt();
    mTotal = ModerationInfosObj[QLatin1String("total")].toInt();
    mModerationInfosList.reserve(mModerationInfosCount);
    parseModerationInfosObj(ModerationInfosObj);
}

void ModerationInfos::parseModerationInfosObj(const QJsonObject &ModerationInfosObj)
{
    const QJsonArray discussionsArray = ModerationInfosObj[QLatin1String("sessions")].toArray();
    for (const QJsonValue &current : discussionsArray) {
        if (current.type() == QJsonValue::Object) {
            const QJsonObject discussionObject = current.toObject();
            ModerationInfo m;
            m.parseModerationInfo(discussionObject);
            mModerationInfosList.append(std::move(m));
        } else {
            qCWarning(RUQOLA_LOG) << "Problem when parsing discussions" << current;
        }
    }
}

void ModerationInfos::parseMoreModerationInfos(const QJsonObject &ModerationInfosObj)
{
    const int ModerationInfosCount = ModerationInfosObj[QLatin1String("count")].toInt();
    mOffset = ModerationInfosObj[QLatin1String("offset")].toInt();
    mTotal = ModerationInfosObj[QLatin1String("total")].toInt();
    parseModerationInfosObj(ModerationInfosObj);
    mModerationInfosCount += ModerationInfosCount;
}

ModerationInfo ModerationInfos::takeAt(int index)
{
    return mModerationInfosList.takeAt(index);
}
