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
class LIBROCKETCHATRESTAPI_QT5_EXPORT GetUsersInRoleJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit GetUsersInRoleJob(QObject *parent = nullptr);
    ~GetUsersInRoleJob() override;

    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] bool start() override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] bool hasQueryParameterSupport() const override;

    [[nodiscard]] const QString &roleId() const;
    void setRoleId(const QString &newRoleId);

    [[nodiscard]] bool canStart() const override;

Q_SIGNALS:
    void getUsersInRoleDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(GetUsersInRoleJob)
    LIBROCKETCHATRESTAPI_QT5_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    LIBROCKETCHATRESTAPI_QT5_NO_EXPORT void initialUrlParameters(QUrlQuery &urlQuery) const;
    QString mRoleId;
};
}
