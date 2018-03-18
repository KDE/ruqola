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

#ifndef RESTAPIABSTRACTJOB_H
#define RESTAPIABSTRACTJOB_H

#include <QObject>
#include "libruqola_private_export.h"
class QNetworkAccessManager;
class QNetworkRequest;
class RestApiMethod;
class RuqolaLogger;
class LIBRUQOLACORE_TESTS_EXPORT RestApiAbstractJob : public QObject
{
public:
    explicit RestApiAbstractJob(QObject *parent = nullptr);
    QNetworkAccessManager *networkAccessManager() const;
    void setNetworkAccessManager(QNetworkAccessManager *networkAccessManager);

    RestApiMethod *restApiMethod() const;
    void setRestApiMethod(RestApiMethod *restApiMethod);

    QString authToken() const;
    void setAuthToken(const QString &authToken);

    QString userId() const;
    void setUserId(const QString &userId);

    bool hasAuthenticationValue() const;

    virtual bool start() = 0;
    virtual bool requireHttpAuthentication() const = 0;

    RuqolaLogger *ruqolaLogger() const;
    void setRuqolaLogger(RuqolaLogger *ruqolaLogger);

    void addLoggerInfo(const QByteArray &str);

protected:
    Q_DISABLE_COPY(RestApiAbstractJob)
    bool canStart() const;
    void addAuthRawHeader(QNetworkRequest &request) const;
    QString mAuthToken;
    QString mUserId;
    QNetworkAccessManager *mNetworkAccessManager = nullptr;
    RestApiMethod *mRestApiMethod = nullptr;
    RuqolaLogger *mRuqolaLogger = nullptr;
};

#endif // RESTAPIABSTRACTJOB_H
