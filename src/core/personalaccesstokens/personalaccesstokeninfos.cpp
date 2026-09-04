/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "personalaccesstokeninfos.h"

#include "ruqola_debug.h"

#include <QJsonArray>
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
PersonalAccessTokenInfos::PersonalAccessTokenInfos() = default;

const QList<PersonalAccessTokenInfo> &PersonalAccessTokenInfos::personalAccessTokenInfos() const
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

const PersonalAccessTokenInfo &PersonalAccessTokenInfos::at(int index) const
{
    if (index < 0 || index >= mPersonalAccessTokenInfos.count()) {
        qCWarning(RUQOLA_LOG) << "Invalid index " << index;
        static const PersonalAccessTokenInfo invalidInfo;
        return invalidInfo;
    }
    return mPersonalAccessTokenInfos.at(index);
}

void PersonalAccessTokenInfos::removeAt(int index)
{
    if (index < 0 || index >= mPersonalAccessTokenInfos.count()) {
        qCWarning(RUQOLA_LOG) << "Invalid index " << index;
        return;
    }
    mPersonalAccessTokenInfos.removeAt(index);
}

bool PersonalAccessTokenInfos::isEmpty() const
{
    return mPersonalAccessTokenInfos.isEmpty();
}

void PersonalAccessTokenInfos::setPersonalAccessTokenInfos(QList<PersonalAccessTokenInfo> tokenInfos)
{
    mPersonalAccessTokenInfos = std::move(tokenInfos);
}

void PersonalAccessTokenInfos::parsePersonalAccessTokenInfos(const QJsonObject &obj)
{
    mPersonalAccessTokenInfos.clear();
    const QJsonArray tokensArray = obj["tokens"_L1].toArray();
    mPersonalAccessTokenInfos.reserve(tokensArray.count());
    for (const QJsonValue &current : tokensArray) {
        const QJsonObject tokenObject = current.toObject();
        PersonalAccessTokenInfo r;
        r.parsePersonalAccessTokenInfo(tokenObject);
        if (r.isValid()) {
            mPersonalAccessTokenInfos.append(std::move(r));
        } else {
            qCWarning(RUQOLA_LOG) << "Invalid personal Access Token Info: " << tokenObject;
        }
    }
}

QDebug operator<<(QDebug d, const PersonalAccessTokenInfos &t)
{
    for (const PersonalAccessTokenInfo &info : t.personalAccessTokenInfos()) {
        d.space() << info << "\n";
    }
    return d;
}

bool PersonalAccessTokenInfos::operator==(const PersonalAccessTokenInfos &other) const
{
    return mPersonalAccessTokenInfos == other.mPersonalAccessTokenInfos;
}
