/*
   Copyright (c) 2018-2019 Montel Laurent <montel@kde.org>

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

#ifndef EMOJIMANAGER_H
#define EMOJIMANAGER_H

#include <QObject>
#include "emoji.h"
#include "unicodeemoticon.h"
#include "libruqola_private_export.h"

class LIBRUQOLACORE_TESTS_EXPORT EmojiManager : public QObject
{
    Q_OBJECT
public:
    explicit EmojiManager(QObject *parent = nullptr, bool loadUnicode = true);
    ~EmojiManager();

    void loadCustomEmoji(const QJsonObject &obj, bool restApi);

    Q_REQUIRED_RESULT int count() const;

    Q_REQUIRED_RESULT QString replaceEmojiIdentifier(const QString &emojiIdentifier);
    Q_REQUIRED_RESULT QString serverUrl() const;
    void setServerUrl(const QString &serverUrl);

private:
    Q_DISABLE_COPY(EmojiManager)
    void clearCustomEmojiCachedHtml();
    void loadUnicodeEmoji();
    //Use identifier in a QMap ???
    QVector<Emoji> mCustomEmojiList;
    QVector<UnicodeEmoticon> mUnicodeEmojiList;
    QString mServerUrl;
};

#endif // EMOJIMANAGER_H
