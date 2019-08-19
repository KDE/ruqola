/*
   Copyright (c) 2017-2019 Montel Laurent <montel@kde.org>

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

class LIBRUQOLACORE_TESTS_EXPORT Emoji
{
    Q_GADGET
public:
    Emoji();
    ~Emoji();

    void parseEmoji(const QJsonObject &emoji, bool restApi);

    Q_REQUIRED_RESULT QString identifier() const;
    void setIdentifier(const QString &identifier);

    Q_REQUIRED_RESULT QString extension() const;
    void setExtension(const QString &extension);

    void setName(const QString &name);
    Q_REQUIRED_RESULT QString name() const;

    bool operator==(const Emoji &other) const;

    Emoji &operator=(const Emoji &other);

    Q_REQUIRED_RESULT bool isValid() const;

    Q_REQUIRED_RESULT QString generateHtmlFromCustomEmoji(const QString &serverUrl);

    Q_REQUIRED_RESULT QStringList aliases() const;
    void setAliases(const QStringList &aliases);

    Q_REQUIRED_RESULT QString emojiIdentifier() const;
    void setEmojiIdentifier(const QString &emojiIdentifier);

    Q_REQUIRED_RESULT QString cachedHtml() const;

    void clearCachedHtml();
    Q_REQUIRED_RESULT bool hasEmoji(const QString &identifier) const;

    Q_REQUIRED_RESULT qint64 updatedAt() const;
    void setUpdatedAt(const qint64 &updatedAt);

    Q_REQUIRED_RESULT bool isAnimatedImage() const;

    Q_REQUIRED_RESULT QString generateAnimatedUrlFromCustomEmoji(const QString &serverUrl);
private:
    QString mEmojiIdentifier;
    QString mIdentifier;
    QString mExtension;
    QString mName;
    QString mCachedHtml;
    QStringList mAliases;
    qint64 mUpdatedAt = 0;
};
Q_DECLARE_METATYPE(Emoji)
LIBRUQOLACORE_EXPORT QDebug operator <<(QDebug d, const Emoji &t);

#endif // EMOJI_H
