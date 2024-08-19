/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolacore_export.h"
#include "moderationreportuserinfo.h"
#include "user.h"
#include <QDebug>
#include <QJsonObject>
class LIBRUQOLACORE_EXPORT ModerationReportUserInfos
{
public:
    ModerationReportUserInfos();
    ~ModerationReportUserInfos();

    [[nodiscard]] QList<ModerationReportUserInfo> moderationReportUserInfosList() const;
    void setModerationReportUserInfosList(const QList<ModerationReportUserInfo> &newModerationReportUserInfosList);

    [[nodiscard]] int offset() const;
    void setOffset(int newOffset);

    [[nodiscard]] int total() const;
    void setTotal(int newTotal);

private:
    QList<ModerationReportUserInfo> mModerationReportUserInfosList;
    User mUser;
    int mOffset = 0;
    int mTotal = 0;
};
Q_DECLARE_METATYPE(ModerationReportUserInfos)
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ModerationReportUserInfos &t);
