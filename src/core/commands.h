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

#ifndef COMMANDS_H
#define COMMANDS_H

#include "command.h"
#include "libruqola_private_export.h"
#include <QVector>
#include <QDebug>

class LIBRUQOLACORE_TESTS_EXPORT Commands
{
public:
    Commands();

    Q_REQUIRED_RESULT bool isEmpty() const;
    void clear();
    Q_REQUIRED_RESULT int count() const;
    Q_REQUIRED_RESULT Command at(int index) const;

    Q_REQUIRED_RESULT int offset() const;
    void setOffset(int offset);

    Q_REQUIRED_RESULT int total() const;
    void setTotal(int total);

    void parseCommands(const QJsonObject &commandsObj);
    void parseMoreCommands(const QJsonObject &fileAttachmentsObj);

    Q_REQUIRED_RESULT QVector<Command> commands() const;
    void setCommands(const QVector<Command> &commands);

    Q_REQUIRED_RESULT int commandsCount() const;
    void setCommandsCount(int commandsCount);

private:
    void parseListCommands(const QJsonObject &commandsObj);
    QVector<Command> mCommands;
    int mCommandsCount = 0;
    int mOffset = 0;
    int mTotal = 0;
};

Q_DECLARE_METATYPE(Commands)
LIBRUQOLACORE_EXPORT QDebug operator <<(QDebug d, const Commands &t);

#endif // COMMANDS_H
