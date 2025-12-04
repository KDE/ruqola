/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
#include <QJsonObject>

class LIBRUQOLACORE_EXPORT ModerationReportInfo
{
public:
    ModerationReportInfo();
    ~ModerationReportInfo();

    [[nodiscard]] bool operator==(const ModerationReportInfo &other) const;
    [[nodiscard]] QString description() const;
    void setDescription(const QString &newDescription);

    [[nodiscard]] QByteArray reportIdentifier() const;
    void setReportIdentifier(const QByteArray &newReportIdentifier);

    void parseModerationReportInfo(const QJsonObject &o);

    [[nodiscard]] qint64 timeStamp() const;
    void setTimeStamp(qint64 newTimeStamp);

    [[nodiscard]] QByteArray roomId() const;
    void setRoomId(const QByteArray &newRoomId);

    [[nodiscard]] QByteArray userId() const;
    void setUserId(const QByteArray &newUserId);

    [[nodiscard]] QString userName() const;
    void setUserName(const QString &newUserName);

    [[nodiscard]] QString timeStampDateTimeStr() const;

private:
    LIBRUQOLACORE_NO_EXPORT void parseRoom(const QJsonObject &o);
    LIBRUQOLACORE_NO_EXPORT void parseReportedBy(const QJsonObject &o);
    QString mTimeStampDateTimeStr;
    QString mDescription;
    QByteArray mReportIdentifier;
    QByteArray mRoomId;
    QString mUserName;
    QByteArray mUserId;
    qint64 mTimeStamp = -1;
};
Q_DECLARE_METATYPE(ModerationReportInfo)
Q_DECLARE_TYPEINFO(ModerationReportInfo, Q_RELOCATABLE_TYPE);
class QDebug;
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ModerationReportInfo &t);
