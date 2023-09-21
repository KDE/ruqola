/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT RoomStartDiscussionJob : public RestApiAbstractJob
{
    Q_OBJECT

    // Since rocketchat 1.0.0
public:
    explicit RoomStartDiscussionJob(QObject *parent = nullptr);
    ~RoomStartDiscussionJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] QString parentRoomId() const;
    void setParentRoomId(const QString &parentRoomId);

    [[nodiscard]] QString discussionName() const;
    void setDiscussionName(const QString &discussionName);

    [[nodiscard]] QString parentMessageId() const;
    void setParentMessageId(const QString &parentMessageId);

    [[nodiscard]] QStringList users() const;
    void setUsers(const QStringList &value);

    [[nodiscard]] QString replyMessage() const;
    void setReplyMessage(const QString &reply);

Q_SIGNALS:
    void startDiscussionDone();

protected:
    [[nodiscard]] QString generateErrorMessage(const QString &errorStr) const override;

private:
    Q_DISABLE_COPY(RoomStartDiscussionJob)
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QStringList mUsers;
    QString mParentRoomId;
    QString mDiscussionName;
    QString mParentMessageId;
    QString mReplyMessage;
};
}
