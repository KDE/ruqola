/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QPointer>

class QNetworkAccessManager;

namespace RocketChatRestApi
{
class RestApiMethod;
class AbstractLogger;

class LIBROCKETCHATRESTAPI_QT_EXPORT QueryParameters
{
public:
    enum class SortOrder {
        Ascendant,
        Descendant,
        NoSorting,
    };
    QueryParameters();

    [[nodiscard]] int offset() const;
    void setOffset(int offset);

    [[nodiscard]] int count() const;
    void setCount(int count);

    [[nodiscard]] bool isValid() const;

    [[nodiscard]] QMap<QString, SortOrder> sorting() const;
    void setSorting(const QMap<QString, SortOrder> &sorting);

    [[nodiscard]] QString type() const;
    void setType(const QString &type);

    [[nodiscard]] QMap<QString, QString> custom() const;
    void setCustom(const QMap<QString, QString> &custom);

    static void generateQueryParameter(const QueryParameters &queryParameters, QUrlQuery &urlQuery);

    [[nodiscard]] const QString &searchString() const;
    void setSearchString(const QString &newSearchString);

    void setFilter(const QString &filter);
    [[nodiscard]] QString filter() const;

private:
    int mOffset = -1;
    int mCount = -1;
    QMap<QString, SortOrder> mSorting;
    QMap<QString, QString> mCustom;
    QString mType;
    QString mSearchString;
    QString mFilter;
};

class LIBROCKETCHATRESTAPI_QT_EXPORT RestApiAbstractJob : public QObject
{
    Q_OBJECT
public:
    explicit RestApiAbstractJob(QObject *parent = nullptr);
    ~RestApiAbstractJob() override;
    [[nodiscard]] QNetworkAccessManager *networkAccessManager() const;
    void setNetworkAccessManager(QNetworkAccessManager *networkAccessManager);

    [[nodiscard]] RocketChatRestApi::RestApiMethod *restApiMethod() const;
    void setRestApiMethod(RocketChatRestApi::RestApiMethod *restApiMethod);

    [[nodiscard]] QString authToken() const;
    void setAuthToken(const QString &authToken);

    [[nodiscard]] QString userId() const;
    void setUserId(const QString &userId);

    [[nodiscard]] bool hasAuthenticationValue() const;

    virtual bool start() = 0;
    virtual bool requireHttpAuthentication() const = 0;
    virtual bool requireTwoFactorAuthentication() const;

    virtual bool hasQueryParameterSupport() const;

    void setEnforcePasswordFallback(bool enforce);
    [[nodiscard]] bool enforcePasswordFallback() const;

    RocketChatRestApi::AbstractLogger *restApiLogger() const;
    void setRestApiLogger(RocketChatRestApi::AbstractLogger *restApiLogger);

    void addLoggerInfo(const QByteArray &str);
    void addLoggerWarning(const QByteArray &str);
    void addStartRestApiInfo(const QByteArray &str);

    [[nodiscard]] virtual bool canStart() const;

    virtual QNetworkRequest request() const = 0;

    [[nodiscard]] QueryParameters queryParameters() const;
    void setQueryParameters(const QueryParameters &queryParameters);

    void addQueryParameter(QUrlQuery &urlQuery) const;
    void addRequestAttribute(QNetworkRequest &request, bool addContentTypeHeader = true) const;

    [[nodiscard]] const QString &authMethod() const;
    void setAuthMethod(const QString &newAuthMethod);

    [[nodiscard]] const QString &authCode() const;
    void setAuthCode(const QString &newAuthCode);

    [[nodiscard]] QDateTime updatedSince() const;
    void setUpdatedSince(const QDateTime &newUpdatedSince);

Q_SIGNALS:
    void failed(const QString &str);

protected:
    Q_DISABLE_COPY(RestApiAbstractJob)

    [[nodiscard]] QString errorStr(const QJsonObject &replyObject);

    [[nodiscard]] QJsonDocument convertToJsonDocument(QNetworkReply *reply);
    void emitFailedMessage(const QString &replyErrorString, const QJsonObject &replyObject);
    void addAuthRawHeader(QNetworkRequest &request) const;
    [[nodiscard]] virtual QString errorMessage(const QString &str, const QJsonObject &detail);
    [[nodiscard]] virtual QString jobName() const;
    [[nodiscard]] virtual QString generateErrorMessage(const QString &errorStr) const;

    // The main methods used for sending requests to the server
    void submitGetRequest();
    void submitPostRequest(const QJsonDocument &doc);
    void submitDeleteRequest();

    RocketChatRestApi::RestApiMethod *mRestApiMethod = nullptr;
    QPointer<QNetworkReply> mReply;

protected Q_SLOTS:
    virtual void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
    {
        Q_UNUSED(replyJson)
        Q_UNUSED(replyErrorString)
    }
    virtual void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
    {
        Q_UNUSED(replyJson)
        Q_UNUSED(replyErrorString)
    }
    virtual void onDeleteRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
    {
        Q_UNUSED(replyJson)
        Q_UNUSED(replyErrorString)
    }

private:
    void genericResponseHandler(void (RestApiAbstractJob::*func)(const QString &, const QJsonDocument &));

    QDateTime mUpdatedSince;
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
