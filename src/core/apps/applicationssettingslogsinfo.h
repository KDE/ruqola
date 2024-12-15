/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"

#include <QDebug>

class LIBRUQOLACORE_EXPORT ApplicationsSettingsLogsInfo
{
public:
    struct LIBRUQOLACORE_EXPORT LogsArgument {
        QString caller;
        QString method;
        QString severity;
        QStringList args;
    };

    ApplicationsSettingsLogsInfo();
    ~ApplicationsSettingsLogsInfo();
    [[nodiscard]] QString method() const;
    void setMethod(const QString &newMethod);

    void parseLogs(const QJsonObject &obj);

    [[nodiscard]] bool operator==(const ApplicationsSettingsLogsInfo &other) const;

private:
    QList<LogsArgument> mArguments;
    QString mMethod;
};

Q_DECLARE_METATYPE(ApplicationsSettingsLogsInfo)
Q_DECLARE_TYPEINFO(ApplicationsSettingsLogsInfo, Q_RELOCATABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ApplicationsSettingsLogsInfo &t);
