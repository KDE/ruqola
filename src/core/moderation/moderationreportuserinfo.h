/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
#include "user.h"
#include <QDebug>
#include <QJsonObject>

class LIBRUQOLACORE_EXPORT ModerationReportUserInfo
{
public:
    ModerationReportUserInfo();
    ~ModerationReportUserInfo();
    [[nodiscard]] bool operator==(const ModerationReportUserInfo &other) const;

    void parseModerationReportUserInfo(const QJsonObject &o);

    [[nodiscard]] QString description() const;
    void setDescription(const QString &newDescription);

    [[nodiscard]] QByteArray reportId() const;
    void setReportId(const QByteArray &newReportId);

    [[nodiscard]] User reportedBy() const;
    void setReportedBy(const User &newReportedBy);

    [[nodiscard]] User reportedUser() const;
    void setReportedUser(const User &newReportedUser);

    [[nodiscard]] qint64 timeStamp() const;
    void setTimeStamp(qint64 newTimeStamp);

private:
    User mReportedBy;
    User mReportedUser;
    QString mDescription;
    QByteArray mReportId;
    qint64 mTimeStamp = -1;
};

Q_DECLARE_METATYPE(ModerationReportUserInfo)
Q_DECLARE_TYPEINFO(ModerationReportUserInfo, Q_RELOCATABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ModerationReportUserInfo &t);
