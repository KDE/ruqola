/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "licensesmanager.h"
#include <QJsonArray>

#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;

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

void LicensesManager::parseLicenses(const QJsonObject &root)
{
    const QJsonObject license = root["license"_L1].toObject();
    const QJsonArray replyArray = license["activeModules"_L1].toArray();
    mLicenses.clear();
    for (int i = 0, total = replyArray.count(); i < total; ++i) {
        mLicenses.append(replyArray.at(i).toString());
    }
}
