/*
   Copyright (c) 2018 Montel Laurent <montel@kde.org>

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

#ifndef TWITTERAUTHJOB_H
#define TWITTERAUTHJOB_H
#include "libruqola_private_export.h"
#include "restapiabstractjob.h"

#include <QNetworkRequest>

class LIBRUQOLACORE_TESTS_EXPORT TwitterAuthJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit TwitterAuthJob(QObject *parent = nullptr);
    ~TwitterAuthJob() override;

    bool start() override;
    bool requireHttpAuthentication() const override;

    bool canStart() const override;

    QNetworkRequest request() const override;
    QJsonDocument json() const;

    QString accessToken() const;
    void setAccessToken(const QString &accessToken);

    QString secret() const;
    void setSecret(const QString &secret);

    int expireTokenInSeconds() const;
    void setExpireTokenInSeconds(int expireTokenInSeconds);

    QString appSecret() const;
    void setAppSecret(const QString &appSecret);

    QString appId() const;
    void setAppId(const QString &appId);

Q_SIGNALS:
    void twitterDone(const QString &authToken, const QString &userId);

private:
    Q_DISABLE_COPY(TwitterAuthJob)
    void slotTwitterauthDone();
    QString mAccessToken;
    QString mSecret;
    QString mAppSecret;
    QString mAppId;
    int mExpireTokenInSeconds = -1;
    //TODO identity ???? it's optional
};

#endif // TWITTERAUTHJOB_H
