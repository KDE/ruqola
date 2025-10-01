/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emoticons/emojimanager.h"

#include "config-ruqola.h"
#include "rocketchataccount.h"
#include "ruqola_debug.h"
#if HAVE_TEXTEMOTICONSCORE_UNICODEMANAGER_CUSTOM_FILENAME
#include "ruqolaunicodeemoticonmanager.h"
#else
#include <TextEmoticonsCore/UnicodeEmoticonManager>
#endif

#include <QJsonArray>
#include <QJsonObject>
#include <QTextStream>

using namespace Qt::Literals::StringLiterals;
EmojiManager::EmojiManager(RocketChatAccount *account, QObject *parent)
    : QObject(parent)
    , mRocketChatAccount(account)
{
}

EmojiManager::~EmojiManager() = default;

QList<TextEmoticonsCore::UnicodeEmoticon> EmojiManager::unicodeEmojiList() const
{
#if HAVE_TEXTEMOTICONSCORE_UNICODEMANAGER_CUSTOM_FILENAME
    return RuqolaUnicodeEmoticonManager::self()->unicodeEmojiList();
#else
    return TextEmoticonsCore::UnicodeEmoticonManager::self()->unicodeEmojiList();
#endif
}

QList<TextEmoticonsCore::EmoticonCategory> EmojiManager::categories() const
{
#if HAVE_TEXTEMOTICONSCORE_UNICODEMANAGER_CUSTOM_FILENAME
    return RuqolaUnicodeEmoticonManager::self()->categories();
#else
    return TextEmoticonsCore::UnicodeEmoticonManager::self()->categories();
#endif
}

QList<TextEmoticonsCore::UnicodeEmoticon> EmojiManager::emojisForCategory(const QString &category) const
{
#if HAVE_TEXTEMOTICONSCORE_UNICODEMANAGER_CUSTOM_FILENAME
    return RuqolaUnicodeEmoticonManager::self()->emojisForCategory(category);
#else
    return TextEmoticonsCore::UnicodeEmoticonManager::self()->emojisForCategory(category);
#endif
}

void EmojiManager::addUpdateEmojiCustomList(const QJsonArray &arrayEmojiCustomArray)
{
    bool newEmoji = true;
    for (int i = 0; i < arrayEmojiCustomArray.count(); ++i) {
        const QJsonObject obj = arrayEmojiCustomArray.at(i).toObject();
        const QJsonObject customEmojiObj = obj.value("emojiData"_L1).toObject();
        if (!customEmojiObj.isEmpty()) {
            if (customEmojiObj.contains("_id"_L1)) {
                const QByteArray identifier = customEmojiObj.value("_id"_L1).toString().toLatin1();
                for (auto emoji : std::as_const(mCustomEmojiList)) {
                    if (emoji.identifier() == identifier) {
                        mCustomEmojiList.removeAll(emoji);
                        emoji.parseEmoji(customEmojiObj, true);
                        mCustomEmojiList.append(emoji);
                        newEmoji = false;
                        break;
                    }
                }
            }
            //            if (!found) {
            //                // Parse
            //                CustomEmoji newEmoji;
            //                newEmoji.parseEmoji(customEmojiObj, true);
            //                if (newEmoji.isValid()) {
            //                    mCustomEmojiList.append(newEmoji);
            //                }
            //            }
        } else {
            qCWarning(RUQOLA_LOG) << "addUpdateEmojiCustomList invalid QJsonObject" << customEmojiObj;
        }
    }

    // New QJsonArray([{"emojiData":{"_id":"HdN28k4PQ6J9xLkZ8","_updatedAt":{"$date":1631885946222},"aliases":["roo"],"extension":"png","name":"ruqola"}}])
    // Update
    // QJsonArray([{"emojiData":{"_id":"vxE6eG5FrZCvbgM3t","aliases":["rooss"],"extension":"png","name":"xxx","newFile":true,"previousExtension":"png","previousName":"ruqolas"}}
    Q_EMIT customEmojiChanged(newEmoji);
}

void EmojiManager::deleteEmojiCustom(const QJsonArray &arrayEmojiCustomArray)
{
    // ([{"emojiData":{"_id":"PpawhZMaseBcEuGCG","_updatedAt":{"$date":1631858916014},"aliases":[],"extension":"png","name":"ruqolaff"}}])
    const auto count{arrayEmojiCustomArray.count()};
    for (auto i = 0; i < count; ++i) {
        const QJsonObject obj = arrayEmojiCustomArray.at(i).toObject();
        const QJsonObject emojiData = obj.value("emojiData"_L1).toObject();
        const QByteArray identifier = emojiData.value("_id"_L1).toString().toLatin1();
        if (!identifier.isEmpty()) {
            auto it = std::find_if(mCustomEmojiList.cbegin(), mCustomEmojiList.cend(), [identifier](const auto &emoji) {
                return emoji.identifier() == identifier;
            });
            if (it != mCustomEmojiList.cend()) {
                mCustomEmojiList.removeAll(*it);
            }
        }
    }
    Q_EMIT customEmojiChanged(false);
}

