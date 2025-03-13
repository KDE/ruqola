/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "personalaccesstokeninfos.h"

#include "ruqola_debug.h"

#include <QJsonArray>
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
PersonalAccessTokenInfos::PersonalAccessTokenInfos() = default;

QList<PersonalAccessTokenInfo> PersonalAccessTokenInfos::personalAccessTokenInfos() const
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

void PersonalAccessTokenInfos::removeAt(int index)
{
    mPersonalAccessTokenInfos.removeAt(index);
}

bool PersonalAccessTokenInfos::isEmpty() const
{
    return mPersonalAccessTokenInfos.isEmpty();
}

void PersonalAccessTokenInfos::setPersonalAccessTokenInfos(const QList<PersonalAccessTokenInfo> &tokenInfos)
{
    mPersonalAccessTokenInfos = tokenInfos;
}

void PersonalAccessTokenInfos::parsePersonalAccessTokenInfos(const QJsonObject &obj)
{
    mPersonalAccessTokenInfos.clear();
    const QJsonArray tokensArray = obj["tokens"_L1].toArray();
    const auto tokensArrayCount = tokensArray.count();
    mPersonalAccessTokenInfos.reserve(tokensArrayCount);
    for (auto i = 0; i < tokensArrayCount; ++i) {
        PersonalAccessTokenInfo r;
        r.parsePersonalAccessTokenInfo(tokensArray.at(i).toObject());
        if (r.isValid()) {
            mPersonalAccessTokenInfos.append(std::move(r));
        } else {
            qCWarning(RUQOLA_LOG) << "Invalid personal Access Token Info: " << tokensArray.at(i).toObject();
        }
    }
}

QDebug operator<<(QDebug d, const PersonalAccessTokenInfos &t)
{
    for (int i = 0, total = t.personalAccessTokenInfos().count(); i < total; ++i) {
        d.space() << t.personalAccessTokenInfos().at(i) << "\n";
    }
    return d;
}

bool PersonalAccessTokenInfos::operator==(const PersonalAccessTokenInfos &other) const
{
    return personalAccessTokenInfos() == other.personalAccessTokenInfos();
}
