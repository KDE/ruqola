/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
#include <QString>
#include <QStringList>

class QJsonObject;
class LIBRUQOLACORE_EXPORT LicensesManager
{
public:
    LicensesManager();
    ~LicensesManager();

    [[nodiscard]] const QStringList &licenses() const;
    void setLicenses(const QStringList &newLicenses);

    [[nodiscard]] bool hasLicense(const QString &name);

    void parseLicenses(const QJsonObject &root);

private:
    QStringList mLicenses;
};
