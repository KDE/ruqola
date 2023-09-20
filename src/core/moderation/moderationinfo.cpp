/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationinfo.h"

ModerationInfo::ModerationInfo() = default;

QDebug operator<<(QDebug d, const ModerationInfo &t)
{
    // TODO
    return d;
}

bool ModerationInfo::operator==(const ModerationInfo &other) const
{
    // TODO
    return false;
}

void ModerationInfo::parseModerationInfo(const QJsonObject &replyObject)
{
    // TODO
}
