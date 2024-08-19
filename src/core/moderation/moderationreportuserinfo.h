/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
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

private:
    QString mDescription;
};

Q_DECLARE_METATYPE(ModerationReportUserInfo)
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ModerationReportUserInfo &t);
