/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT RoomStartDiscussionJob : public RestApiAbstractJob
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

    [[nodiscard]] QByteArray parentRoomId() const;
    void setParentRoomId(const QByteArray &parentRoomId);

    [[nodiscard]] QString discussionName() const;
    void setDiscussionName(const QString &discussionName);

    [[nodiscard]] QByteArray parentMessageId() const;
    void setParentMessageId(const QByteArray &parentMessageId);

    [[nodiscard]] QList<QByteArray> users() const;
    void setUsers(const QList<QByteArray> &value);

    [[nodiscard]] QString replyMessage() const;
    void setReplyMessage(const QString &reply);

    [[nodiscard]] bool encrypted() const;
    void setEncrypted(bool newEncrypted);

Q_SIGNALS:
    void startDiscussionDone(const QJsonObject &replyObject);

protected:
    [[nodiscard]] QString generateErrorMessage(const QString &errorStr) const override;

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QList<QByteArray> mUsers;
    QByteArray mParentRoomId;
    QString mDiscussionName;
    QByteArray mParentMessageId;
    QString mReplyMessage;
    bool mEncrypted = false;
};
}
