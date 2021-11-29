/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include "librestapi_private_export.h"
#include "restapiabstractjob.h"

#include <QNetworkRequest>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT LoginJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit LoginJob(QObject *parent = nullptr);
    ~LoginJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT QString userName() const;
    void setUserName(const QString &userName);

    Q_REQUIRED_RESULT QString password() const;
    void setPassword(const QString &password);

    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;
    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT QString resume() const;
    void setResume(const QString &resume);

    Q_REQUIRED_RESULT QString code() const;
    void setCode(const QString &code);

Q_SIGNALS:
    void loginDone(const QString &authToken, const QString &userId);

private:
    Q_DISABLE_COPY(LoginJob)
    void slotLoginDone();
    QString mUserName;
    QString mPassword;
    QString mResume;
    QString mCode;
};
}
