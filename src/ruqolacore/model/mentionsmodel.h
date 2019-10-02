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

#include <QAbstractListModel>
class Mentions;
class RocketChatAccount;
class TextConverter;
class LIBRUQOLACORE_TESTS_EXPORT MentionsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(bool hasFullList READ hasFullList WRITE setHasFullList NOTIFY hasFullListChanged)
public:
    //Same as messageroles
    enum MessageRoles {
        Username = Qt::UserRole + 1,
        OriginalMessage,
        MessageConvertedText,
        Timestamp,
        UserId,
        SystemMessageType,
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
        CanEditMessage,
        Starred,
        UsernameUrl,
        Roles,
        Reactions,
        Ignored,
        Pinned,
        DiscussionCount,
        DiscussionRoomId,
        DiscussionLastMessage,
        ThreadCount,
        ThreadLastMessage,
        ThreadMessageId,
        ThreadMessagePreview,
        SortByTimeStamp,
        ShowTranslatedMessage,
    };
    Q_ENUM(MessageRoles)

    explicit MentionsModel(RocketChatAccount *account, QObject *parent = nullptr);
    ~MentionsModel() override;

    Q_REQUIRED_RESULT Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role) const override;

    void setMentions(const Mentions &mentions);

    Q_REQUIRED_RESULT QHash<int, QByteArray> roleNames() const override;

    Q_REQUIRED_RESULT QString roomId() const;
    void setRoomId(const QString &roomId);

    void initialize();
    void addMoreMentions(const QJsonObject &mentionsObj);
    void parseMentions(const QJsonObject &mentionsObj, const QString &roomId);
    Mentions *mentions() const;

    Q_REQUIRED_RESULT int total() const;

    Q_REQUIRED_RESULT bool hasFullList() const;

    void setHasFullList(bool state);

    Q_REQUIRED_RESULT bool loadMoreMentionsInProgress() const;
    void setLoadMoreMentionsInProgress(bool loadMoreMentionsInProgress);

Q_SIGNALS:
    void hasFullListChanged();

private:
    Q_DISABLE_COPY(MentionsModel)
    void checkFullList();
    QString convertMessageText(const QString &str, const QString &userName) const;
    QString mRoomId;
    TextConverter *mTextConverter = nullptr;
    RocketChatAccount *mRocketChatAccount = nullptr;
    Mentions *mMentions = nullptr;
    bool mHasFullList = false;
    bool mLoadMoreMentionsInProgress = false;
};

#endif // MENTIONSMODEL_H
