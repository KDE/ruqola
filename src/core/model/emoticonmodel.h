/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "emoticons/customemoji.h"
#include "libruqolacore_export.h"
#include <QAbstractListModel>
#include <TextEmoticonsCore/UnicodeEmoticon>

class RocketChatAccount;
// Model showing all emojis
class LIBRUQOLACORE_EXPORT EmoticonModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum EmoticonsRoles {
        UnicodeEmojiRole = Qt::UserRole + 1,
        CompleterName, // keep value in sync with InputCompleterModel
        IdentifierRole,
        Category,
        IconRole,
    };
    Q_ENUM(EmoticonsRoles)

    explicit EmoticonModel(RocketChatAccount *account, QObject *parent = nullptr);
    ~EmoticonModel() override;

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;

    [[nodiscard]] QList<TextEmoticonsCore::UnicodeEmoticon> unicodeEmoticons() const;

    void setUnicodeEmoticons(const QList<TextEmoticonsCore::UnicodeEmoticon> &emoticons);

    [[nodiscard]] const QVector<CustomEmoji> &customEmojiList() const;
    void setCustomEmojiList(const QVector<CustomEmoji> &newCustomEmojiList);

private:
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT QIcon createCustomIcon(const QString &name) const;
    QList<TextEmoticonsCore::UnicodeEmoticon> mEmoticons;
    QVector<CustomEmoji> mCustomEmojiList;
    // first int is an index into mEmoticons
    // second is -1 for the emoticon identifier or otherwise an index into the alias list
    QVector<QPair<int, int>> mUnicodeRows;
    QVector<QPair<int, int>> mCustomRows;
    RocketChatAccount *const mRocketChatAccount;
};
