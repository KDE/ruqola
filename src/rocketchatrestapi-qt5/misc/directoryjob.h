/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"

#include <QNetworkRequest>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT DirectoryJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit DirectoryJob(QObject *parent = nullptr);
    ~DirectoryJob() override;
    enum SearchType {
        Unknown = 0,
        Rooms = 1,
        Users = 2,
        Teams = 3,
    };
    struct LIBROCKETCHATRESTAPI_QT5_EXPORT DirectoryInfo {
        SearchType searchType = SearchType::Unknown;
        QString pattern;
        Q_REQUIRED_RESULT bool canStart() const;
        Q_REQUIRED_RESULT bool operator==(const DirectoryInfo &other) const;
    };

    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;

    Q_REQUIRED_RESULT bool start() override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT DirectoryInfo directoryInfo() const;
    void setDirectoryInfo(const DirectoryInfo &directoryInfo);

    Q_REQUIRED_RESULT bool hasQueryParameterSupport() const override;

Q_SIGNALS:
    void directoryDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(DirectoryJob)
    void initialQueryParameters();
    void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    DirectoryInfo mDirectoryInfo;
};
}
