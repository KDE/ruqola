/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "restapiabstractjob.h"

#include <QUrl>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT DownloadFileJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit DownloadFileJob(QObject *parent = nullptr);
    ~DownloadFileJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QUrl url() const;
    void setUrl(const QUrl &url);

    Q_REQUIRED_RESULT QString mimeType() const;
    void setMimeType(const QString &mimeType);

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QUrl localFileUrl() const;
    void setLocalFileUrl(const QUrl &localFileUrl);

Q_SIGNALS:
    void downloadFileDone(const QUrl &url, const QUrl &localFileUrl);

private:
    Q_DISABLE_COPY(DownloadFileJob)
    void slotDownloadDone();
    QUrl mUrl;
    QString mMimeType;
    QUrl mLocalFileUrl;
};
}
