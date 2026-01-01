/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "commandsmodel.h"
using namespace Qt::Literals::StringLiterals;

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
    case Permissions:
        return command.permissions();
    }

    return {};
}

Commands CommandsModel::commands() const
{
    return mCommands;
}

void CommandsModel::clear()
{
    if (!mCommands.isEmpty()) {
        beginResetModel();
        mCommands.clear();
        endResetModel();
    }
}

void CommandsModel::setCommands(const Commands &commands)
{
    clear();
    if (!commands.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, commands.count() - 1);
        mCommands = commands;
        endInsertRows();
    }
}

bool CommandsModel::commandHasPreview(const QString &commandName) const
{
    const auto commands = mCommands.commands();
    const auto index = std::find_if(commands.begin(), commands.end(), [commandName](const Command &command) {
        return (command.providesPreview() && (command.commandName() == u'/' + commandName));
    });
    return (index != commands.cend());
}

#include "moc_commandsmodel.cpp"
