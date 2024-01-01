/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "restapiabstractjob.h"

#include <QUrl>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT DownloadFileJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit DownloadFileJob(QObject *parent = nullptr);
    ~DownloadFileJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QUrl url() const;
    void setUrl(const QUrl &url);

    [[nodiscard]] QByteArray mimeType() const;
    void setMimeType(const QByteArray &mimeType);

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QUrl localFileUrl() const;
    void setLocalFileUrl(const QUrl &localFileUrl);

    [[nodiscard]] bool requiredAuthentication() const;
    void setRequiredAuthentication(bool newRequiredAuthentication);

Q_SIGNALS:
    void downloadFileDone(const QUrl &url, const QUrl &localFileUrl);

private:
    Q_DISABLE_COPY(DownloadFileJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void slotDownloadDone();
    QUrl mUrl;
    QByteArray mMimeType;
    QUrl mLocalFileUrl;
    bool mRequiredAuthentication = true;
};
}
