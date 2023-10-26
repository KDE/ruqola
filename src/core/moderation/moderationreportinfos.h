/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "moderationreportinfo.h"
#include <QDebug>
#include <QJsonObject>
#include <QString>
#include <QVector>

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

    [[nodiscard]] const QVector<ModerationReportInfo> &moderationReportInfosList() const;
    void setModerationReportInfosList(const QVector<ModerationReportInfo> &newModerationReportInfosList);

    [[nodiscard]] bool isEmpty() const;
    [[nodiscard]] int count() const;
    [[nodiscard]] ModerationReportInfo at(int index) const;

    void parseModerationReportInfos(const QJsonObject &moderationReportInfosObj);
    void parseMoreModerationReportInfos(const QJsonObject &moderationReportInfosObj);

    ModerationReportInfo takeAt(int index);

private:
    LIBRUQOLACORE_NO_EXPORT void parseModerationReportInfosObj(const QJsonObject &moderationReportInfosObj);
    QVector<ModerationReportInfo> mModerationReportInfosList;
    int mModerationReportInfosCount = 0;
    int mOffset = 0;
    int mTotal = 0;
};
Q_DECLARE_METATYPE(ModerationReportInfos)
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ModerationReportInfos &t);
