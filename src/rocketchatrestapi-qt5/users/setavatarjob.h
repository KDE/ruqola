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
        [[nodiscard]] bool isValid() const;
    };

    explicit SetAvatarJob(QObject *parent = nullptr);
    ~SetAvatarJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] SetAvatarInfo avatarInfo() const;
    void setAvatarInfo(const SetAvatarInfo &avatarInfo);

Q_SIGNALS:
    void setAvatarDone();

protected:
    [[nodiscard]] QString errorMessage(const QString &str, const QJsonObject &details) override;

private:
    Q_DISABLE_COPY(SetAvatarJob)
    LIBROCKETCHATRESTAPI_QT5_NO_EXPORT void slotSetAvatar();
    LIBROCKETCHATRESTAPI_QT5_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    SetAvatarInfo mAvatarInfo;
};
}
