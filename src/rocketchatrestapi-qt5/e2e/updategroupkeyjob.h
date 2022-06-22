/*
   SPDX-FileCopyrightText: 2018-2022 Laurent Montel <montel@kde.org>

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
        Q_REQUIRED_RESULT bool isValid() const;
        QString uid;
        QString roomId;
        // TODO use struct for key ?
        QString key;
    };

    explicit UpdateGroupKeyJob(QObject *parent = nullptr);
    ~UpdateGroupKeyJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT const UpdateGroupKeyInfo &updateGroupInfo() const;
    void setUpdateGroupInfo(const UpdateGroupKeyInfo &newUpdateGroupInfo);

Q_SIGNALS:
    void updateGroupKeyDone();

private:
    Q_DISABLE_COPY(UpdateGroupKeyJob)
    void onPostRequestResponse(const QJsonDocument &replyJson) override;
    UpdateGroupKeyInfo mUpdateGroupInfo;
};
}
