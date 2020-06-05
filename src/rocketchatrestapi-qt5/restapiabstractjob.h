/*
   Copyright (c) 2018-2020 Laurent Montel <montel@kde.org>

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
#include <QNetworkRequest>
#include "librestapi_private_export.h"

class QNetworkAccessManager;
class QNetworkReply;

namespace RocketChatRestApi {
class RestApiMethod;
class AbstractLogger;

class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT QueryParameters
{
public:
    enum class SortOrder {
        Ascendant,
        Descendant,
        NoSorting
    };
    QueryParameters();

    Q_REQUIRED_RESULT int offset() const;
    void setOffset(int offset);

    Q_REQUIRED_RESULT int count() const;
    void setCount(int count);

    Q_REQUIRED_RESULT bool isValid() const;

    Q_REQUIRED_RESULT QMap<QString, SortOrder> sorting() const;
    void setSorting(const QMap<QString, SortOrder> &sorting);

private:
    int mOffset = -1;
    int mCount = -1;
    QMap<QString, SortOrder> mSorting;
};

class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT RestApiAbstractJob : public QObject
{
    Q_OBJECT
public:
    explicit RestApiAbstractJob(QObject *parent = nullptr);
    Q_REQUIRED_RESULT QNetworkAccessManager *networkAccessManager() const;
    void setNetworkAccessManager(QNetworkAccessManager *networkAccessManager);

    Q_REQUIRED_RESULT RocketChatRestApi::RestApiMethod *restApiMethod() const;
    void setRestApiMethod(RocketChatRestApi::RestApiMethod *restApiMethod);

    Q_REQUIRED_RESULT QString authToken() const;
    void setAuthToken(const QString &authToken);

    Q_REQUIRED_RESULT QString userId() const;
    void setUserId(const QString &userId);

    Q_REQUIRED_RESULT bool hasAuthenticationValue() const;

    virtual bool start() = 0;
    virtual bool requireHttpAuthentication() const = 0;

    virtual bool hasQueryParameterSupport() const;

    RocketChatRestApi::AbstractLogger *restApiLogger() const;
    void setRestApiLogger(RocketChatRestApi::AbstractLogger *restApiLogger);

    void addLoggerInfo(const QByteArray &str);
    void addLoggerWarning(const QByteArray &str);
    void addStartRestApiInfo(const QByteArray &str);

    Q_REQUIRED_RESULT virtual bool canStart() const;

    virtual QNetworkRequest request() const = 0;

    Q_REQUIRED_RESULT QueryParameters queryParameters() const;
    void setQueryParameters(const QueryParameters &queryParameters);

    void addQueryParameter(QUrlQuery &urlQuery) const;
    void addRequestAttribute(QNetworkRequest &request) const;
Q_SIGNALS:
    void failed(const QString &str);

protected:
    Q_DISABLE_COPY(RestApiAbstractJob)
    void emitFailedMessage(const QJsonObject &replyObject, QNetworkReply *reply);
    void addAuthRawHeader(QNetworkRequest &request) const;
    virtual Q_REQUIRED_RESULT QString errorMessage(const QString &str);
    virtual Q_REQUIRED_RESULT QString jobName() const;
    Q_REQUIRED_RESULT QNetworkReply *submitGetRequest();
    Q_REQUIRED_RESULT QNetworkReply *submitPostRequest(const QJsonDocument &doc);

    RocketChatRestApi::RestApiMethod *mRestApiMethod = nullptr;
private:
    QueryParameters mQueryParameters;
    QString mAuthToken;
    QString mUserId;
    QNetworkAccessManager *mNetworkAccessManager = nullptr;
    RocketChatRestApi::AbstractLogger *mRestApiLogger = nullptr;

private:
    Q_REQUIRED_RESULT QString errorStr(const QJsonObject &replyObject);
    Q_REQUIRED_RESULT QString generateErrorMessage(const QString &errorStr) const;
};
}
#endif // RESTAPIABSTRACTJOB_H
