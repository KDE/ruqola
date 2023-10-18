/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
#include <QDebug>
#include <QJsonObject>

class LIBRUQOLACORE_EXPORT ModerationReportInfo
{
public:
    ModerationReportInfo();
    ~ModerationReportInfo();

    [[nodiscard]] bool operator==(const ModerationReportInfo &other) const;
    [[nodiscard]] QString description() const;
    void setDescription(const QString &newDescription);

    [[nodiscard]] QString reportIdentifier() const;
    void setReportIdentifier(const QString &newReportIdentifier);

    void parseModerationReportInfo(const QJsonObject &o);

    [[nodiscard]] qint64 timeStamp() const;
    void setTimeStamp(qint64 newTimeStamp);

    [[nodiscard]] QString roomId() const;
    void setRoomId(const QString &newRoomId);

    [[nodiscard]] QString userId() const;
    void setUserId(const QString &newUserId);

private:
    LIBRUQOLACORE_NO_EXPORT void parseRoom(const QJsonObject &o);
    LIBRUQOLACORE_NO_EXPORT void parseReportedBy(const QJsonObject &o);
    QString mTimeStampDateTimeStr;
    QString mDescription;
    QString mReportIdentifier;
    QString mRoomId;
    QString mUserId;
    qint64 mTimeStamp = -1;
};
Q_DECLARE_METATYPE(ModerationReportInfo)
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ModerationReportInfo &t);
