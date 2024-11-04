/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "config-ruqola.h"
#include "libruqolacore_export.h"
#include "messages/message.h"
#include <QList>
#include <QMap>
#include <QString>
#include <QStringList>

class EmojiManager;
class Message;
class MessageCache;

namespace TextConverter
{
struct LIBRUQOLACORE_EXPORT ConvertMessageTextSettings {
    ConvertMessageTextSettings(const QString &_str,
                               const QString &_userName,
                               const QList<Message> &_allMessages,
                               const QStringList &_highlightWords,
                               EmojiManager *_emojiManager,
                               MessageCache *_messageCache,
                               const QMap<QString, QByteArray> &_mentions,
                               const Channels *_channels,
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
    const QString str;
    const QString userName;
    const QList<Message> allMessages;
    const QStringList highlightWords;
    EmojiManager *const emojiManager;
    MessageCache *const messageCache;
    const QMap<QString, QByteArray> mentions;
    const Channels *const channels;
    const QString searchedText;
    int maximumRecursiveQuotedText = -1;
};

[[nodiscard]] LIBRUQOLACORE_EXPORT QString convertMessageText(const ConvertMessageTextSettings &settings, QByteArray &needUpdateMessageId, int &recusiveIndex);
#if USE_CMARK_RC_RENDERING_TEXT
[[nodiscard]] LIBRUQOLACORE_EXPORT QString convertMessageTextCMark(const TextConverter::ConvertMessageTextSettings &settings,
                                                                   const QString &quotedMessage = {});
[[nodiscard]] LIBRUQOLACORE_EXPORT QString convertMessageTextCMark(const TextConverter::ConvertMessageTextSettings &settings,
                                                                   QByteArray &needUpdateMessageId,
                                                                   int &recusiveIndex);
#endif
[[nodiscard]] LIBRUQOLACORE_EXPORT QString convertMessageTextRuqola(const ConvertMessageTextSettings &settings,
                                                                    QByteArray &needUpdateMessageId,
                                                                    int &recusiveIndex);
}
