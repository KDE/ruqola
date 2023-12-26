/*
   SPDX-FileCopyrightText: 2017-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emoticons/customemoji.h"
#include "utils.h"

#include <QJsonArray>
#include <QJsonObject>

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
    return mExtension == QLatin1String("gif");
}

void CustomEmoji::parseEmoji(const QJsonObject &emoji, bool useIsoDate)
{
    mIdentifier = emoji.value(QLatin1String("_id")).toString();
    mExtension = emoji.value(QLatin1String("extension")).toString();
    mName = emoji.value(QLatin1String("name")).toString();
    mEmojiIdentifier = QLatin1Char(':') + mName + QLatin1Char(':');
    if (useIsoDate) {
        mUpdatedAt = Utils::parseDate(QStringLiteral("_updatedAt"), emoji);
    } else {
        mUpdatedAt = Utils::parseIsoDate(QStringLiteral("_updatedAt"), emoji);
    }
    const QJsonArray array = emoji.value(QLatin1String("aliases")).toArray();
    const auto arrayCount = array.count();
    QStringList lst;
    lst.reserve(arrayCount);
    for (auto i = 0; i < arrayCount; ++i) {
        lst.append(QLatin1Char(':') + array.at(i).toString() + QLatin1Char(':'));
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
    if (!url.startsWith(QLatin1String("http://")) && !url.startsWith(QLatin1String("https://"))) {
        url.prepend(QLatin1String("http://"));
    }
    return url;
}

QString CustomEmoji::emojiFileName() const
{
    return QStringLiteral("/emoji-custom/%1.%2").arg(mName, mExtension);
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
        mCachedHtml = QStringLiteral("<img height='22' width='22' src='%1' title='%2'/>").arg(emojiLocalPath, mEmojiIdentifier);
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

QString CustomEmoji::identifier() const
{
    return mIdentifier;
}

void CustomEmoji::setIdentifier(const QString &identifier)
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
