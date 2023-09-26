/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

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
    d << "ModerationInfosCount " << t.moderationInfosCount() << "\n";
    for (int i = 0, total = t.moderationInfosList().count(); i < total; ++i) {
        d << t.moderationInfosList().at(i) << "\n";
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

int ModerationInfos::moderationInfosCount() const
{
    return mModerationInfosCount;
}

void ModerationInfos::setModerationInfosCount(int newModerationInfosCount)
{
    mModerationInfosCount = newModerationInfosCount;
}

const QVector<ModerationInfo> &ModerationInfos::moderationInfosList() const
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

void ModerationInfos::parseModerationInfos(const QJsonObject &moderationInfosObj)
{
    mModerationInfosList.clear();
    mModerationInfosCount = moderationInfosObj[QLatin1String("count")].toInt();
    mOffset = moderationInfosObj[QLatin1String("offset")].toInt();
    mTotal = moderationInfosObj[QLatin1String("total")].toInt();
    mModerationInfosList.reserve(mModerationInfosCount);
    parseModerationInfosObj(moderationInfosObj);
}

void ModerationInfos::parseModerationInfosObj(const QJsonObject &moderationInfosObj)
{
    const QJsonArray moderationsArray = moderationInfosObj[QLatin1String("reports")].toArray();
    for (const QJsonValue &current : moderationsArray) {
        if (current.type() == QJsonValue::Object) {
            const QJsonObject moderationObject = current.toObject();
            ModerationInfo m;
            m.parseModerationInfo(moderationObject);
            mModerationInfosList.append(std::move(m));
        } else {
            qCWarning(RUQOLA_LOG) << "Problem when parsing moderations" << current;
        }
    }
}

void ModerationInfos::parseMoreModerationInfos(const QJsonObject &moderationInfosObj)
{
    const int moderationInfosCount = moderationInfosObj[QLatin1String("count")].toInt();
    mOffset = moderationInfosObj[QLatin1String("offset")].toInt();
    mTotal = moderationInfosObj[QLatin1String("total")].toInt();
    parseModerationInfosObj(moderationInfosObj);
    mModerationInfosCount += moderationInfosCount;
}

ModerationInfo ModerationInfos::takeAt(int index)
{
    return mModerationInfosList.takeAt(index);
}
