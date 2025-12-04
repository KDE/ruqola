/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"

#include <QDateTime>

class LIBRUQOLACORE_EXPORT ApplicationsSettingsLogsInfo
{
public:
    struct LIBRUQOLACORE_EXPORT LogsArgument {
        QString caller;
        QString method;
        QString severity;
        QByteArray args;
        void parseArguments(const QJsonObject &obj);

        [[nodiscard]] bool operator==(const LogsArgument &other) const;
    };

    ApplicationsSettingsLogsInfo();
    ~ApplicationsSettingsLogsInfo();
    [[nodiscard]] QString method() const;
    void setMethod(const QString &newMethod);

    void parseLogs(const QJsonObject &obj);

    [[nodiscard]] bool operator==(const ApplicationsSettingsLogsInfo &other) const;

    [[nodiscard]] QList<LogsArgument> arguments() const;
    void setArguments(const QList<LogsArgument> &newArguments);

    [[nodiscard]] QDateTime createdAt() const;
    void setCreatedAt(const QDateTime &newCreatedAt);

    [[nodiscard]] qint64 totalTime() const;
    void setTotalTime(qint64 newTotalTime);

private:
    QList<LogsArgument> mArguments;
    QString mMethod;
    QDateTime mCreatedAt;
    qint64 mTotalTime = 0;
};

Q_DECLARE_METATYPE(ApplicationsSettingsLogsInfo)
Q_DECLARE_TYPEINFO(ApplicationsSettingsLogsInfo, Q_RELOCATABLE_TYPE);
class QDebug;
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ApplicationsSettingsLogsInfo &t);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ApplicationsSettingsLogsInfo::LogsArgument &arg);
