/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "emoticoncategory.h"
#include "textemoticonscore_export.h"
#include "unicodeemoticon.h"
#include <QObject>
namespace TextEmoticonsCore
{
class UnicodeEmoticonManagerPrivate;
/**
 * @brief The UnicodeEmoticonManager class
 * @author Laurent Montel <montel@kde.org>
 */
class TEXTEMOTICONSCORE_EXPORT UnicodeEmoticonManager : public QObject
{
    Q_OBJECT
public:
    explicit UnicodeEmoticonManager(QObject *parent = nullptr);
    ~UnicodeEmoticonManager() override;

    static UnicodeEmoticonManager *self();

    Q_REQUIRED_RESULT QList<UnicodeEmoticon> unicodeEmojiList() const;

    Q_REQUIRED_RESULT QList<UnicodeEmoticon> emojisForCategory(const QString &category) const;
    Q_REQUIRED_RESULT QList<EmoticonCategory> categories() const;
    Q_REQUIRED_RESULT UnicodeEmoticon unicodeEmoticonForEmoji(const QString &emojiIdentifier) const;
    Q_REQUIRED_RESULT int count() const;

private:
    std::unique_ptr<UnicodeEmoticonManagerPrivate> const d;
};
}
