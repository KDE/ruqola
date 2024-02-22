/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QString>

namespace PersonalAccessTokenPluginUtil
{
struct PersonalAccessTokenPluginInfo {
    QString userId;
    QString token;
    QString serverUrl;
    QString accountName;
    [[nodiscard]] bool isEmpty() const;
    [[nodiscard]] bool isValid() const;
};
};
