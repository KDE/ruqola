/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"

#include <QNetworkRequest>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT UserRequestDataDownloadJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit UserRequestDataDownloadJob(QObject *parent = nullptr);
    ~UserRequestDataDownloadJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT bool fullExport() const;
    void setFullExport(bool newFullExport);

Q_SIGNALS:
    void userRequestDataDownloadDone(const QString &result);

private:
    Q_DISABLE_COPY(UserRequestDataDownloadJob)
    void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    bool mFullExport = false;
};
}
