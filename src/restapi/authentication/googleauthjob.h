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

#ifndef GOOGLEAUTHJOB_H
#define GOOGLEAUTHJOB_H
#include "libruqola_private_export.h"
#include "restapiabstractjob.h"

#include <QNetworkRequest>

class LIBRUQOLACORE_TESTS_EXPORT GoogleAuthJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit GoogleAuthJob(QObject *parent = nullptr);
    ~GoogleAuthJob() override;

    bool start() override;
    bool requireHttpAuthentication() const override;

    bool canStart() const override;

    QNetworkRequest request() const override;
    QJsonDocument json() const;

    QString accessToken() const;
    void setAccessToken(const QString &accessToken);

    int expireTokenInSeconds() const;
    void setExpireTokenInSeconds(int expireTokenInSeconds);

    QString idToken() const;
    void setIdToken(const QString &idToken);

Q_SIGNALS:
    void googleauthDone(const QString &authToken, const QString &userId);

private:
    Q_DISABLE_COPY(GoogleAuthJob)
    void slotFacebookauthDone();
    QString mAccessToken;
    QString mIdToken;
    int mExpireTokenInSeconds = -1;
    //TODO identity ???? it's optional
};

#endif // GOOGLEAUTHJOB_H
