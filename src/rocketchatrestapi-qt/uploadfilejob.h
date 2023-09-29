/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"
#include <QNetworkRequest>
class QObject;
#include <QUrl>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT UploadFileJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    struct LIBROCKETCHATRESTAPI_QT_EXPORT UploadFileInfo {
        QString roomId;
        QString description;
        QString messageText;
        QString threadMessageId;
        QUrl filenameUrl;
        QString fileName;
        bool deleteTemporaryFile = false;
        [[nodiscard]] bool isValid() const;
    };

    struct LIBROCKETCHATRESTAPI_QT_EXPORT UploadStatusInfo {
        qint64 bytesSent = 0;
        qint64 bytesTotal = 0;
        QString fileName;
    };
    explicit UploadFileJob(QObject *parent = nullptr);
    ~UploadFileJob() override;

    [[nodiscard]] bool start() override;

    [[nodiscard]] bool requireHttpAuthentication() const final;

    [[nodiscard]] QNetworkRequest request() const override;
    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] UploadFileInfo uploadFileInfo() const;
    void setUploadFileInfo(const UploadFileInfo &uploadFileInfo);

    void abort();

Q_SIGNALS:
    void uploadProgress(const RocketChatRestApi::UploadFileJob::UploadStatusInfo &info);
    void uploadFinished();

private:
    Q_DISABLE_COPY(UploadFileJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void slotUploadProgress(qint64 bytesSent, qint64 bytesTotal);
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void slotUploadFinished();
    UploadFileInfo mUploadFileInfo;
};
}
Q_DECLARE_METATYPE(RocketChatRestApi::UploadFileJob::UploadFileInfo)
Q_DECLARE_TYPEINFO(RocketChatRestApi::UploadFileJob::UploadFileInfo, Q_MOVABLE_TYPE);
Q_DECLARE_METATYPE(RocketChatRestApi::UploadFileJob::UploadStatusInfo)
Q_DECLARE_TYPEINFO(RocketChatRestApi::UploadFileJob::UploadStatusInfo, Q_MOVABLE_TYPE);
