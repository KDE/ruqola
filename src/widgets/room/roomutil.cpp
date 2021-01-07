/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "roomutil.h"
#include "ruqolawidgets_debug.h"
QString RoomUtil::generateUserLink(const QString &userName)
{
    return QStringLiteral("ruqola:/user/") + userName;
}

QString RoomUtil::generatePermalink(const QString &messageId, const QString &roomId, const QString &channelType)
{
    QString prefix;
    if (channelType == QLatin1Char('c')) {
        prefix = QStringLiteral("channel/");
    } else if (channelType == QLatin1Char('d')) {
        prefix = QStringLiteral("direct/");
    } else if (channelType == QLatin1Char('p')) {
        prefix = QStringLiteral("group/");
    } else {
        qCWarning(RUQOLAWIDGETS_LOG) << " channel type undefined " << channelType;
    }
    const QString result = QStringLiteral("%1%2?msg=%3").arg(prefix, roomId, messageId);
    return result;
}
