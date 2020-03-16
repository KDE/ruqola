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

#include "commandsmodel.h"

CommandsModel::CommandsModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

CommandsModel::~CommandsModel()
{
}

int CommandsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0; // flat model
    }
    return mCommands.count();
}

QVariant CommandsModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mCommands.count()) {
        return {};
    }
    const Command &command = mCommands.at(index.row());
    switch (role) {
    case Description:
        return command.description();
    case Parameters:
        return command.params();
    case CompleterName:
        return command.commandName().mid(1);
    case CommandName:
        return command.commandName();
    case TranslatedDescription:
        return command.translatedDescription();
    case Qt::DisplayRole: // for the completion popup (until we have a delegate)
        return command.commandName();
    }

    return {};
}

QHash<int, QByteArray> CommandsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[CommandName] = QByteArrayLiteral("commandname");
    roles[Description] = QByteArrayLiteral("description");
    roles[Parameters] = QByteArrayLiteral("parameters");
    return roles;
}

Commands CommandsModel::commands() const
{
    return mCommands;
}

void CommandsModel::setCommands(const Commands &commands)
{
    if (rowCount() != 0) {
        beginRemoveRows(QModelIndex(), 0, mCommands.count() - 1);
        mCommands.clear();
        endRemoveRows();
    }
    if (!commands.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, commands.count() - 1);
        mCommands = commands;
        endInsertRows();
    }
}
