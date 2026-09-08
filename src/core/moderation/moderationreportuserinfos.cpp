/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationreportuserinfos.h"
QT_IMPL_METATYPE_EXTERN_TAGGED(ModerationReportUserInfos, Ruqola_ModerationReportUserInfos)

using namespace Qt::Literals::StringLiterals;

User ModerationReportUserInfos::user() const
{
    return mUser;
}

void ModerationReportUserInfos::setUser(const User &newUser)
{
    mUser = newUser;
}

void ModerationReportUserInfos::parseModerationReportUserInfos(const QJsonObject &obj)
{
    mUser.parseUserRestApi(obj["user"_L1].toObject(), {});
    parseInfos(obj, "reports"_L1);
}

QDebug operator<<(QDebug d, const ModerationReportUserInfos &t)
{
    d.space() << "total" << t.total();
    d.space() << "offset" << t.offset();
    d.space() << "moderationReportUserInfosCount" << t.loadedCount();
    d.space() << "user" << t.user();
    for (const ModerationReportUserInfo &info : t.list()) {
        d.space() << info << "\n";
    }
    return d;
}
