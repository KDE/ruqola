/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "moderationreportedmessageinfo.h"
#include <QDebug>
#include <QList>

class LIBRUQOLACORE_EXPORT ModerationReportedMessageInfos
{
public:
    ModerationReportedMessageInfos();

    void clear();

    [[nodiscard]] int offset() const;
    void setOffset(int newOffset);

    [[nodiscard]] int total() const;
    void setTotal(int newTotal);

    [[nodiscard]] int moderationInfosCount() const;
    void setModerationInfosCount(int newModerationInfosCount);

    [[nodiscard]] const QList<ModerationReportedMessageInfo> &moderationInfosList() const;
    void setModerationInfosList(const QList<ModerationReportedMessageInfo> &newModerationInfosList);

    [[nodiscard]] bool isEmpty() const;
    [[nodiscard]] int count() const;
    [[nodiscard]] ModerationReportedMessageInfo at(int index) const;

    void parseModerationInfos(const QJsonObject &moderationInfosObj);
    void parseMoreModerationInfos(const QJsonObject &moderationInfosObj);

    ModerationReportedMessageInfo takeAt(int index);

private:
    LIBRUQOLACORE_NO_EXPORT void parseModerationInfosObj(const QJsonObject &moderationInfosObj);
    QList<ModerationReportedMessageInfo> mModerationInfosList;
    int mModerationInfosCount = 0;
    int mOffset = 0;
    int mTotal = 0;
};
Q_DECLARE_METATYPE(ModerationReportedMessageInfos)
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ModerationReportedMessageInfos &t);
