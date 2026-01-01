/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT RoomsImagesJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    struct LIBROCKETCHATRESTAPI_QT_EXPORT RoomsImagesJobInfo {
        QByteArray roomId;
        QByteArray startingFromId;
        int offset = 0;
        int count = 0;
        [[nodiscard]] bool isValid() const;
        void generateRequest(QUrlQuery &query) const;
    };

    explicit RoomsImagesJob(QObject *parent = nullptr);
    ~RoomsImagesJob() override;

    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] bool start() override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] bool hasQueryParameterSupport() const override;

    [[nodiscard]] RoomsImagesJobInfo roomsImagesJobInfo() const;
    void setRoomsImagesJobInfo(const RoomsImagesJobInfo &newRoomsImagesJobInfo);

Q_SIGNALS:
    void roomsImagesDone(const QJsonObject &obj);

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    RoomsImagesJobInfo mRoomsImagesJobInfo;
};
}
