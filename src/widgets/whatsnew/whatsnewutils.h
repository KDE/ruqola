/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class WhatsNewUtils
{
public:
    enum VersionType {
        AllVersion,
        Version2_0,
        Version2_1,
        Version2_2,
        Version2_3,
        LastVersion = Version2_3,
    };

    [[nodiscard]] static QString convertVersionEnumToString(WhatsNewUtils::VersionType type);
};
