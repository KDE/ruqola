/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "moderationreportinfo.h"
#include <QDebug>
#include <QList>
#include <QString>

class LIBRUQOLACORE_EXPORT ModerationReportInfos
{
public:
    ModerationReportInfos();

    void clear();

    [[nodiscard]] int offset() const;
    void setOffset(int newOffset);

    [[nodiscard]] int total() const;
    void setTotal(int newTotal);

    [[nodiscard]] int moderationReportInfosCount() const;
    void setModerationReportInfosCount(int newModerationReportInfosCount);

    [[nodiscard]] const QList<ModerationReportInfo> &moderationReportInfosList() const;
    void setModerationReportInfosList(const QList<ModerationReportInfo> &newModerationReportInfosList);

    [[nodiscard]] bool isEmpty() const;
    [[nodiscard]] int count() const;
    [[nodiscard]] ModerationReportInfo at(int index) const;

    void parseModerationReportInfos(const QJsonObject &moderationReportInfosObj);
    void parseMoreModerationReportInfos(const QJsonObject &moderationReportInfosObj);

    ModerationReportInfo takeAt(int index);

private:
    LIBRUQOLACORE_NO_EXPORT void parseModerationReportInfosObj(const QJsonObject &moderationReportInfosObj);
    QList<ModerationReportInfo> mModerationReportInfosList;
    int mModerationReportInfosCount = 0;
    int mOffset = 0;
    int mTotal = 0;
};
Q_DECLARE_METATYPE(ModerationReportInfos)
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ModerationReportInfos &t);
