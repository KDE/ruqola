/*
   Copyright (c) 2019-2021 Laurent Montel <montel@kde.org>

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

#pragma once

#include "libruqolacore_export.h"
#include "messagemodel.h"

class LIBRUQOLACORE_EXPORT ListMessagesModel : public MessageModel
{
    Q_OBJECT
public:
    enum ListMessageType {
        Unknown = 0,
        StarredMessages,
        SnipperedMessages,
        PinnedMessages,
        MentionsMessages,
        ThreadsMessages,
        UnreadThreadsMessages,
    };
    Q_ENUM(ListMessageType)

    explicit ListMessagesModel(const QString &roomID = QStringLiteral("no_room"),
                               RocketChatAccount *account = nullptr,
                               Room *room = nullptr,
                               QObject *parent = nullptr);
    ~ListMessagesModel() override;

    void parseListMessages(const QJsonObject &obj);
    void loadMoreListMessages(const QJsonObject &obj);

    Q_REQUIRED_RESULT int total() const;
    void setTotal(int total);

    Q_REQUIRED_RESULT bool loadMoreListMessagesInProgress() const;
    void setLoadMoreListMessagesInProgress(bool inProgress);

    void setHasFullList(bool state);
    Q_REQUIRED_RESULT bool hasFullList() const;

    Q_REQUIRED_RESULT ListMessageType listMessageType() const;
    void setListMessageType(ListMessagesModel::ListMessageType listMessageType);

Q_SIGNALS:
    void hasFullListChanged();
    void listMessageTypeChanged();
    void totalChanged();
    void loadingInProgressChanged();

private:
    void parse(const QJsonObject &obj);
    void checkFullList();
    int mTotal = 0;
    bool mLoadingInProgress = false;
    bool mHasFullList = false;
    ListMessageType mListMessageType = Unknown;
};