void EmojiManager::loadCustomEmoji(const QJsonObject &obj)
{
    mCustomEmojiList.clear();
    const QJsonObject result = obj.value("emojis"_L1).toObject();
    const QJsonArray array = result.value("update"_L1).toArray();
    // TODO add support for remove when we store it in local
    for (int i = 0, total = array.size(); i < total; ++i) {
        const QJsonObject emojiJson = array.at(i).toObject();
        CustomEmoji emoji;
        emoji.parseEmoji(emojiJson);
        if (emoji.isValid()) {
            mCustomEmojiList.append(std::move(emoji));
        }
    }

    // clear cache
    mReplacePatternDirty = true;
}

int EmojiManager::count() const
{
#if HAVE_TEXTEMOTICONSCORE_UNICODEMANAGER_CUSTOM_FILENAME
    return mCustomEmojiList.count() + RuqolaUnicodeEmoticonManager::self()->count();
#else
    return mCustomEmojiList.count() + TextEmoticonsCore::UnicodeEmoticonManager::self()->count();
#endif
}

bool EmojiManager::isAnimatedImage(const QString &emojiIdentifier) const
{
    if (emojiIdentifier.startsWith(u':') && emojiIdentifier.endsWith(u':')) {
        for (int i = 0, total = mCustomEmojiList.size(); i < total; ++i) {
            const CustomEmoji emoji = mCustomEmojiList.at(i);
            if (emoji.hasEmoji(emojiIdentifier)) {
                return emoji.isAnimatedImage();
            }
        }
    }
    return false;
}

TextEmoticonsCore::UnicodeEmoticon EmojiManager::unicodeEmoticonForEmoji(const QString &emojiIdentifier) const
{
#if HAVE_TEXTEMOTICONSCORE_UNICODEMANAGER_CUSTOM_FILENAME
    return RuqolaUnicodeEmoticonManager::self()->unicodeEmoticonForEmoji(emojiIdentifier);
#else
    return TextEmoticonsCore::UnicodeEmoticonManager::self()->unicodeEmoticonForEmoji(emojiIdentifier);
#endif
}

QString EmojiManager::customEmojiFileNameFromIdentifier(const QByteArray &emojiIdentifier) const
{
    for (const CustomEmoji &customEmoji : mCustomEmojiList) {
        if (customEmoji.identifier() == emojiIdentifier) {
            return customEmoji.emojiFileName();
        }
    }
    return {};
}

QString EmojiManager::customEmojiFileName(const QString &emojiIdentifier) const
{
    for (const CustomEmoji &customEmoji : mCustomEmojiList) {
        if (customEmoji.hasEmoji(emojiIdentifier)) {
            return customEmoji.emojiFileName();
        }
    }
    return {};
}

QString EmojiManager::normalizedReactionEmoji(const QString &emojiIdentifier) const
{
    for (const auto &customEmoji : mCustomEmojiList) {
        if (customEmoji.hasEmoji(emojiIdentifier)) {
            return customEmoji.emojiIdentifier();
        }
    }
    const auto unicodeEmojis = unicodeEmojiList();
    for (const auto &unicodeEmoji : unicodeEmojis) {
        if (unicodeEmoji.hasEmoji(emojiIdentifier)) {
            return unicodeEmoji.identifier();
        }
    }
    return emojiIdentifier;
}

QString EmojiManager::replaceEmojiIdentifier(const QString &emojiIdentifier, bool isReaction)
{
    if (mServerUrl.isEmpty()) {
        qCWarning(RUQOLA_LOG) << "Server Url not defined";
        return emojiIdentifier;
    }
    if (mRocketChatAccount && !mRocketChatAccount->ownUserPreferences().convertAsciiEmoji()) {
        return emojiIdentifier;
    }
    if (emojiIdentifier.startsWith(u':') && emojiIdentifier.endsWith(u':')) {
        for (const CustomEmoji &emoji : std::as_const(mCustomEmojiList)) {
            if (emoji.hasEmoji(emojiIdentifier)) {
                QString cachedHtml = emoji.cachedHtml();
                if (cachedHtml.isEmpty()) {
                    // For the moment we can't support animated image as emoticon in text. Only as Reaction.
                    if (emoji.isAnimatedImage() && isReaction) {
                        cachedHtml = emoji.generateAnimatedUrlFromCustomEmoji(mServerUrl);
                    } else {
                        const QString fileName = customEmojiFileName(emojiIdentifier);
                        if (!fileName.isEmpty() && mRocketChatAccount) {
                            const QUrl emojiUrl = mRocketChatAccount->attachmentUrlFromLocalCache(fileName);
                            if (emojiUrl.isEmpty()) {
                                // The download is happening, this will all be updated again later
                            } else {
                                cachedHtml = emoji.generateHtmlFromCustomEmojiLocalPath(emojiUrl.path());
                            }
                        } else {
                            qCDebug(RUQOLA_LOG) << " Impossible to find custom emoji " << emojiIdentifier;
                        }
                    }
                }
                return cachedHtml;
            }
        }
    }

    const TextEmoticonsCore::UnicodeEmoticon unicodeEmoticon = unicodeEmoticonForEmoji(emojiIdentifier);
    if (unicodeEmoticon.isValid()) {
        return unicodeEmoticon.unicodeDisplay();
    }

    return emojiIdentifier;
}

