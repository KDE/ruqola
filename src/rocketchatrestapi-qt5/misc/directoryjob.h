/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

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
        Room = 1,
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
    void slotDirectoryFinished();
    DirectoryInfo mDirectoryInfo;
};
}
