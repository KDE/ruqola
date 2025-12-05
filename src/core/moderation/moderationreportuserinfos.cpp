/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationreportuserinfos.h"
QT_IMPL_METATYPE_EXTERN_TAGGED(ModerationReportUserInfos, Ruqola_ModerationReportUserInfos)

#include "ruqola_debug.h"
#include <QJsonArray>

#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
ModerationReportUserInfos::ModerationReportUserInfos() = default;

ModerationReportUserInfos::~ModerationReportUserInfos() = default;

QDebug operator<<(QDebug d, const ModerationReportUserInfos &t)
{
    d.space() << "total" << t.total();
    d.space() << "offset" << t.offset();
    d.space() << "moderationReportUserInfosCount" << t.moderationReportUserInfosCount();
    d.space() << "user" << t.user();
    for (int i = 0, total = t.moderationReportUserInfosList().count(); i < total; ++i) {
        d.space() << t.moderationReportUserInfosList().at(i) << "\n";
    }
    return d;
}

bool ModerationReportUserInfos::operator==(const ModerationReportUserInfos &other) const
{
    return mUser == other.mUser && mOffset == other.mOffset && mTotal == other.mTotal && mModerationReportUserInfosList == other.mModerationReportUserInfosList
        && mModerationReportUserInfosCount == other.mModerationReportUserInfosCount;
}

QList<ModerationReportUserInfo> ModerationReportUserInfos::moderationReportUserInfosList() const
{
    return mModerationReportUserInfosList;
}

void ModerationReportUserInfos::setModerationReportUserInfosList(const QList<ModerationReportUserInfo> &newModerationReportUserInfosList)
{
    mModerationReportUserInfosList = newModerationReportUserInfosList;
}

int ModerationReportUserInfos::offset() const
{
    return mOffset;
}

void ModerationReportUserInfos::setOffset(int newOffset)
{
    mOffset = newOffset;
}

int ModerationReportUserInfos::total() const
{
    return mTotal;
}

void ModerationReportUserInfos::setTotal(int newTotal)
{
    mTotal = newTotal;
}

User ModerationReportUserInfos::user() const
{
    return mUser;
}

void ModerationReportUserInfos::setUser(const User &newUser)
{
    mUser = newUser;
}

int ModerationReportUserInfos::moderationReportUserInfosCount() const
{
    return mModerationReportUserInfosCount;
}

void ModerationReportUserInfos::setModerationReportUserInfosCount(int newModerationReportUserInfosCount)
{
    mModerationReportUserInfosCount = newModerationReportUserInfosCount;
}

void ModerationReportUserInfos::parseModerationReportUserInfos(const QJsonObject &moderationReportedUserInfosObj)
{
    mModerationReportUserInfosList.clear();
    // qDebug() << " ModerationReportedUserInfosObj " << ModerationReportedUserInfosObj;
    mModerationReportUserInfosCount = moderationReportedUserInfosObj["count"_L1].toInt();
    mOffset = moderationReportedUserInfosObj["offset"_L1].toInt();
    mTotal = moderationReportedUserInfosObj["total"_L1].toInt();
    mModerationReportUserInfosList.reserve(mModerationReportUserInfosCount);
    mUser.parseUserRestApi(moderationReportedUserInfosObj["user"_L1].toObject(), {});
    parseModerationReportUserInfosObj(moderationReportedUserInfosObj);
}

void ModerationReportUserInfos::parseModerationReportUserInfosObj(const QJsonObject &ModerationReportedUserInfosObj)
{
    const QJsonArray moderationsArray = ModerationReportedUserInfosObj["reports"_L1].toArray();
    for (const QJsonValue &current : moderationsArray) {
        if (current.type() == QJsonValue::Object) {
            const QJsonObject moderationObject = current.toObject();
            ModerationReportUserInfo m;
            // TODO isValid ?
            m.parseModerationReportUserInfo(moderationObject);
            mModerationReportUserInfosList.append(std::move(m));
        } else {
            qCWarning(RUQOLA_LOG) << "Problem when parsing ModerationReportUserInfos" << current;
        }
    }
}
