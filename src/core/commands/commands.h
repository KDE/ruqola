/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "command.h"
#include "libruqola_private_export.h"
#include <QDebug>
#include <QList>
class DownloadAppsLanguagesManager;
class LIBRUQOLACORE_TESTS_EXPORT Commands
{
public:
    Commands();

    [[nodiscard]] bool isEmpty() const;
    void clear();
    [[nodiscard]] int count() const;
    [[nodiscard]] Command at(int index) const;

    [[nodiscard]] int offset() const;
    void setOffset(int offset);

    [[nodiscard]] int total() const;
    void setTotal(int total);

    void parseCommands(const QJsonObject &commandsObj);
    void parseMoreCommands(const QJsonObject &commandsObj);

    [[nodiscard]] QList<Command> commands() const;
    void setCommands(const QList<Command> &commands);

    [[nodiscard]] int commandsCount() const;
    void setCommandsCount(int commandsCount);

    DownloadAppsLanguagesManager *downloadManager() const;
    void setDownloadManager(DownloadAppsLanguagesManager *downloadManager);

private:
    void parseListCommands(const QJsonObject &commandsObj);
    QList<Command> mCommands;
    int mCommandsCount = 0;
    int mOffset = 0;
    int mTotal = 0;
    DownloadAppsLanguagesManager *mDownloadManager = nullptr;
};

Q_DECLARE_METATYPE(Commands)
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const Commands &t);
