/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QMap>
#include <QString>
#include <QStringList>
#include <QVector>

class EmojiManager;
class Message;
class MessageCache;

namespace TextConverter
{
struct LIBRUQOLACORE_EXPORT convertMessageTextSettings {
    convertMessageTextSettings(const QString &_str,
                               const QString &_userName,
                               const QVector<Message> &_allMessages,
                               const QStringList &_highlightWords,
                               EmojiManager *_emojiManager,
                               MessageCache *_messageCache,
                               const QMap<QString, QString> &_mentions,
                               const QMap<QString, QString> &_channels,
                               const QString &_searchedText = {})
        : str(_str)
        , userName(_userName)
        , allMessages(_allMessages)
        , highlightWords(_highlightWords)
        , emojiManager(_emojiManager)
        , messageCache(_messageCache)
        , mentions(_mentions)
        , channels(_channels)
        , searchedText(_searchedText)
    {
    }
    QString str;
    QString userName;
    QVector<Message> allMessages;
    QStringList highlightWords;
    EmojiManager *emojiManager = nullptr;
    MessageCache *messageCache = nullptr;
    QMap<QString, QString> mentions;
    QMap<QString, QString> channels;
    QString searchedText;
};

Q_REQUIRED_RESULT LIBRUQOLACORE_EXPORT QString convertMessageText(const QString &str,
                                                                  const QString &userName,
                                                                  const QVector<Message> &allMessages,
                                                                  const QStringList &highlightWords,
                                                                  EmojiManager *emojiManager,
                                                                  MessageCache *messageCache,
                                                                  QString &needUpdateMessageId,
                                                                  const QMap<QString, QString> &mentions,
                                                                  const QMap<QString, QString> &channels,
                                                                  const QString &searchedText = {});
}
