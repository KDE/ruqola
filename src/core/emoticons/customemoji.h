/*
   SPDX-FileCopyrightText: 2017-2024 Laurent Montel <montel@kde.org>

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

    [[nodiscard]] QString identifier() const;
    void setIdentifier(const QString &identifier);

    [[nodiscard]] QString extension() const;
    void setExtension(const QString &extension);

    void setName(const QString &name);
    [[nodiscard]] QString name() const;

    [[nodiscard]] bool operator==(const CustomEmoji &other) const;

    CustomEmoji &operator=(const CustomEmoji &other) = default;

    [[nodiscard]] bool isValid() const;

    [[nodiscard]] QStringList aliases() const;
    void setAliases(const QStringList &aliases);

    [[nodiscard]] QString emojiIdentifier() const;
    void setEmojiIdentifier(const QString &emojiIdentifier);

    [[nodiscard]] QString cachedHtml() const;

    void clearCachedHtml();
    [[nodiscard]] bool hasEmoji(const QString &identifier) const;

    [[nodiscard]] qint64 updatedAt() const;
    void setUpdatedAt(qint64 updatedAt);

    [[nodiscard]] bool isAnimatedImage() const;

    [[nodiscard]] QString emojiUrl(const QString &serverUrl) const;
    [[nodiscard]] QString emojiFileName() const;

    [[nodiscard]] QString generateAnimatedUrlFromCustomEmoji(const QString &serverUrl) const;
    [[nodiscard]] QString generateHtmlFromCustomEmojiLocalPath(const QString &emojoLocalPath) const;

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
