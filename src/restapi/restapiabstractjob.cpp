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

#include "restapiabstractjob.h"
#include "ruqola_restapi_debug.h"
#include "ruqolalogger.h"

#include <QNetworkRequest>

RestApiAbstractJob::RestApiAbstractJob(QObject *parent)
    : QObject(parent)
{

}

QNetworkAccessManager *RestApiAbstractJob::networkAccessManager() const
{
    return mNetworkAccessManager;
}

void RestApiAbstractJob::setNetworkAccessManager(QNetworkAccessManager *networkAccessManager)
{
    mNetworkAccessManager = networkAccessManager;
}

RestApiMethod *RestApiAbstractJob::restApiMethod() const
{
    return mRestApiMethod;
}

void RestApiAbstractJob::setRestApiMethod(RestApiMethod *restApiMethod)
{
    mRestApiMethod = restApiMethod;
}

QString RestApiAbstractJob::authToken() const
{
    return mAuthToken;
}

void RestApiAbstractJob::setAuthToken(const QString &authToken)
{
    mAuthToken = authToken;
}

QString RestApiAbstractJob::userId() const
{
    return mUserId;
}

void RestApiAbstractJob::setUserId(const QString &userId)
{
    mUserId = userId;
}

bool RestApiAbstractJob::hasAuthenticationValue() const
{
    return !mAuthToken.isEmpty() && !mUserId.isEmpty();
}

bool RestApiAbstractJob::canStart() const
{
    if (!mNetworkAccessManager) {
        qCWarning(RUQOLA_RESTAPI_LOG) << "Network manager not defined";
        return false;
    }
    if (!mRestApiMethod) {
        qCWarning(RUQOLA_RESTAPI_LOG) << "RestaApiMethod not defined";
        return false;
    }
    if (requireHttpAuthentication() && !hasAuthenticationValue()) {
        qCWarning(RUQOLA_RESTAPI_LOG) << "Auth settings is empty. It's a bug";
        return false;
    }
    return true;
}

void RestApiAbstractJob::addAuthRawHeader(QNetworkRequest &request) const
{
    request.setRawHeader(QByteArrayLiteral("X-Auth-Token"), mAuthToken.toLocal8Bit());
    request.setRawHeader(QByteArrayLiteral("X-User-Id"), mUserId.toLocal8Bit());
}

RuqolaLogger *RestApiAbstractJob::ruqolaLogger() const
{
    return mRuqolaLogger;
}

void RestApiAbstractJob::setRuqolaLogger(RuqolaLogger *ruqolaLogger)
{
    mRuqolaLogger = ruqolaLogger;
}

void RestApiAbstractJob::addLoggerInfo(const QByteArray &str)
{
    if (mRuqolaLogger) {
        mRuqolaLogger->dataSent("RESTAPI: " + str);
    } else {
        qCDebug(RUQOLA_RESTAPI_LOG) << "RESTAPI: " << str;
    }
}
