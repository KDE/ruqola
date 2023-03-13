/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "userbasejob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT SetAvatarJob : public UserBaseJob
{
    Q_OBJECT
public:
    struct LIBROCKETCHATRESTAPI_QT5_EXPORT SetAvatarInfo {
        QString mAvatarUrl;
        QUrl mImageUrl;
        Q_REQUIRED_RESULT bool isValid() const;
    };

    explicit SetAvatarJob(QObject *parent = nullptr);
    ~SetAvatarJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT SetAvatarInfo avatarInfo() const;
    void setAvatarInfo(const SetAvatarInfo &avatarInfo);

Q_SIGNALS:
    void setAvatarDone();

protected:
    Q_REQUIRED_RESULT QString errorMessage(const QString &str, const QJsonObject &details) override;

private:
    Q_DISABLE_COPY(SetAvatarJob)
    void slotSetAvatar();
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    SetAvatarInfo mAvatarInfo;
};
}
