/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
#include "personalaccesstokeninfo.h"
#include <QVector>

class LIBRUQOLACORE_EXPORT PersonalAccessTokenInfos
{
public:
    PersonalAccessTokenInfos();

    [[nodiscard]] QVector<PersonalAccessTokenInfo> personalAccessTokenInfos() const;
    void setPersonalAccessTokenInfos(const QVector<PersonalAccessTokenInfo> &tokenInfos);

    void parsePersonalAccessTokenInfos(const QJsonObject &obj);
    [[nodiscard]] bool isEmpty() const;

    [[nodiscard]] int count() const;
    void clear();

    [[nodiscard]] PersonalAccessTokenInfo at(int index) const;

    void removeAt(int index);

private:
    QVector<PersonalAccessTokenInfo> mPersonalAccessTokenInfos;
};
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const PersonalAccessTokenInfos &t);
