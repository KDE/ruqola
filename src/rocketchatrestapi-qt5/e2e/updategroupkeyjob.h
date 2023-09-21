/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT UpdateGroupKeyJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    struct LIBROCKETCHATRESTAPI_QT5_EXPORT UpdateGroupKeyInfo {
        [[nodiscard]] bool isValid() const;
        QString uid;
        QString roomId;
        // TODO use struct for key ?
        QString key;
    };

    explicit UpdateGroupKeyJob(QObject *parent = nullptr);
    ~UpdateGroupKeyJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] const UpdateGroupKeyInfo &updateGroupInfo() const;
    void setUpdateGroupInfo(const UpdateGroupKeyInfo &newUpdateGroupInfo);

Q_SIGNALS:
    void updateGroupKeyDone();

private:
    Q_DISABLE_COPY(UpdateGroupKeyJob)
    LIBROCKETCHATRESTAPI_QT5_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    UpdateGroupKeyInfo mUpdateGroupInfo;
};
}
