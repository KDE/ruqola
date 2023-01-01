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

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT QString parentRoomId() const;
    void setParentRoomId(const QString &parentRoomId);

    Q_REQUIRED_RESULT QString discussionName() const;
    void setDiscussionName(const QString &discussionName);

    Q_REQUIRED_RESULT QString parentMessageId() const;
    void setParentMessageId(const QString &parentMessageId);

    Q_REQUIRED_RESULT QStringList users() const;
    void setUsers(const QStringList &value);

    Q_REQUIRED_RESULT QString replyMessage() const;
    void setReplyMessage(const QString &reply);

Q_SIGNALS:
    void startDiscussionDone();

protected:
    Q_REQUIRED_RESULT QString generateErrorMessage(const QString &errorStr) const override;

private:
    Q_DISABLE_COPY(RoomStartDiscussionJob)
    void onPostRequestResponse(const QJsonDocument &replyJson) override;
    QStringList mUsers;
    QString mParentRoomId;
    QString mDiscussionName;
    QString mParentMessageId;
    QString mReplyMessage;
};
}
