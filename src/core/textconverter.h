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
struct LIBRUQOLACORE_EXPORT ConvertMessageTextSettings {
    ConvertMessageTextSettings(const QString &_str,
                               const QString &_userName,
                               const QVector<Message> &_allMessages,
                               const QStringList &_highlightWords,
                               EmojiManager *_emojiManager,
                               MessageCache *_messageCache,
                               const QMap<QString, QString> &_mentions,
                               const QMap<QString, QString> &_channels,
                               const QString &_searchedText = {},
                               int _maximumRecursiveQuotedText = -1)
        : str(_str)
        , userName(_userName)
        , allMessages(_allMessages)
        , highlightWords(_highlightWords)
        , emojiManager(_emojiManager)
        , messageCache(_messageCache)
        , mentions(_mentions)
        , channels(_channels)
        , searchedText(_searchedText)
        , maximumRecursiveQuotedText(_maximumRecursiveQuotedText)
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
    int maximumRecursiveQuotedText = -1;
};

Q_REQUIRED_RESULT LIBRUQOLACORE_EXPORT QString convertMessageText(const ConvertMessageTextSettings &settings,
                                                                  QString &needUpdateMessageId,
                                                                  int &recusiveIndex,
                                                                  bool &hasQuotedText);
}
