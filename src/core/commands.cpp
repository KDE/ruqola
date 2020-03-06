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
#include "commands.h"
#include <QJsonArray>
#include <QJsonObject>

Commands::Commands()
{
}

bool Commands::isEmpty() const
{
    return mCommands.isEmpty();
}

void Commands::clear()
{
    mCommands.clear();
}

int Commands::count() const
{
    return mCommands.count();
}

Command Commands::at(int index) const
{
    return mCommands.at(index);
}

void Commands::parseMoreCommands(const QJsonObject &fileAttachmentsObj)
{
    const int commandsCount = fileAttachmentsObj[QStringLiteral("count")].toInt();
    mOffset = fileAttachmentsObj[QStringLiteral("offset")].toInt();
    mTotal = fileAttachmentsObj[QStringLiteral("total")].toInt();
    parseListCommands(fileAttachmentsObj);
    mCommandsCount += commandsCount;
}

void Commands::parseListCommands(const QJsonObject &commandsObj)
{
    const QJsonArray commandsArray = commandsObj[QStringLiteral("commands")].toArray();
    mCommands.reserve(mCommands.count() + commandsArray.count());
    for (const QJsonValue &current : commandsArray) {
        if (current.type() == QJsonValue::Object) {
            const QJsonObject commandsObj = current.toObject();
            Command m;
            m.parseCommand(commandsObj);
            mCommands.append(m);
        } else {
            qCWarning(RUQOLA_LOG) << "Problem when parsing commands" << current;
        }
    }
}

int Commands::commandsCount() const
{
    return mCommandsCount;
}

void Commands::setCommandsCount(int commandsCount)
{
    mCommandsCount = commandsCount;
}

QVector<Command> Commands::commands() const
{
    return mCommands;
}

void Commands::setCommands(const QVector<Command> &commands)
{
    mCommands = commands;
}

void Commands::parseCommands(const QJsonObject &commandsObj)
{
    mCommandsCount = commandsObj[QStringLiteral("count")].toInt();
    mOffset = commandsObj[QStringLiteral("offset")].toInt();
    mTotal = commandsObj[QStringLiteral("total")].toInt();
    mCommands.clear();
    parseListCommands(commandsObj);
}

int Commands::offset() const
{
    return mOffset;
}

void Commands::setOffset(int offset)
{
    mOffset = offset;
}

int Commands::total() const
{
    return mTotal;
}

void Commands::setTotal(int total)
{
    mTotal = total;
}

QDebug operator <<(QDebug d, const Commands &t)
{
    d << "total " << t.total();
    d << "offset " << t.offset();
    d << "commandsCount " << t.commandsCount();
    for (int i = 0, total = t.commands().count(); i < total; ++i) {
        d << t.commands().at(i);
    }
    return d;
}
