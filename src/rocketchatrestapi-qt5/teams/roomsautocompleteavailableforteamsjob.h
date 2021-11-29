/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"

#include <QNetworkRequest>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT RoomsAutocompleteAvailableForTeamsJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    struct LIBROCKETCHATRESTAPI_QT5_EXPORT RoomsAutocompleteChannelAndPrivateInfo {
        Q_REQUIRED_RESULT bool isValid() const;
        QString name;
        QString exception;
    };

    explicit RoomsAutocompleteAvailableForTeamsJob(QObject *parent = nullptr);
    ~RoomsAutocompleteAvailableForTeamsJob() override;

    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;

    Q_REQUIRED_RESULT bool start() override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT RoomsAutocompleteChannelAndPrivateInfo roomsCompleterInfo() const;
    void setRoomsCompleterInfo(const RoomsAutocompleteChannelAndPrivateInfo &roomCompleterInfo);

Q_SIGNALS:
    void roomsAutoCompleteChannelAndPrivateDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(RoomsAutocompleteAvailableForTeamsJob)
    void slotRoomsAutoCompleteChannelAndPrivateFinished();
    RoomsAutocompleteChannelAndPrivateInfo mRoomsAutocompleteInfo;
};
}
