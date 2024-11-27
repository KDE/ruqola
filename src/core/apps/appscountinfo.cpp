/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appscountinfo.h"

AppsCountInfo::AppsCountInfo() = default;

AppsCountInfo::~AppsCountInfo() = default;

void AppsCountInfo::parseCountInfo(const QJsonObject &replyObject)
{
    qDebug() << " replyObject " << replyObject;
}

QDebug operator<<(QDebug d, const AppsCountInfo &t)
{
    // TODO
    return d;
}
