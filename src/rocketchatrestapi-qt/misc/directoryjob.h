/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT DirectoryJob : public RestApiAbstractJob
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
    struct LIBROCKETCHATRESTAPI_QT_EXPORT DirectoryInfo {
        SearchType searchType = SearchType::Unknown;
        QString pattern;
        [[nodiscard]] bool canStart() const;
        [[nodiscard]] bool operator==(const DirectoryInfo &other) const;
    };

    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] bool start() override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] DirectoryInfo directoryInfo() const;
    void setDirectoryInfo(const DirectoryInfo &directoryInfo);

    [[nodiscard]] bool hasQueryParameterSupport() const override;

Q_SIGNALS:
    void directoryDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(DirectoryJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void initialQueryParameters();
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    DirectoryInfo mDirectoryInfo;
};
}
