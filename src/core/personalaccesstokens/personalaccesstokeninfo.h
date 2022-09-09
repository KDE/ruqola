/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqola_private_export.h"
#include <QDebug>
#include <QJsonObject>
#include <QVector>

class LIBRUQOLACORE_TESTS_EXPORT PersonalAccessTokenInfo
{
public:
    PersonalAccessTokenInfo();
};

Q_DECLARE_METATYPE(PersonalAccessTokenInfo)
Q_DECLARE_TYPEINFO(PersonalAccessTokenInfo, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const PersonalAccessTokenInfo &t);
