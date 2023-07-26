/*
   SPDX-FileCopyrightText: 2017-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqola_private_export.h"
#include <QDebug>
#include <QString>

class LIBRUQOLACORE_TESTS_EXPORT CustomEmoji
{
    Q_GADGET
public:
    CustomEmoji();
    ~CustomEmoji();

    void parseEmoji(const QJsonObject &emoji, bool useIsoDate = false);

    Q_REQUIRED_RESULT QString identifier() const;
    void setIdentifier(const QString &identifier);

    Q_REQUIRED_RESULT QString extension() const;
    void setExtension(const QString &extension);

    void setName(const QString &name);
    Q_REQUIRED_RESULT QString name() const;

    Q_REQUIRED_RESULT bool operator==(const CustomEmoji &other) const;

    CustomEmoji &operator=(const CustomEmoji &other) = default;

    Q_REQUIRED_RESULT bool isValid() const;

    Q_REQUIRED_RESULT QStringList aliases() const;
    void setAliases(const QStringList &aliases);

    Q_REQUIRED_RESULT QString emojiIdentifier() const;
    void setEmojiIdentifier(const QString &emojiIdentifier);

    Q_REQUIRED_RESULT QString cachedHtml() const;

    void clearCachedHtml();
    Q_REQUIRED_RESULT bool hasEmoji(const QString &identifier) const;

    Q_REQUIRED_RESULT qint64 updatedAt() const;
    void setUpdatedAt(qint64 updatedAt);

    Q_REQUIRED_RESULT bool isAnimatedImage() const;

    Q_REQUIRED_RESULT QString emojiUrl(const QString &serverUrl) const;
    Q_REQUIRED_RESULT QString emojiFileName() const;

    Q_REQUIRED_RESULT QString generateAnimatedUrlFromCustomEmoji(const QString &serverUrl) const;
    Q_REQUIRED_RESULT QString generateHtmlFromCustomEmojiLocalPath(const QString &emojoLocalPath) const;

private:
    QString mEmojiIdentifier;
    QString mIdentifier;
    QString mExtension;
    QString mName;
    mutable QString mCachedHtml;
    QStringList mAliases;
    qint64 mUpdatedAt = 0;
};
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
Q_DECLARE_METATYPE(CustomEmoji)
#endif
Q_DECLARE_TYPEINFO(CustomEmoji, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const CustomEmoji &t);
