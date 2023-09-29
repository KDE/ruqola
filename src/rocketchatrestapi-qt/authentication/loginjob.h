/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "librestapi_private_export.h"
#include "restapiabstractjob.h"

#include <QNetworkRequest>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_TESTS_EXPORT LoginJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit LoginJob(QObject *parent = nullptr);
    ~LoginJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] QString userName() const;
    void setUserName(const QString &userName);

    [[nodiscard]] QString password() const;
    void setPassword(const QString &password);

    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QNetworkRequest request() const override;
    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] QString resume() const;
    void setResume(const QString &resume);

    [[nodiscard]] QString code() const;
    void setCode(const QString &code);

Q_SIGNALS:
    void loginDone(const QString &authToken, const QString &userId);

protected:
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;

private:
    Q_DISABLE_COPY(LoginJob)
    QString mUserName;
    QString mPassword;
    QString mResume;
    QString mCode;
};
}
