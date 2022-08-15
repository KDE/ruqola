/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "licensesmanager.h"

LicensesManager::LicensesManager() = default;

LicensesManager::~LicensesManager() = default;

const QStringList &LicensesManager::licenses() const
{
    return mLicenses;
}

void LicensesManager::setLicenses(const QStringList &newLicenses)
{
    mLicenses = newLicenses;
}

bool LicensesManager::hasLicense(const QString &name)
{
    return mLicenses.contains(name);
}
