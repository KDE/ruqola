/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationreportuserinfos.h"

ModerationReportUserInfos::ModerationReportUserInfos() = default;

ModerationReportUserInfos::~ModerationReportUserInfos() = default;

QDebug operator<<(QDebug d, const ModerationReportUserInfos &t)
{
    d.space() << "total" << t.total();
    d.space() << "offset" << t.offset();
#if 0
    d.space() << "ModerationReportUserInfos" << t.ModerationReportedUserInfosCount() << "\n";
    for (int i = 0, total = t.ModerationReportedUserInfosList().count(); i < total; ++i) {
        d << t.ModerationReportedUserInfosList().at(i) << "\n";
    }
#endif
    return d;
}

bool ModerationReportUserInfos::operator==(const ModerationReportUserInfos &other) const
{
    return mUser == other.mUser && mOffset == other.mOffset && mTotal == other.mTotal && mModerationReportUserInfosList == other.mModerationReportUserInfosList;
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
