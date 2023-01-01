/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "emoticons/customemoji.h"
#include "libruqolacore_export.h"
#include <QAbstractListModel>

class RocketChatAccount;
class LIBRUQOLACORE_EXPORT EmoticonCustomModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum EmoticonsRoles {
        EmojiName = Qt::UserRole + 1,
        Identifier,
    };
    Q_ENUM(EmoticonsRoles)

    explicit EmoticonCustomModel(RocketChatAccount *account, QObject *parent = nullptr);
    ~EmoticonCustomModel() override;

    Q_REQUIRED_RESULT int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role) const override;

    Q_REQUIRED_RESULT const QVector<CustomEmoji> &customEmojiList() const;
    void setCustomEmojiList(const QVector<CustomEmoji> &newCustomEmojiList);

private:
    Q_DISABLE_COPY(EmoticonCustomModel)
    Q_REQUIRED_RESULT QIcon generateIcon(const QString &name) const;
    QVector<CustomEmoji> mCustomEmojiList;
    // first int is an index into mEmoticons
    // second is -1 for the emoticon identifier or otherwise an index into the alias list
    QVector<QPair<int, int>> mCustomRows;
    RocketChatAccount *const mRocketChatAccount;
};