void EmojiManager::replaceEmojis(QString *str)
{
    Q_ASSERT(str);
    if (mReplacePatternDirty) {
        // build a regexp pattern for all the possible emoticons we want to replace
        // i.e. this is going to build a pattern like this:
        // \:smiley\:|\:\-\)|...
        // to optimize it a bit, we use a common pattern that matches most
        // emojis and then we only need to add the other special (ascii) ones
        // otherwise the pattern could become extremely long
        //
        // furthermore, we don't want to replace emojis (esp. non-colon escaped ones) in the
        // middle of another string, such as within a URL or such. at the same time, multiple
        // smileys may come after another...
        const auto commonPattern = ":[\\w\\-]+:"_L1;
        // TODO: use QRegularExpression::anchoredPattern once ruqola depends on Qt 5.15
        static const QRegularExpression common(u'^' + commonPattern + u'$');

        QString pattern;
        QTextStream stream(&pattern);
        // prevent replacements within other strings, use a negative-lookbehind to rule out
        // that we are within some word or link or such
        stream << "(?<![\\w\\-:])";
        // put all other patterns in a non-capturing group
        stream << "(?:";
        stream << commonPattern;

        auto addEmoji = [&](const QString &string) {
            if (common.match(string).hasMatch()) {
                return;
            }
            stream << '|';
            stream << QRegularExpression::escape(string);
        };
        auto addEmojis = [&](const QList<CustomEmoji> &emojis) {
            for (const auto &emoji : emojis) {
                const QByteArray b = emoji.identifier();
                addEmoji(QString::fromLatin1(b));
                const auto aliases = emoji.aliases();
                for (const auto &alias : aliases) {
                    addEmoji(alias);
                }
            }
        };
        auto addUnicodeEmojis = [&](const QList<TextEmoticonsCore::UnicodeEmoticon> &emojis) {
            for (const auto &emoji : emojis) {
                addEmoji(emoji.identifier());
                const auto aliases = emoji.aliases();
                for (const auto &alias : aliases) {
                    addEmoji(alias);
                }
            }
        };
        addEmojis(mCustomEmojiList);
        addUnicodeEmojis(unicodeEmojiList());
        // close non-capturing group
        stream << ")";
        stream.flush();

        mReplacePattern.setPattern(pattern);
        mReplacePattern.optimize();
        mReplacePatternDirty = false;
    }

    if (mReplacePattern.pattern().isEmpty() || !mReplacePattern.isValid()) {
        qCWarning(RUQOLA_LOG) << "invalid emoji replace pattern" << mReplacePattern.pattern() << mReplacePattern.errorString();
        return;
    }

    int offset = 0;
    while (offset < str->size()) {
        const auto match = mReplacePattern.matchView(QStringView(*str), offset);
        if (!match.hasMatch()) {
            break;
        }
        const auto word = match.captured();
        const auto replaceWord = replaceEmojiIdentifier(word);
        str->replace(match.capturedStart(), word.size(), replaceWord);
        offset = match.capturedStart() + replaceWord.size();
    }
}

QString EmojiManager::serverUrl() const
{
    return mServerUrl;
}

void EmojiManager::setServerUrl(const QString &serverUrl)
{
    if (mServerUrl != serverUrl) {
        mServerUrl = serverUrl;
        clearCustomEmojiCachedHtml();
    }
}

void EmojiManager::clearCustomEmojiCachedHtml()
{
    for (int i = 0, total = mCustomEmojiList.size(); i < total; ++i) {
        mCustomEmojiList[i].clearCachedHtml();
    }
}

const QList<CustomEmoji> &EmojiManager::customEmojiList() const
{
    return mCustomEmojiList;
}

#include "moc_emojimanager.cpp"
