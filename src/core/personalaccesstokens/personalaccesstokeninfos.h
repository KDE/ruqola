/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
#include "personalaccesstokeninfo.h"
#include <QList>

class LIBRUQOLACORE_EXPORT PersonalAccessTokenInfos
{
public:
    PersonalAccessTokenInfos();

    [[nodiscard]] QList<PersonalAccessTokenInfo> personalAccessTokenInfos() const;
    void setPersonalAccessTokenInfos(const QList<PersonalAccessTokenInfo> &tokenInfos);

    void parsePersonalAccessTokenInfos(const QJsonObject &obj);
    [[nodiscard]] bool isEmpty() const;

    [[nodiscard]] int count() const;
    void clear();

    [[nodiscard]] PersonalAccessTokenInfo at(int index) const;

    void removeAt(int index);

private:
    QList<PersonalAccessTokenInfo> mPersonalAccessTokenInfos;
};
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const PersonalAccessTokenInfos &t);
