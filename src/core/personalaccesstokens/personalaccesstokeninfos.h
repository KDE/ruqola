/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
#include "personalaccesstokeninfo.h"
#include <QJsonObject>
#include <QVector>

class LIBRUQOLACORE_EXPORT PersonalAccessTokenInfos
{
public:
    PersonalAccessTokenInfos();

    Q_REQUIRED_RESULT QVector<PersonalAccessTokenInfo> personalAccessTokenInfos() const;
    void setPersonalAccessTokenInfos(const QVector<PersonalAccessTokenInfo> &tokenInfos);

    void parsePersonalAccessTokenInfos(const QJsonObject &obj);
    Q_REQUIRED_RESULT bool isEmpty() const;

    Q_REQUIRED_RESULT int count() const;
    void clear();

    Q_REQUIRED_RESULT PersonalAccessTokenInfo at(int index) const;

    void removeAt(int index);

private:
    QVector<PersonalAccessTokenInfo> mPersonalAccessTokenInfos;
};
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const PersonalAccessTokenInfos &t);
