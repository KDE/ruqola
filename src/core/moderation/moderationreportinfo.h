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

private:
    QString mDescription;
};
