/*
   Copyright (c) 2017-2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef EMOJI_H
#define EMOJI_H

#include <QString>
#include <QDebug>
#include "libruqola_private_export.h"

class LIBRUQOLACORE_TESTS_EXPORT CustomEmoji
{
    Q_GADGET
public:
    CustomEmoji();
    ~CustomEmoji();

    void parseEmoji(const QJsonObject &emoji);

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
Q_DECLARE_METATYPE(CustomEmoji)
LIBRUQOLACORE_EXPORT QDebug operator <<(QDebug d, const CustomEmoji &t);

#endif // EMOJI_H
