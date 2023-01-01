/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqola_private_export.h"
#include <QJsonArray>
#include <QString>

class LIBRUQOLACORE_TESTS_EXPORT LicensesManager
{
public:
    LicensesManager();
    ~LicensesManager();

    Q_REQUIRED_RESULT const QStringList &licenses() const;
    void setLicenses(const QStringList &newLicenses);

    Q_REQUIRED_RESULT bool hasLicense(const QString &name);

    void parseLicenses(const QJsonArray &obj);

private:
    QStringList mLicenses;
};
