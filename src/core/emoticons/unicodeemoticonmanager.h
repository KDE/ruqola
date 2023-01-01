/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "emoticoncategory.h"
#include "libruqolacore_export.h"
#include "unicodeemoticon.h"
#include <QObject>

class LIBRUQOLACORE_EXPORT UnicodeEmoticonManager : public QObject
{
    Q_OBJECT
public:
    explicit UnicodeEmoticonManager(QObject *parent = nullptr);
    ~UnicodeEmoticonManager() override;

    static UnicodeEmoticonManager *self();

    Q_REQUIRED_RESULT QVector<UnicodeEmoticon> unicodeEmojiList() const;

    Q_REQUIRED_RESULT QVector<UnicodeEmoticon> emojisForCategory(const QString &category) const;
    Q_REQUIRED_RESULT QVector<EmoticonCategory> categories() const;
    Q_REQUIRED_RESULT UnicodeEmoticon unicodeEmoticonForEmoji(const QString &emojiIdentifier) const;
    Q_REQUIRED_RESULT int count() const;

private:
    Q_REQUIRED_RESULT QString i18nUnicodeCategory(const QString &name) const;
    void loadUnicodeEmoji();
    QVector<UnicodeEmoticon> mUnicodeEmojiList;
};
