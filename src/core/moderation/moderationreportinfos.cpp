/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationreportinfos.h"
#include "ruqola_debug.h"

#include <QJsonArray>

ModerationReportInfos::ModerationReportInfos() = default;

QDebug operator<<(QDebug d, const ModerationReportInfos &t)
{
    d << "total " << t.total();
    d << "offset " << t.offset();
    d << "ModerationReportInfosCount " << t.moderationReportInfosCount() << "\n";
    for (int i = 0, total = t.moderationReportInfosList().count(); i < total; ++i) {
        d << t.moderationReportInfosList().at(i) << "\n";
    }
    return d;
}

int ModerationReportInfos::offset() const
{
    return mOffset;
}

void ModerationReportInfos::setOffset(int newOffset)
{
    mOffset = newOffset;
}

int ModerationReportInfos::total() const
{
    return mTotal;
}

void ModerationReportInfos::setTotal(int newTotal)
{
    mTotal = newTotal;
}

int ModerationReportInfos::moderationReportInfosCount() const
{
    return mModerationReportInfosCount;
}

void ModerationReportInfos::setModerationReportInfosCount(int newModerationReportInfosCount)
{
    mModerationReportInfosCount = newModerationReportInfosCount;
}

const QVector<ModerationReportInfo> &ModerationReportInfos::moderationReportInfosList() const
{
    return mModerationReportInfosList;
}

void ModerationReportInfos::setModerationReportInfosList(const QVector<ModerationReportInfo> &newModerationReportInfosList)
{
    mModerationReportInfosList = newModerationReportInfosList;
}

bool ModerationReportInfos::isEmpty() const
{
    return mModerationReportInfosList.isEmpty();
}

void ModerationReportInfos::clear()
{
    mModerationReportInfosList.clear();
}

int ModerationReportInfos::count() const
{
    return mModerationReportInfosList.count();
}

ModerationReportInfo ModerationReportInfos::at(int index) const
{
    if (index < 0 || index > mModerationReportInfosList.count()) {
        qCWarning(RUQOLA_LOG) << "Invalid index " << index;
        return {};
    }
    return mModerationReportInfosList.at(index);
}

void ModerationReportInfos::parseModerationReportInfos(const QJsonObject &moderationReportInfosObj)
{
    mModerationReportInfosList.clear();
    qDebug() << " moderationReportInfosObj " << moderationReportInfosObj;
    mModerationReportInfosCount = moderationReportInfosObj[QLatin1String("count")].toInt();
    mOffset = moderationReportInfosObj[QLatin1String("offset")].toInt();
    mTotal = moderationReportInfosObj[QLatin1String("total")].toInt();
    mModerationReportInfosList.reserve(mModerationReportInfosCount);
    parseModerationReportInfosObj(moderationReportInfosObj);
}

void ModerationReportInfos::parseModerationReportInfosObj(const QJsonObject &moderationReportInfosObj)
{
    const QJsonArray moderationsArray = moderationReportInfosObj[QLatin1String("reports")].toArray();
    for (const QJsonValue &current : moderationsArray) {
        if (current.type() == QJsonValue::Object) {
            const QJsonObject moderationObject = current.toObject();
            ModerationReportInfo m;
            m.parseModerationReportInfo(moderationObject);
            mModerationReportInfosList.append(std::move(m));
        } else {
            qCWarning(RUQOLA_LOG) << "Problem when parsing moderations" << current;
        }
    }
}

void ModerationReportInfos::parseMoreModerationReportInfos(const QJsonObject &moderationReportInfosObj)
{
    const int moderationReportInfosCount = moderationReportInfosObj[QLatin1String("count")].toInt();
    mOffset = moderationReportInfosObj[QLatin1String("offset")].toInt();
    mTotal = moderationReportInfosObj[QLatin1String("total")].toInt();
    parseModerationReportInfosObj(moderationReportInfosObj);
    mModerationReportInfosCount += moderationReportInfosCount;
}

ModerationReportInfo ModerationReportInfos::takeAt(int index)
{
    return mModerationReportInfosList.takeAt(index);
}
