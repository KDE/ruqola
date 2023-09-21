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

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] bool fullExport() const;
    void setFullExport(bool newFullExport);

Q_SIGNALS:
    void userRequestDataDownloadDone(const QString &result);

private:
    Q_DISABLE_COPY(UserRequestDataDownloadJob)
    LIBROCKETCHATRESTAPI_QT5_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    bool mFullExport = false;
};
}
