/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationreporteduserinfos.h"
#include "ruqola_debug.h"

#include <QJsonArray>
#include <QJsonObject>

ModerationReportedUserInfos::ModerationReportedUserInfos() = default;

QDebug operator<<(QDebug d, const ModerationReportedUserInfos &t)
{
    d.space() << "total" << t.total();
    d.space() << "offset" << t.offset();
    d.space() << "ModerationReportedUserInfosCount" << t.ModerationReportedUserInfosCount() << "\n";
    for (int i = 0, total = t.ModerationReportedUserInfosList().count(); i < total; ++i) {
        d << t.ModerationReportedUserInfosList().at(i) << "\n";
    }
    return d;
}

int ModerationReportedUserInfos::offset() const
{
    return mOffset;
}

void ModerationReportedUserInfos::setOffset(int newOffset)
{
    mOffset = newOffset;
}

int ModerationReportedUserInfos::total() const
{
    return mTotal;
}

void ModerationReportedUserInfos::setTotal(int newTotal)
{
    mTotal = newTotal;
}

int ModerationReportedUserInfos::ModerationReportedUserInfosCount() const
{
    return mModerationReportedUserInfosCount;
}

void ModerationReportedUserInfos::setModerationReportedUserInfosCount(int newModerationReportedUserInfosCount)
{
    mModerationReportedUserInfosCount = newModerationReportedUserInfosCount;
}

const QList<ModerationReportedUserInfo> &ModerationReportedUserInfos::ModerationReportedUserInfosList() const
{
    return mModerationReportedUserInfosList;
}

void ModerationReportedUserInfos::setModerationReportedUserInfosList(const QList<ModerationReportedUserInfo> &newModerationReportedUserInfosList)
{
    mModerationReportedUserInfosList = newModerationReportedUserInfosList;
}

bool ModerationReportedUserInfos::isEmpty() const
{
    return mModerationReportedUserInfosList.isEmpty();
}

void ModerationReportedUserInfos::clear()
{
    mModerationReportedUserInfosList.clear();
}

int ModerationReportedUserInfos::count() const
{
    return mModerationReportedUserInfosList.count();
}

ModerationReportedUserInfo ModerationReportedUserInfos::at(int index) const
{
    if (index < 0 || index > mModerationReportedUserInfosList.count()) {
        qCWarning(RUQOLA_LOG) << "Invalid index " << index;
        return {};
    }
    return mModerationReportedUserInfosList.at(index);
}

void ModerationReportedUserInfos::parseModerationReportedUserInfos(const QJsonObject &ModerationReportedUserInfosObj)
{
    mModerationReportedUserInfosList.clear();
    // qDebug() << " ModerationReportedUserInfosObj " << ModerationReportedUserInfosObj;
    mModerationReportedUserInfosCount = ModerationReportedUserInfosObj[QLatin1StringView("count")].toInt();
    mOffset = ModerationReportedUserInfosObj[QLatin1StringView("offset")].toInt();
    mTotal = ModerationReportedUserInfosObj[QLatin1StringView("total")].toInt();
    mModerationReportedUserInfosList.reserve(mModerationReportedUserInfosCount);
    parseModerationReportedUserInfosObj(ModerationReportedUserInfosObj);
}

void ModerationReportedUserInfos::parseModerationReportedUserInfosObj(const QJsonObject &ModerationReportedUserInfosObj)
{
    const QJsonArray moderationsArray = ModerationReportedUserInfosObj[QLatin1StringView("reports")].toArray();
    for (const QJsonValue &current : moderationsArray) {
        if (current.type() == QJsonValue::Object) {
            const QJsonObject moderationObject = current.toObject();
            ModerationReportedUserInfo m;
            m.parseModerationReportedUserInfo(moderationObject);
            mModerationReportedUserInfosList.append(std::move(m));
        } else {
            qCWarning(RUQOLA_LOG) << "Problem when parsing moderations" << current;
        }
    }
}

void ModerationReportedUserInfos::parseMoreModerationReportedUserInfos(const QJsonObject &ModerationReportedUserInfosObj)
{
    const int ModerationReportedUserInfosCount = ModerationReportedUserInfosObj[QLatin1StringView("count")].toInt();
    mOffset = ModerationReportedUserInfosObj[QLatin1StringView("offset")].toInt();
    mTotal = ModerationReportedUserInfosObj[QLatin1StringView("total")].toInt();
    parseModerationReportedUserInfosObj(ModerationReportedUserInfosObj);
    mModerationReportedUserInfosCount += ModerationReportedUserInfosCount;
}

ModerationReportedUserInfo ModerationReportedUserInfos::takeAt(int index)
{
    return mModerationReportedUserInfosList.takeAt(index);
}
