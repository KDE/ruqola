/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "command.h"
#include "libruqola_private_export.h"
#include <QDebug>
#include <QVector>
class RocketChatAccount;
class DownloadAppsLanguagesManager;
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

    void parseCommands(const QJsonObject &commandsObj, RocketChatAccount *account);
    void parseMoreCommands(const QJsonObject &commandsObj, RocketChatAccount *account);

    Q_REQUIRED_RESULT QVector<Command> commands() const;
    void setCommands(const QVector<Command> &commands);

    Q_REQUIRED_RESULT int commandsCount() const;
    void setCommandsCount(int commandsCount);

    DownloadAppsLanguagesManager *downloadManager() const;
    void setDownloadManager(DownloadAppsLanguagesManager *downloadManager);

private:
    void parseListCommands(const QJsonObject &commandsObj, RocketChatAccount *account);
    QVector<Command> mCommands;
    int mCommandsCount = 0;
    int mOffset = 0;
    int mTotal = 0;
    DownloadAppsLanguagesManager *mDownloadManager = nullptr;
};

Q_DECLARE_METATYPE(Commands)
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const Commands &t);
