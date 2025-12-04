/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "textconverter.h"
#include "messagecache.h"
#include "ruqola_texttohtml_debug.h"
#include "ruqolablockcmarksupport.h"
#include "utils.h"

#include <KSyntaxHighlighting/Theme>
#include <TextUtils/TextUtilsSyntaxHighlighter>
#include <TextUtils/TextUtilsSyntaxHighlightingManager>

#include <KColorScheme>
using namespace Qt::Literals::StringLiterals;

static QString convertMessageText2(TextConverter::ConvertMessageTextSettings *settings, int &numberOfTextSearched, int hightLightStringIndex)
{
    RuqolaBlockCMarkSupport cmarkSupport;
    cmarkSupport.setSettings(settings);
    const QString result =
        cmarkSupport.convertMessageText(settings->str, settings->messageId, settings->searchedText, numberOfTextSearched, hightLightStringIndex);
    delete settings;
    return result;
}

QString TextConverter::convertMessageText(const TextConverter::ConvertMessageTextSettings &settings,
                                          QByteArray &needUpdateMessageId,
                                          int &recusiveIndex,
                                          int &numberOfTextSearched,
                                          int hightLightStringIndex)
{
    if (!settings.emojiManager) {
        qCWarning(RUQOLA_TEXTTOHTML_LOG) << "Emojimanager is null";
    }

    QString quotedMessage;

    QString str = settings.str;
    // TODO we need to look at room name too as we can have it when we use "direct reply"
    if (str.contains("[ ](http"_L1)
        && (settings.maximumRecursiveQuotedText == -1 || (settings.maximumRecursiveQuotedText > recusiveIndex))) { // ## is there a better way?
        const int startPos = str.indexOf(u'(');
        const int endPos = str.indexOf(u')');
        const QString url = str.mid(startPos + 1, endPos - startPos - 1);
        // URL example https://HOSTNAME/channel/all?msg=3BR34NSG5x7ZfBa22
        const QByteArray messageId = url.mid(url.indexOf("msg="_L1) + 4).toLatin1();
        // qCDebug(RUQOLA_TEXTTOHTML_LOG) << "Extracted messageId" << messageId;
        auto it = std::find_if(settings.allMessages.cbegin(), settings.allMessages.cend(), [messageId](const Message &msg) {
            return msg.messageId() == messageId;
        });
        if (it != settings.allMessages.cend()) {
            const TextConverter::ConvertMessageTextSettings newSetting(u'@' + (*it).username() + u": "_s + (*it).text(),
                                                                       settings.userName,
                                                                       settings.allMessages,
                                                                       settings.highlightWords,
                                                                       settings.emojiManager,
                                                                       settings.messageCache,
                                                                       (*it).mentions(),
                                                                       (*it).channels(),
                                                                       (*it).messageId(),
                                                                       settings.searchedText,
                                                                       settings.maximumRecursiveQuotedText);
            recusiveIndex++;
            const QString text = TextConverter::convertMessageText(newSetting, needUpdateMessageId, recusiveIndex, numberOfTextSearched, hightLightStringIndex);
            Utils::QuotedRichTextInfo info;
            info.url = url;
            info.richText = text;
            info.displayTime = (*it).dateTime();

            str = str.left(startPos - 3) + str.mid(endPos + 1);
            auto newsettings = new TextConverter::ConvertMessageTextSettings{
                str.toHtmlEscaped(),
                settings.userName,
                settings.allMessages,
                settings.highlightWords,
                settings.emojiManager,
                settings.messageCache,
                settings.mentions,
                settings.channels,
                (*it).messageId(),
                settings.searchedText,
                settings.maximumRecursiveQuotedText,
            };
            str = convertMessageText2(newsettings, numberOfTextSearched, hightLightStringIndex);
            quotedMessage = Utils::formatQuotedRichText(info) + str;
            str.clear();
        } else {
            if (settings.messageCache) {
                // TODO allow to reload index when we loaded message
                Message *msg = settings.messageCache->messageForId(messageId);
                if (msg) {
                    const TextConverter::ConvertMessageTextSettings newSetting(msg->text(),
                                                                               settings.userName,
                                                                               settings.allMessages,
                                                                               settings.highlightWords,
                                                                               settings.emojiManager,
                                                                               settings.messageCache,
                                                                               msg->mentions(),
                                                                               msg->channels(),
                                                                               msg->messageId(),
                                                                               settings.searchedText,
                                                                               settings.maximumRecursiveQuotedText);
                    recusiveIndex++;
                    const QString text =
                        TextConverter::convertMessageText(newSetting, needUpdateMessageId, recusiveIndex, numberOfTextSearched, hightLightStringIndex);
                    Utils::QuotedRichTextInfo info;
                    info.url = url;
                    info.richText = text;
                    info.displayTime = msg->dateTime();
                    auto newsettings = new TextConverter::ConvertMessageTextSettings{
                        str.toHtmlEscaped(),
                        settings.userName,
                        settings.allMessages,
                        settings.highlightWords,
                        settings.emojiManager,
                        settings.messageCache,
                        settings.mentions,
                        settings.channels,
                        msg->messageId(),
                        settings.searchedText,
                        settings.maximumRecursiveQuotedText,
                    };
                    str = convertMessageText2(newsettings, numberOfTextSearched, hightLightStringIndex);

                    quotedMessage = Utils::formatQuotedRichText(info) + str;
                    str.clear();
                } else {
                    qCDebug(RUQOLA_TEXTTOHTML_LOG) << "Quoted message" << messageId << "not found"; // could be a very old one
                    needUpdateMessageId = messageId;
                }
            }
        }
    }

    // Need to escaped text (avoid to interpret html code)
    auto newsettings = new TextConverter::ConvertMessageTextSettings{
        quotedMessage + str.toHtmlEscaped(),
        settings.userName,
        settings.allMessages,
        settings.highlightWords,
        settings.emojiManager,
        settings.messageCache,
        settings.mentions,
        settings.channels,
        settings.messageId,
        settings.searchedText,
        settings.maximumRecursiveQuotedText,
    };

    // qDebug() << "settings.str  " << settings.str;
    const QString result = convertMessageText2(newsettings, numberOfTextSearched, hightLightStringIndex);
    // qDebug() << " RESULT ************ " << result;
    return "<qt>"_L1 + result + "</qt>"_L1;
}
