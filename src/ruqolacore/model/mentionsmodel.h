/*
   Copyright (c) 2019 Montel Laurent <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef MENTIONSMODEL_H
#define MENTIONSMODEL_H
#include "libruqola_private_export.h"
#include "mentions.h"
#include <QAbstractListModel>

class LIBRUQOLACORE_TESTS_EXPORT MentionsModel : public QAbstractListModel
{
public:
    enum MessageRoles {
        Username = Qt::UserRole + 1,
        OriginalMessage,
        MessageConvertedText,
        Timestamp,
        UserId,
        MessageId,
        RoomId,
        UpdatedAt,
        EditedAt,
        EditedByUserName,
        EditedByUserId,
        Alias,
        Avatar,
        Groupable,
        ParseUrls,
        MessageType,
        Attachments,
        Urls,
        Date,
        Starred,
        UsernameUrl,
        Roles,
        Reactions,
    };
    Q_ENUM(MessageRoles)

    explicit MentionsModel(QObject *parent = nullptr);
    ~MentionsModel() override;

    Q_REQUIRED_RESULT Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role) const override;

    void setMentions(const Mentions &mentions);

    Q_REQUIRED_RESULT QHash<int, QByteArray> roleNames() const override;

private:
    Q_DISABLE_COPY(MentionsModel)
    Mentions mMentions;
};

#endif // MENTIONSMODEL_H
