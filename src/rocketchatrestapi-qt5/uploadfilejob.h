/*
   SPDX-FileCopyrightText: 2018-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"
#include <QNetworkRequest>
#include <QObject>
#include <QUrl>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT UploadFileJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    struct LIBROCKETCHATRESTAPI_QT5_EXPORT UploadFileInfo {
        QString roomId;
        QString description;
        QString messageText;
        QString threadMessageId;
        QUrl filenameUrl;
        Q_REQUIRED_RESULT bool isValid() const;
    };

    struct LIBROCKETCHATRESTAPI_QT5_EXPORT UploadStatusInfo {
        qint64 bytesSent = 0;
        qint64 bytesTotal = 0;
        QString fileName;
    };
    explicit UploadFileJob(QObject *parent = nullptr);
    ~UploadFileJob() override;

    Q_REQUIRED_RESULT bool start() override;

    Q_REQUIRED_RESULT bool requireHttpAuthentication() const final;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;
    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT UploadFileInfo uploadFileInfo() const;
    void setUploadFileInfo(const UploadFileInfo &uploadFileInfo);

    void abort();

Q_SIGNALS:
    void uploadProgress(const RocketChatRestApi::UploadFileJob::UploadStatusInfo &info);

private:
    Q_DISABLE_COPY(UploadFileJob)
    void slotUploadProgress(qint64 bytesSent, qint64 bytesTotal);
    void slotUploadFinished();
    UploadFileInfo mUploadFileInfo;
};
}
Q_DECLARE_METATYPE(RocketChatRestApi::UploadFileJob::UploadFileInfo)
Q_DECLARE_TYPEINFO(RocketChatRestApi::UploadFileJob::UploadFileInfo, Q_MOVABLE_TYPE);
Q_DECLARE_METATYPE(RocketChatRestApi::UploadFileJob::UploadStatusInfo)
Q_DECLARE_TYPEINFO(RocketChatRestApi::UploadFileJob::UploadStatusInfo, Q_MOVABLE_TYPE);
