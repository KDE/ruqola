/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appscategoryinfo.h"

AppsCategoryInfo::AppsCategoryInfo() = default;

AppsCategoryInfo::~AppsCategoryInfo() = default;

void AppsCategoryInfo::parseAppsCategoryInfo(const QJsonObject &replyObject)
{
    // TODO
}

QDebug operator<<(QDebug d, const AppsCategoryInfo &t)
{
    // TODO
    return d;
}
