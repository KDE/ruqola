/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QPointer>

class QNetworkAccessManager;

namespace RocketChatRestApi
{
class RestApiMethod;
class AbstractLogger;

class LIBROCKETCHATRESTAPI_QT5_EXPORT QueryParameters
{
public:
    enum class SortOrder {
        Ascendant,
        Descendant,
        NoSorting,
    };
    QueryParameters();

    Q_REQUIRED_RESULT int offset() const;
    void setOffset(int offset);

    Q_REQUIRED_RESULT int count() const;
    void setCount(int count);

    Q_REQUIRED_RESULT bool isValid() const;

    Q_REQUIRED_RESULT QMap<QString, SortOrder> sorting() const;
    void setSorting(const QMap<QString, SortOrder> &sorting);

    Q_REQUIRED_RESULT QString type() const;
    void setType(const QString &type);

    Q_REQUIRED_RESULT QMap<QString, QString> custom() const;
    void setCustom(const QMap<QString, QString> &custom);

    static void generateQueryParameter(const QueryParameters &queryParameters, QUrlQuery &urlQuery);

    Q_REQUIRED_RESULT const QString &searchString() const;
    void setSearchString(const QString &newSearchString);

private:
    int mOffset = -1;
    int mCount = -1;
    QMap<QString, SortOrder> mSorting;
    QMap<QString, QString> mCustom;
    QString mType;
    QString mSearchString;
};

class LIBROCKETCHATRESTAPI_QT5_EXPORT RestApiAbstractJob : public QObject
{
    Q_OBJECT
public:
    explicit RestApiAbstractJob(QObject *parent = nullptr);
    ~RestApiAbstractJob() override;
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
    virtual bool requireTwoFactorAuthentication() const;

    virtual bool hasQueryParameterSupport() const;

    void setEnforcePasswordFallback(bool enforce);
    Q_REQUIRED_RESULT bool enforcePasswordFallback() const;

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
    void addRequestAttribute(QNetworkRequest &request, bool addContentTypeHeader = true) const;

    Q_REQUIRED_RESULT const QString &authMethod() const;
    void setAuthMethod(const QString &newAuthMethod);

    Q_REQUIRED_RESULT const QString &authCode() const;
    void setAuthCode(const QString &newAuthCode);

Q_SIGNALS:
    void failed(const QString &str);

protected:
    Q_DISABLE_COPY(RestApiAbstractJob)

    Q_REQUIRED_RESULT QString errorStr(const QJsonObject &replyObject);

    Q_REQUIRED_RESULT QJsonDocument convertToJsonDocument(QNetworkReply *reply);
    void emitFailedMessage(const QJsonObject &replyObject);
    void addAuthRawHeader(QNetworkRequest &request) const;
    Q_REQUIRED_RESULT virtual QString errorMessage(const QString &str, const QJsonObject &detail);
    Q_REQUIRED_RESULT virtual QString jobName() const;
    Q_REQUIRED_RESULT virtual QString generateErrorMessage(const QString &errorStr) const;

    // The main methods used for sending requests to the server
    void submitGetRequest();
    void submitPostRequest(const QJsonDocument &doc);
    void submitDeleteRequest();

    RocketChatRestApi::RestApiMethod *mRestApiMethod = nullptr;
    QPointer<QNetworkReply> mReply;

protected Q_SLOTS:
    virtual void onGetRequestResponse(const QJsonDocument &replyJson)
    {
        Q_UNUSED(replyJson)
    }
    virtual void onPostRequestResponse(const QJsonDocument &replyJson)
    {
        Q_UNUSED(replyJson)
    }
    virtual void onDeleteRequestResponse(const QJsonDocument &replyJson)
    {
        Q_UNUSED(replyJson)
    }

private:
    void genericResponseHandler(void (RestApiAbstractJob::*func)(const QJsonDocument &replyJson));

    QueryParameters mQueryParameters;
    QString mAuthToken;
    QString mUserId;
    QString mAuthMethod;
    QString mAuthCode;
    bool mEnforcePasswordFallBack = true;
    QNetworkAccessManager *mNetworkAccessManager = nullptr;
    RocketChatRestApi::AbstractLogger *mRestApiLogger = nullptr;
};
}
