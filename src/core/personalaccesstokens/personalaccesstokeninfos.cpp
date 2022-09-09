/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "personalaccesstokeninfos.h"
#include "ruqola_debug.h"

#include <QJsonArray>

PersonalAccessTokenInfos::PersonalAccessTokenInfos() = default;

QVector<PersonalAccessTokenInfo> PersonalAccessTokenInfos::personalAccessTokenInfos() const
{
    return mPersonalAccessTokenInfos;
}

int PersonalAccessTokenInfos::count() const
{
    return mPersonalAccessTokenInfos.count();
}

void PersonalAccessTokenInfos::clear()
{
    mPersonalAccessTokenInfos.clear();
}

PersonalAccessTokenInfo PersonalAccessTokenInfos::at(int index) const
{
    if (index < 0 || index > mPersonalAccessTokenInfos.count()) {
        qCWarning(RUQOLA_LOG) << "Invalid index " << index;
        return {};
    }
    return mPersonalAccessTokenInfos.at(index);
}

bool PersonalAccessTokenInfos::isEmpty() const
{
    return mPersonalAccessTokenInfos.isEmpty();
}

void PersonalAccessTokenInfos::setPersonalAccessTokenInfos(const QVector<PersonalAccessTokenInfo> &tokenInfos)
{
    mPersonalAccessTokenInfos = tokenInfos;
}

void PersonalAccessTokenInfos::parsePersonalAccessTokenInfos(const QJsonObject &obj)
{
    mPersonalAccessTokenInfos.clear();
#if 0
    const QJsonArray permissionArray = obj[str.isEmpty() ? QStringLiteral("update") : str].toArray();
    const auto roleArrayCount = permissionArray.count();
    mPersonalAccessTokenInfos.reserve(roleArrayCount);
    for (auto i = 0; i < roleArrayCount; ++i) {
        Permission r;
        if (r.parsePermission(permissionArray.at(i).toObject(), roleInfo)) {
            if (r.isValid()) {
                mPersonalAccessTokenInfos.append(std::move(r));
            } else {
                qCWarning(RUQOLA_LOG) << "Invalid permission: " << permissionArray.at(i).toObject();
            }
        }
    }
#endif
}
