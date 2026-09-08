/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "command.h"
#include "libruqola_private_export.h"
#include "paginatedinfolist.h"
class QDebug;
class DownloadAppsLanguagesManager;

class LIBRUQOLACORE_TESTS_EXPORT Commands : public PaginatedInfoList<Command, &Command::parseCommand>
{
public:
    void parseCommands(const QJsonObject &commandsObj);
    void parseMoreCommands(const QJsonObject &commandsObj);

    [[nodiscard]] DownloadAppsLanguagesManager *downloadManager() const;
    void setDownloadManager(DownloadAppsLanguagesManager *downloadManager);

private:
    // Commands drop the invalid entries and get their description translated, so they don't go
    // through PaginatedInfoList::parseElements().
    LIBRUQOLACORE_NO_EXPORT void parseListCommands(const QJsonObject &commandsObj);
    DownloadAppsLanguagesManager *mDownloadManager = nullptr;
};

QT_DECL_METATYPE_EXTERN_TAGGED(Commands, Ruqola_Commands, LIBRUQOLACORE_EXPORT)
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const Commands &t);
