/*
   SPDX-FileCopyrightText: 2017-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emoticons/customemoji.h"
#include "utils.h"

#include <QJsonArray>
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
CustomEmoji::CustomEmoji() = default;

CustomEmoji::~CustomEmoji() = default;

bool CustomEmoji::hasEmoji(const QString &identifier) const
{
    return (mEmojiIdentifier == identifier) || mAliases.contains(identifier);
}

qint64 CustomEmoji::updatedAt() const
{
    return mUpdatedAt;
}

void CustomEmoji::setUpdatedAt(qint64 updatedAt)
{
    mUpdatedAt = updatedAt;
}

bool CustomEmoji::isAnimatedImage() const
{
    return mExtension == "gif"_L1;
}

void CustomEmoji::parseEmoji(const QJsonObject &emoji, bool useIsoDate)
{
    mIdentifier = emoji.value("_id"_L1).toString().toLatin1();
    mExtension = emoji.value("extension"_L1).toString();
    mName = emoji.value("name"_L1).toString();
    mEmojiIdentifier = u':' + mName + u':';
    if (useIsoDate) {
        mUpdatedAt = Utils::parseDate(u"_updatedAt"_s, emoji);
    } else {
        mUpdatedAt = Utils::parseIsoDate(u"_updatedAt"_s, emoji);
    }
    const QJsonArray array = emoji.value("aliases"_L1).toArray();
    const auto arrayCount = array.count();
    QStringList lst;
    lst.reserve(arrayCount);
    for (auto i = 0; i < arrayCount; ++i) {
        lst.append(u':' + array.at(i).toString() + u':');
    }
    mAliases = lst;
}

bool CustomEmoji::isValid() const
{
    // Add more check ?
    return !mIdentifier.isEmpty() && !mName.isEmpty();
}

QString CustomEmoji::emojiUrl(const QString &serverUrl) const
{
    QString url = serverUrl + emojiFileName();
    // ???? http ? not https ???
    if (!url.startsWith("http://"_L1) && !url.startsWith("https://"_L1)) {
        url.prepend("http://"_L1);
    }
    return url;
}

QString CustomEmoji::emojiFileName() const
{
    return u"/emoji-custom/%1.%2"_s.arg(mName, mExtension);
}

QString CustomEmoji::generateAnimatedUrlFromCustomEmoji(const QString &serverUrl) const
{
    if (mCachedHtml.isEmpty()) {
        const QString url = emojiUrl(serverUrl);
        // https://rocket.chat/docs/developer-guides/realtime-api/method-calls/list-custom-emoji/#list-custom-emoji
        // http://yourhost.com/emoji-custom/Emoji%20Name.png
        // TODO customize size.
        // Don't store url as cached html
        // mCachedHtml = url;
        return url;
    }
    return mCachedHtml;
}

QString CustomEmoji::generateHtmlFromCustomEmojiLocalPath(const QString &emojiLocalPath) const
{
    if (mCachedHtml.isEmpty()) {
        mCachedHtml = u"<img height='22' width='22' src='%1' title='%2'/>"_s.arg(emojiLocalPath, mEmojiIdentifier);
    }
    return mCachedHtml;
}

void CustomEmoji::clearCachedHtml()
{
    mCachedHtml.clear();
}

QStringList CustomEmoji::aliases() const
{
    return mAliases;
}

void CustomEmoji::setAliases(const QStringList &aliases)
{
    mAliases = aliases;
}

QString CustomEmoji::emojiIdentifier() const
{
    return mEmojiIdentifier;
}

void CustomEmoji::setEmojiIdentifier(const QString &emojiIdentifier)
{
    mEmojiIdentifier = emojiIdentifier;
}

QString CustomEmoji::cachedHtml() const
{
    return mCachedHtml;
}

QByteArray CustomEmoji::identifier() const
{
    return mIdentifier;
}

void CustomEmoji::setIdentifier(const QByteArray &identifier)
{
    mIdentifier = identifier;
}

QString CustomEmoji::extension() const
{
    return mExtension;
}

void CustomEmoji::setExtension(const QString &extension)
{
    mExtension = extension;
}

void CustomEmoji::setName(const QString &name)
{
    mName = name;
}

QString CustomEmoji::name() const
{
    return mName;
}

bool CustomEmoji::operator==(const CustomEmoji &other) const
{
    return (mName == other.name()) && (mExtension == other.extension()) && (mIdentifier == other.identifier()) && (mAliases == other.aliases())
        && (mEmojiIdentifier == other.emojiIdentifier()) && (mUpdatedAt == other.updatedAt());
}

QDebug operator<<(QDebug d, const CustomEmoji &t)
{
    d.space() << "Name:" << t.name();
    d.space() << "Identifier:" << t.identifier();
    d.space() << "extension:" << t.extension();
    d.space() << "aliases:" << t.aliases();
    d.space() << "UpdatedAt:" << t.updatedAt();
    d.space() << "EmojiIdentifier:" << t.emojiIdentifier();
    return d;
}

#include "moc_customemoji.cpp"
