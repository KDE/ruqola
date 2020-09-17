/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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

#include "ruqola_debug.h"
#include "adminrooms.h"
#include <QJsonArray>
#include <QJsonObject>

AdminRooms::AdminRooms()
{
}

bool AdminRooms::isEmpty() const
{
    return mAdminRooms.isEmpty();
}

void AdminRooms::clear()
{
    mAdminRooms.clear();
}

int AdminRooms::count() const
{
    return mAdminRooms.count();
}

AdminRoom AdminRooms::at(int index) const
{
    return mAdminRooms.at(index);
}

void AdminRooms::parseMoreAdminRooms(const QJsonObject &fileAttachmentsObj)
{
    const int commandsCount = fileAttachmentsObj[QStringLiteral("count")].toInt();
    mOffset = fileAttachmentsObj[QStringLiteral("offset")].toInt();
    mTotal = fileAttachmentsObj[QStringLiteral("total")].toInt();
    parseListAdminRooms(fileAttachmentsObj);
    mAdminRoomsCount += commandsCount;
}

void AdminRooms::parseListAdminRooms(const QJsonObject &commandsObj)
{
//    const QJsonArray commandsArray = commandsObj[QStringLiteral("commands")].toArray();
//    mAdminRooms.reserve(mAdminRooms.count() + commandsArray.count());
//    const QString lang = QLocale().name();
//    for (const QJsonValue &current : commandsArray) {
//        if (current.type() == QJsonValue::Object) {
//            const QJsonObject commandsObj = current.toObject();
//            Command m;
//            m.parseCommand(commandsObj);
//            if (mDownloadManager) {
//                const QString description = mDownloadManager->translatedString(lang, m.description());
//                if (!description.isEmpty()) {
//                    m.setDescription(description);
//                }
//                const QString parameters = mDownloadManager->translatedString(lang, m.params());
//                if (!parameters.isEmpty()) {
//                    m.setParams(parameters);
//                }
//            }
//            mAdminRooms.append(m);
//        } else {
//            qCWarning(RUQOLA_LOG) << "Problem when parsing commands" << current;
//        }
//    }
}

int AdminRooms::adminRoomsCount() const
{
    return mAdminRoomsCount;
}

void AdminRooms::setAdminRoomsCount(int adminroomsCount)
{
    mAdminRoomsCount = adminroomsCount;
}

QVector<AdminRoom> AdminRooms::adminRooms() const
{
    return mAdminRooms;
}

void AdminRooms::setAdminRooms(const QVector<AdminRoom> &commands)
{
    mAdminRooms = commands;
}

void AdminRooms::parseAdminRooms(const QJsonObject &commandsObj)
{
    mAdminRoomsCount = commandsObj[QStringLiteral("count")].toInt();
    mOffset = commandsObj[QStringLiteral("offset")].toInt();
    mTotal = commandsObj[QStringLiteral("total")].toInt();
    mAdminRooms.clear();
    parseListAdminRooms(commandsObj);
}

int AdminRooms::offset() const
{
    return mOffset;
}

void AdminRooms::setOffset(int offset)
{
    mOffset = offset;
}

int AdminRooms::total() const
{
    return mTotal;
}

void AdminRooms::setTotal(int total)
{
    mTotal = total;
}

QDebug operator <<(QDebug d, const AdminRooms &t)
{
    d << "total " << t.total();
    d << "offset " << t.offset();
    d << "commandsCount " << t.adminRoomsCount();
//    for (int i = 0, total = t.commands().count(); i < total; ++i) {
//        d << t.commands().at(i);
//    }
    return d;
}
