/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT RoomsAutocompleteAvailableForTeamsJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    struct LIBROCKETCHATRESTAPI_QT_EXPORT RoomsAutocompleteChannelAndPrivateInfo {
        [[nodiscard]] bool isValid() const;
        QString name;
        QString exception;
    };

    explicit RoomsAutocompleteAvailableForTeamsJob(QObject *parent = nullptr);
    ~RoomsAutocompleteAvailableForTeamsJob() override;

    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] bool start() override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] RoomsAutocompleteChannelAndPrivateInfo roomsCompleterInfo() const;
    void setRoomsCompleterInfo(const RoomsAutocompleteChannelAndPrivateInfo &roomCompleterInfo);

Q_SIGNALS:
    void roomsAutoCompleteChannelAndPrivateDone(const QJsonObject &obj);

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    RoomsAutocompleteChannelAndPrivateInfo mRoomsAutocompleteInfo;
};
}
