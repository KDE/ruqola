/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"

#include <QNetworkRequest>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT UsersAutocompleteJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    struct LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT UsersAutocompleterInfo {
        Q_REQUIRED_RESULT bool isValid() const;

        QString pattern;
        QString exception;
    };

    explicit UsersAutocompleteJob(QObject *parent = nullptr);
    ~UsersAutocompleteJob() override;

    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;

    Q_REQUIRED_RESULT bool start() override;

    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT UsersAutocompleterInfo usersCompleterInfo() const;
    void setUsersCompleterInfo(const UsersAutocompleterInfo &usersCompleterInfo);

Q_SIGNALS:
    void usersAutocompleteDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(UsersAutocompleteJob)
    void onGetRequestResponse(const QJsonDocument &replyJson) override;
    UsersAutocompleterInfo mUsersCompleterInfo;
};
}
