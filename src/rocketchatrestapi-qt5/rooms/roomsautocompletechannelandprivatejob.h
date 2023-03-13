/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"

#include <QNetworkRequest>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT RoomsAutocompleteChannelAndPrivateJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    struct LIBROCKETCHATRESTAPI_QT5_EXPORT RoomsAutocompleteChannelAndPrivateInfo {
        Q_REQUIRED_RESULT bool isValid() const;

        QString name;
        QString exception;
    };

    explicit RoomsAutocompleteChannelAndPrivateJob(QObject *parent = nullptr);
    ~RoomsAutocompleteChannelAndPrivateJob() override;

    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;

    Q_REQUIRED_RESULT bool start() override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT RoomsAutocompleteChannelAndPrivateInfo roomsCompleterInfo() const;
    void setRoomsCompleterInfo(const RoomsAutocompleteChannelAndPrivateInfo &roomCompleterInfo);

Q_SIGNALS:
    void roomsAutoCompleteChannelAndPrivateDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(RoomsAutocompleteChannelAndPrivateJob)
    void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    RoomsAutocompleteChannelAndPrivateInfo mRoomsAutocompleteInfo;
};
}
