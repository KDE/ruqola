/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "messagesmodel.h"

class LIBRUQOLACORE_EXPORT ListMessagesModel : public MessagesModel
{
    Q_OBJECT
public:
    enum ListMessageType {
        Unknown = 0,
        StarredMessages,
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

    [[nodiscard]] int total() const;
    void setTotal(int total);

    [[nodiscard]] bool loadMoreListMessagesInProgress() const;
    void setLoadMoreListMessagesInProgress(bool inProgress);

    void setHasFullList(bool state);
    [[nodiscard]] bool hasFullList() const;

    [[nodiscard]] ListMessageType listMessageType() const;
    void setListMessageType(ListMessagesModel::ListMessageType listMessageType);

Q_SIGNALS:
    void hasFullListChanged();
    void listMessageTypeChanged();
    void totalChanged();
    void loadingInProgressChanged();

private:
    LIBRUQOLACORE_NO_EXPORT void parse(const QJsonObject &obj);
    int mTotal = -1;
    bool mLoadingInProgress = false;
    bool mHasFullList = false;
    ListMessageType mListMessageType = Unknown;
};
