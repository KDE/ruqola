/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqola_private_export.h"
#include <QJsonObject>
#include <QString>

class LIBRUQOLACORE_TESTS_EXPORT LicensesManager
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
