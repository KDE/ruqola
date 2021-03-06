/*
   Copyright (c) 2018-2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
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
