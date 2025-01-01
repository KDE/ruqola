/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "commands.h"

#include "downloadappslanguages/downloadappslanguagesmanager.h"
#include "ruqola_commands_debug.h"
#include <QJsonArray>
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
Commands::Commands() = default;

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
    if (index < 0 || index > mCommands.count()) {
        qCWarning(RUQOLA_COMMANDS_LOG) << "Invalid index " << index;
        return {};
    }
    return mCommands.at(index);
}

void Commands::parseMoreCommands(const QJsonObject &commandsObj)
{
    const int commandsCount = commandsObj["count"_L1].toInt();
    mOffset = commandsObj["offset"_L1].toInt();
    mTotal = commandsObj["total"_L1].toInt();
    parseListCommands(commandsObj);
    mCommandsCount += commandsCount;
}

void Commands::parseListCommands(const QJsonObject &commandsObj)
{
    const QJsonArray commandsArray = commandsObj["commands"_L1].toArray();
    mCommands.reserve(mCommands.count() + commandsArray.count());
    const QString lang = QLocale().name();
    for (const QJsonValue &current : commandsArray) {
        if (current.type() == QJsonValue::Object) {
            const QJsonObject commandsObj = current.toObject();
            Command m;
            m.parseCommand(commandsObj);
            if (mDownloadManager) {
                const QString description = mDownloadManager->translatedString(lang, m.description());
                if (!description.isEmpty()) {
                    m.setDescription(description);
                }
                const QString parameters = mDownloadManager->translatedString(lang, m.params());
                if (!parameters.isEmpty()) {
                    m.setParams(parameters);
                }
            }
            mCommands.append(std::move(m));
        } else {
            qCWarning(RUQOLA_COMMANDS_LOG) << "Problem when parsing commands" << current.type();
        }
    }
}

DownloadAppsLanguagesManager *Commands::downloadManager() const
{
    return mDownloadManager;
}

void Commands::setDownloadManager(DownloadAppsLanguagesManager *downloadManager)
{
    mDownloadManager = downloadManager;
}

int Commands::commandsCount() const
{
    return mCommandsCount;
}

void Commands::setCommandsCount(int commandsCount)
{
    mCommandsCount = commandsCount;
}

QList<Command> Commands::commands() const
{
    return mCommands;
}

void Commands::setCommands(const QList<Command> &commands)
{
    mCommands = commands;
}

void Commands::parseCommands(const QJsonObject &commandsObj)
{
    mCommandsCount = commandsObj["count"_L1].toInt();
    mOffset = commandsObj["offset"_L1].toInt();
    mTotal = commandsObj["total"_L1].toInt();
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

QDebug operator<<(QDebug d, const Commands &t)
{
    d.space() << "total" << t.total();
    d.space() << "offset" << t.offset();
    d.space() << "commandsCount" << t.commandsCount() << "\n";
    for (int i = 0, total = t.commands().count(); i < total; ++i) {
        d.space() << t.commands().at(i) << "\n";
    }
    return d;
}
