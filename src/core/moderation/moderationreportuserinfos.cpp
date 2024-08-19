/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationreportuserinfos.h"

ModerationReportUserInfos::ModerationReportUserInfos() = default;

ModerationReportUserInfos::~ModerationReportUserInfos() = default;

QDebug operator<<(QDebug d, const ModerationReportUserInfos &t)
{
    // TODO
    return d;
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
