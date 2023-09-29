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
class LIBROCKETCHATRESTAPI_QT_TESTS_EXPORT UsersAutocompleteJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    struct LIBROCKETCHATRESTAPI_QT_TESTS_EXPORT UsersAutocompleterInfo {
        [[nodiscard]] bool isValid() const;

        QString pattern;
        QString exception;
    };

    explicit UsersAutocompleteJob(QObject *parent = nullptr);
    ~UsersAutocompleteJob() override;

    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] bool start() override;

    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] UsersAutocompleterInfo usersCompleterInfo() const;
    void setUsersCompleterInfo(const UsersAutocompleterInfo &usersCompleterInfo);

Q_SIGNALS:
    void usersAutocompleteDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(UsersAutocompleteJob)
    void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    UsersAutocompleterInfo mUsersCompleterInfo;
};
}
