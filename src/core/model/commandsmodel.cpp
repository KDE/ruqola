/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "commandsmodel.h"

CommandsModel::CommandsModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

CommandsModel::~CommandsModel() = default;

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
    case TranslatedDescription:
        return command.translatedDescription();
    case TranslatedParams:
        return command.translatedParams();
    case CommandName:
    case Qt::DisplayRole: // for the completion popup (until we have a delegate)
        return command.commandName();
    }

    return {};
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
