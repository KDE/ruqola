/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "moderationreporteduserinfo.h"
#include <QList>

class LIBRUQOLACORE_EXPORT ModerationReportedUserInfos
{
public:
    ModerationReportedUserInfos();

    void clear();

    [[nodiscard]] int offset() const;
    void setOffset(int newOffset);

    [[nodiscard]] int total() const;
    void setTotal(int newTotal);

    [[nodiscard]] int moderationReportedUserInfosCount() const;
    void setModerationReportedUserInfosCount(int newModerationReportedUserInfosCount);

    [[nodiscard]] const QList<ModerationReportedUserInfo> &ModerationReportedUserInfosList() const;
    void setModerationReportedUserInfosList(const QList<ModerationReportedUserInfo> &newModerationReportedUserInfosList);

    [[nodiscard]] bool isEmpty() const;
    [[nodiscard]] int count() const;
    [[nodiscard]] ModerationReportedUserInfo at(int index) const;

    void parseModerationReportedUserInfos(const QJsonObject &ModerationReportedUserInfosObj);
    void parseMoreModerationReportedUserInfos(const QJsonObject &ModerationReportedUserInfosObj);

    ModerationReportedUserInfo takeAt(int index);

private:
    LIBRUQOLACORE_NO_EXPORT void parseModerationReportedUserInfosObj(const QJsonObject &ModerationReportedUserInfosObj);
    QList<ModerationReportedUserInfo> mModerationReportedUserInfosList;
    int mModerationReportedUserInfosCount = 0;
    int mOffset = 0;
    int mTotal = 0;
};
QT_DECL_METATYPE_EXTERN_TAGGED(ModerationReportedUserInfos, Ruqola_ModerationReportedUserInfos, LIBRUQOLACORE_EXPORT)
class QDebug;
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ModerationReportedUserInfos &t);
