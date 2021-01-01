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

#ifndef UPLOADFILEJOB_H
#define UPLOADFILEJOB_H

#include <QNetworkRequest>
#include <QObject>
#include <QUrl>
#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi {
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
    explicit UploadFileJob(QObject *parent = nullptr);
    ~UploadFileJob() override;

    Q_REQUIRED_RESULT bool start() override;

    Q_REQUIRED_RESULT bool requireHttpAuthentication() const final;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;
    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT UploadFileInfo uploadFileInfo() const;
    void setUploadFileInfo(const UploadFileInfo &uploadFileInfo);

Q_SIGNALS:
    void uploadProgress(qint64 bytesSent, qint64 bytesTotal);

private:
    Q_DISABLE_COPY(UploadFileJob)
    void slotUploadFinished();
    UploadFileInfo mUploadFileInfo;
};
}
Q_DECLARE_METATYPE(RocketChatRestApi::UploadFileJob::UploadFileInfo)
Q_DECLARE_TYPEINFO(RocketChatRestApi::UploadFileJob::UploadFileInfo, Q_MOVABLE_TYPE);
#endif // UPLOADFILEJOB_H
