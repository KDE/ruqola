/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "commands.h"
QT_IMPL_METATYPE_EXTERN_TAGGED(Commands, Ruqola_Commands)

#include "downloadappslanguages/downloadappslanguagesmanager.h"
#include "ruqola_commands_debug.h"

#include <QLocale>

using namespace Qt::Literals::StringLiterals;

DownloadAppsLanguagesManager *Commands::downloadManager() const
{
    return mDownloadManager;
}

void Commands::setDownloadManager(DownloadAppsLanguagesManager *downloadManager)
{
    mDownloadManager = downloadManager;
}

void Commands::parseCommands(const QJsonObject &commandsObj)
{
    parseFirstPageCounters(commandsObj);
    parseListCommands(commandsObj);
}

void Commands::parseMoreCommands(const QJsonObject &commandsObj)
{
    parseNextPageCounters(commandsObj);
    parseListCommands(commandsObj);
}

void Commands::parseListCommands(const QJsonObject &commandsObj)
{
    const QJsonArray commandsArray = commandsObj["commands"_L1].toArray();
    mList.reserve(mList.count() + commandsArray.count());
    const QString lang = QLocale().name();
    for (const auto &current : commandsArray) {
        if (current.type() == QJsonValue::Object) {
            const QJsonObject commandObj = current.toObject();
            Command m;
            m.parseCommand(commandObj);
            if (!m.isValid()) {
                qCWarning(RUQOLA_COMMANDS_LOG) << "Invalid command" << commandObj;
                continue;
            }
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
            mList.append(std::move(m));
        } else {
            qCWarning(RUQOLA_COMMANDS_LOG) << "Problem when parsing commands" << current.type();
        }
    }
}

QDebug operator<<(QDebug d, const Commands &t)
{
    d.space() << "total" << t.total();
    d.space() << "offset" << t.offset();
    d.space() << "commandsCount" << t.loadedCount() << "\n";
    for (const Command &command : t.list()) {
        d.space() << command << "\n";
    }
    return d;
}
