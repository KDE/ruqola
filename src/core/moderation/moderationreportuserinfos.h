/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolacore_export.h"
#include "moderationreportuserinfo.h"
#include "user.h"
class QJsonObject;

class LIBRUQOLACORE_EXPORT ModerationReportUserInfos
{
public:
    ModerationReportUserInfos();
    ~ModerationReportUserInfos();

    [[nodiscard]] bool operator==(const ModerationReportUserInfos &other) const;

    [[nodiscard]] QList<ModerationReportUserInfo> moderationReportUserInfosList() const;
    void setModerationReportUserInfosList(const QList<ModerationReportUserInfo> &newModerationReportUserInfosList);

    [[nodiscard]] int offset() const;
    void setOffset(int newOffset);

    [[nodiscard]] int total() const;
    void setTotal(int newTotal);

    [[nodiscard]] User user() const;
    void setUser(const User &newUser);

    [[nodiscard]] int moderationReportUserInfosCount() const;
    void setModerationReportUserInfosCount(int newModerationReportUserInfosCount);

    void parseModerationReportUserInfos(const QJsonObject &moderationReportedUserInfosObj);

private:
    LIBRUQOLACORE_NO_EXPORT void parseModerationReportUserInfosObj(const QJsonObject &ModerationReportedUserInfosObj);
    QList<ModerationReportUserInfo> mModerationReportUserInfosList;
    User mUser;
    int mOffset = 0;
    int mTotal = 0;
    int mModerationReportUserInfosCount = 0;
};
QT_DECL_METATYPE_EXTERN_TAGGED(ModerationReportUserInfos, Ruqola_ModerationReportUserInfos, LIBRUQOLACORE_EXPORT)
class QDebug;
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ModerationReportUserInfos &t);
