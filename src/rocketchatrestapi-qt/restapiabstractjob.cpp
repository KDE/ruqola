/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "restapiabstractjob.h"
#include "abstractlogger.h"
#include "rocketchatqtrestapi_debug.h"
#include <KLocalizedString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>

using namespace RocketChatRestApi;
RestApiAbstractJob::RestApiAbstractJob(QObject *parent)
    : QObject(parent)
{
}

RestApiAbstractJob::~RestApiAbstractJob()
{
    if (mReply) {
        mReply->disconnect(this);
    }
}

QNetworkAccessManager *RestApiAbstractJob::networkAccessManager() const
{
    return mNetworkAccessManager;
}

void RestApiAbstractJob::setNetworkAccessManager(QNetworkAccessManager *networkAccessManager)
{
    mNetworkAccessManager = networkAccessManager;
}

RocketChatRestApi::RestApiMethod *RestApiAbstractJob::restApiMethod() const
{
    return mRestApiMethod;
}

void RestApiAbstractJob::setRestApiMethod(RocketChatRestApi::RestApiMethod *restApiMethod)
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

bool RestApiAbstractJob::requireTwoFactorAuthentication() const
{
    return false;
}

bool RestApiAbstractJob::hasQueryParameterSupport() const
{
    return false;
}

bool RestApiAbstractJob::enforcePasswordFallback() const
{
    return mEnforcePasswordFallBack;
}

void RestApiAbstractJob::setEnforcePasswordFallback(bool enforce)
{
    mEnforcePasswordFallBack = enforce;
}

bool RestApiAbstractJob::canStart() const
{
    if (requireTwoFactorAuthentication() && mEnforcePasswordFallBack) {
        if (mAuthMethod.isEmpty() || mAuthCode.isEmpty()) {
            qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Job required two factor auth but mAuthMethod or mAuthCode is empty";
            return false;
        }
    }
    if (!mNetworkAccessManager) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Network manager not defined";
        return false;
    }
    if (!mRestApiMethod) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "RestApiMethod not defined";
        return false;
    }
    if (requireHttpAuthentication() && !hasAuthenticationValue()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Auth settings is empty. It's a bug";
        return false;
    }
    return true;
}

void RestApiAbstractJob::addRequestAttribute(QNetworkRequest &request, bool addContentTypeHeader) const
{
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    request.setAttribute(QNetworkRequest::Http2AllowedAttribute, true);
    if (addContentTypeHeader) {
        request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    }
}

void RestApiAbstractJob::addAuthRawHeader(QNetworkRequest &request) const
{
    request.setRawHeader(QByteArrayLiteral("X-Auth-Token"), mAuthToken.toLocal8Bit());
    request.setRawHeader(QByteArrayLiteral("X-User-Id"), mUserId.toLocal8Bit());
    if (requireTwoFactorAuthentication() && mEnforcePasswordFallBack) {
        if (!mAuthMethod.isEmpty() && !mAuthCode.isEmpty()) {
            request.setRawHeader(QByteArrayLiteral("x-2fa-code"), mAuthCode.toLocal8Bit());
            request.setRawHeader(QByteArrayLiteral("x-2fa-method"), mAuthMethod.toLocal8Bit());
        }
    }
}

QueryParameters RestApiAbstractJob::queryParameters() const
{
    return mQueryParameters;
}

void RestApiAbstractJob::setQueryParameters(const QueryParameters &queryParameters)
{
    mQueryParameters = queryParameters;
}

void RestApiAbstractJob::addQueryParameter(QUrlQuery &urlQuery) const
{
    if (hasQueryParameterSupport() && mQueryParameters.isValid()) {
        QueryParameters::generateQueryParameter(mQueryParameters, urlQuery);
    }
    // qDebug() << " urlQuery " << urlQuery.toString();
}

RocketChatRestApi::AbstractLogger *RestApiAbstractJob::restApiLogger() const
{
    return mRestApiLogger;
}

void RestApiAbstractJob::setRestApiLogger(RocketChatRestApi::AbstractLogger *ruqolaLogger)
{
    mRestApiLogger = ruqolaLogger;
}

void RestApiAbstractJob::addLoggerInfo(const QByteArray &str)
{
    if (mRestApiLogger) { // when $RUQOLA_LOGFILE is set
        mRestApiLogger->dataSent("RESTAPI: " + str);
    } else {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "RESTAPI: " << str;
    }
}

void RestApiAbstractJob::addStartRestApiInfo(const QByteArray &str)
{
    if (mRestApiLogger) { // when $RUQOLA_LOGFILE is set
        mRestApiLogger->dataSent(AbstractLogger::RESTApiType, "RESTAPI:", str);
    } else {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "RESTAPI: " << str;
    }
}

void RestApiAbstractJob::addLoggerWarning(const QByteArray &str)
{
    if (mRestApiLogger) {
        mRestApiLogger->dataSent(AbstractLogger::RESTApiType, "WARNING RESTAPI: ", str);
    } else {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "RESTAPI: " << str;
    }
}

void RestApiAbstractJob::emitFailedMessage(const QString &replyErrorString, const QJsonObject &replyObject)
{
    Q_EMIT failed(replyErrorString + QLatin1Char('\n') + errorStr(replyObject));
}

QString RestApiAbstractJob::errorStr(const QJsonObject &replyObject)
{
    // JSon-level error
    const QString errorType = replyObject["errorType"_L1].toString();
    if (!errorType.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "errorType" << errorType;
        const QString trStr = errorMessage(errorType, replyObject["details"_L1].toObject());
        if (!trStr.isEmpty()) {
            return trStr;
        } else {
            qCWarning(ROCKETCHATQTRESTAPI_LOG) << " errorType not defined as translated message: " << errorType;
            return i18n("Unauthorized");
        }
    } else if (replyObject["status"_L1].toString() == QLatin1StringView("error")) {
        const QString message = replyObject["message"_L1].toString();
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "message error " << message;
        return generateErrorMessage(message);
    } else {
        const QString error = replyObject["error"_L1].toString();
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "error " << error;
        return generateErrorMessage(error);
    }
}

QString RestApiAbstractJob::generateErrorMessage(const QString &errorStr) const
{
    if (jobName().isEmpty()) {
        return errorStr;
    }
    return i18n("%1:%2", jobName(), errorStr);
}

QString RestApiAbstractJob::errorMessage(const QString &str, const QJsonObject &details)
{
    qDebug() << " details " << details;
    if (str == QLatin1StringView("error-action-not-allowed")) {
        const QString actionName = details["action"_L1].toString();
        return i18n("'%1' is not allowed", actionName);
    } else if (str == QLatin1StringView("error-application-not-found")) {
        return i18n("Application not found");
    } else if (str == QLatin1StringView("error-archived-duplicate-name")) {
        const QString roomName = details["room_name"_L1].toString();
        return i18n("There's an archived channel with name '%1'", roomName);
    } else if (str == QLatin1StringView("error-cant-invite-for-direct-room")) {
        return i18n("Can't invite user to direct rooms");
    } else if (str == QLatin1StringView("error-channels-setdefault-is-same")) {
        return i18n("The channel default setting is the same as what it would be changed to.");
    } else if (str == QLatin1StringView("error-channels-setdefault-missing-default-param")) {
        return i18n("The bodyParam 'default' is required");
    } else if (str == QLatin1StringView("error-could-not-change-email")) {
        return i18n("Could not change email");
    } else if (str == QLatin1StringView("error-could-not-change-name")) {
        return i18n("Could not change name");
    } else if (str == QLatin1StringView("error-could-not-change-username")) {
        return i18n("Could not change username");
    } else if (str == QLatin1StringView("error-delete-protected-role")) {
        return i18n("Cannot delete a protected role");
    } else if (str == QLatin1StringView("error-department-not-found")) {
        return i18n("Department not found");
    } else if (str == QLatin1StringView("error-direct-message-file-upload-not-allowed")) {
        return i18n("File sharing not allowed in direct messages");
    } else if (str == QLatin1StringView("error-duplicate-channel-name")) {
        const QString channelName = details["channel_name"_L1].toString();
        return i18n("A channel with name '%1' exists", channelName);
    } else if (str == QLatin1StringView("error-edit-permissions-not-allowed")) {
        return i18n("Editing permissions is not allowed");
    } else if (str == QLatin1StringView("error-email-domain-blacklisted")) {
        return i18n("The email domain is blacklisted");
    } else if (str == QLatin1StringView("error-email-send-failed")) {
        const QString message = details["message"_L1].toString();
        return i18n("Error trying to send email: %1", message);
    } else if (str == QLatin1StringView("error-field-unavailable")) {
        const QString field = details["field"_L1].toString();
        return i18n("'%1' is already in use :(", field);
    } else if (str == QLatin1StringView("error-file-too-large")) {
        return i18n("File is too large");
    } else if (str == QLatin1StringView("error-importer-not-defined")) {
        return i18n("The importer was not defined correctly, it is missing the Import class.");
    } else if (str == QLatin1StringView("error-import-file-extract-error")) {
        return i18n("Failed to extract import file.");
    } else if (str == QLatin1StringView("error-import-file-is-empty")) {
        return i18n("Imported file seems to be empty.");
    } else if (str == QLatin1StringView("error-import-file-missing")) {
        return i18n("The file to be imported was not found on the specified path.");
    } else if (str == QLatin1StringView("error-input-is-not-a-valid-field")) {
        const QString field = details["field"_L1].toString();
        const QString input = details["input"_L1].toString();
        return i18n("%1 is not a valid %2", input, field);
    } else if (str == QLatin1StringView("error-invalid-actionlink")) {
        return i18n("Invalid action link");
    } else if (str == QLatin1StringView("error-invalid-account")) {
        return i18n("Invalid Account");
    } else if (str == QLatin1StringView("error-invalid-arguments")) {
        return i18n("Invalid arguments");
    } else if (str == QLatin1StringView("error-invalid-asset")) {
        return i18n("Invalid asset");
    } else if (str == QLatin1StringView("error-invalid-channel")) {
        return i18n("Invalid channel.");
    } else if (str == QLatin1StringView("error-invalid-channel-start-with-chars")) {
        return i18n("Invalid channel. Start with @ or #");
    } else if (str == QLatin1StringView("error-invalid-custom-field")) {
        return i18n("Invalid custom field");
    } else if (str == QLatin1StringView("error-invalid-custom-field-name")) {
        return i18n("Invalid custom field name. Use only letters, numbers, hyphens and underscores.");
    } else if (str == QLatin1StringView("error-invalid-date")) {
        return i18n("Invalid date provided.");
    } else if (str == QLatin1StringView("error-invalid-description")) {
        return i18n("Invalid description");
    } else if (str == QLatin1StringView("error-invalid-domain")) {
        return i18n("Invalid domain");
    } else if (str == QLatin1StringView("error-invalid-email")) {
        const QString email = details["email"_L1].toString();
        return i18n("Invalid email '%1'", email);
    } else if (str == QLatin1StringView("error-invalid-email-address")) {
        return i18n("Invalid email address");
    } else if (str == QLatin1StringView("error-invalid-file-height")) {
        return i18n("Invalid file height");
    } else if (str == QLatin1StringView("error-invalid-file-type")) {
        return i18n("Invalid file type");
    } else if (str == QLatin1StringView("error-invalid-file-width")) {
        return i18n("Invalid file width");
    } else if (str == QLatin1StringView("error-invalid-from-address")) {
        return i18n("You informed an invalid FROM address.");
    } else if (str == QLatin1StringView("error-invalid-integration")) {
        return i18n("Invalid integration");
    } else if (str == QLatin1StringView("error-invalid-message")) {
        return i18n("Invalid message");
    } else if (str == QLatin1StringView("error-invalid-method")) {
        return i18n("Invalid method");
    } else if (str == QLatin1StringView("error-invalid-name")) {
        return i18n("Invalid name");
    } else if (str == QLatin1StringView("error-invalid-password")) {
        return i18n("Invalid password");
    } else if (str == QLatin1StringView("error-invalid-permission")) {
        return i18n("Invalid permission");
    } else if (str == QLatin1StringView("error-invalid-redirectUri")) {
        return i18n("Invalid redirectUri");
    } else if (str == QLatin1StringView("error-invalid-role")) {
        return i18n("Invalid role");
    } else if (str == QLatin1StringView("error-invalid-room")) {
        return i18n("Invalid room");
    } else if (str == QLatin1StringView("error-invalid-room-name")) {
        const QString roomName = details["room_name"_L1].toString();
        return i18n("'%1' is not a valid room name", roomName);
    } else if (str == QLatin1StringView("error-invalid-room-type")) {
        const QString roomType = details["type"_L1].toString();
        return i18n("'%1' is not a valid room type.", roomType);
    } else if (str == QLatin1StringView("error-invalid-settings")) {
        return i18n("Invalid settings provided");
    } else if (str == QLatin1StringView("error-invalid-subscription")) {
        return i18n("Invalid subscription");
    } else if (str == QLatin1StringView("error-invalid-token")) {
        return i18n("Invalid token");
    } else if (str == QLatin1StringView("error-invalid-triggerWords")) {
        return i18n("Invalid triggerWords");
    } else if (str == QLatin1StringView("error-invalid-urls")) {
        return i18n("Invalid URLs");
    } else if (str == QLatin1StringView("error-invalid-user")) {
        return i18n("Invalid user");
    } else if (str == QLatin1StringView("error-invalid-username")) {
        return i18n("Invalid username");
    } else if (str == QLatin1StringView("error-invalid-webhook-response")) {
        return i18n("The webhook URL responded with a status other than 200");
    } else if (str == QLatin1StringView("error-message-deleting-blocked")) {
        return i18n("Message deleting is blocked");
    } else if (str == QLatin1StringView("error-message-editing-blocked")) {
        return i18n("Message editing is blocked");
    } else if (str == QLatin1StringView("error-message-size-exceeded")) {
        return i18n("Message size exceeds Message_MaxAllowedSize");
    } else if (str == QLatin1StringView("error-missing-unsubscribe-link")) {
        return i18n("You must provide the [unsubscribe] link.");
    } else if (str == QLatin1StringView("error-no-tokens-for-this-user")) {
        return i18n("There are no tokens for this user");
    } else if (str == QLatin1StringView("error-not-allowed")) {
        return i18n("Not allowed");
    } else if (str == QLatin1StringView("error-not-authorized")) {
        return i18n("Not authorized");
    } else if (str == QLatin1StringView("not-authorized")) {
        return i18n("Not authorized");
    } else if (str == QLatin1StringView("error-password-policy-not-met")) {
        return i18n("Password does not meet the server's policy");
    } else if (str == QLatin1StringView("error-password-policy-not-met-maxLength")) {
        return i18n("Password does not meet the server's policy of maximum length (password too long)");
    } else if (str == QLatin1StringView("error-password-policy-not-met-minLength")) {
        return i18n("Password does not meet the server's policy of minimum length (password too short)");
    } else if (str == QLatin1StringView("error-password-policy-not-met-oneLowercase")) {
        return i18n("Password does not meet the server's policy of at least one lowercase character");
    } else if (str == QLatin1StringView("error-password-policy-not-met-oneNumber")) {
        return i18n("Password does not meet the server's policy of at least one numerical character");
    } else if (str == QLatin1StringView("error-password-policy-not-met-oneSpecial")) {
        return i18n("Password does not meet the server's policy of at least one special character");
    } else if (str == QLatin1StringView("error-password-policy-not-met-oneUppercase")) {
        return i18n("Password does not meet the server's policy of at least one uppercase character");
    } else if (str == QLatin1StringView("error-password-policy-not-met-repeatingCharacters")) {
        return i18n(
            "Password does not meet the server's policy of forbidden repeating characters (you have too many of the same characters next to each other)");
    } else if (str == QLatin1StringView("error-push-disabled")) {
        return i18n("Push is disabled");
    } else if (str == QLatin1StringView("error-remove-last-owner")) {
        return i18n("This is the last owner. Please set a new owner before removing this one.");
    } else if (str == QLatin1StringView("error-role-in-use")) {
        return i18n("Cannot delete role because it's in use");
    } else if (str == QLatin1StringView("error-role-name-required")) {
        return i18n("Role name is required");
    } else if (str == QLatin1StringView("error-room-is-not-closed")) {
        return i18n("Room is not closed");
    } else if (str == QLatin1StringView("error-the-field-is-required")) {
        const QString field = details["field"_L1].toString();
        return i18n("The field '%1' is required.", field);
    } else if (str == QLatin1StringView("error-this-is-not-a-livechat-room")) {
        return i18n("This is not a Livechat room");
    } else if (str == QLatin1StringView("error-personal-access-tokens-are-current-disabled")) {
        return i18n("Personal Access Tokens are currently disabled");
    } else if (str == QLatin1StringView("error-token-already-exists")) {
        return i18n("A token with this name already exists");
    } else if (str == QLatin1StringView("error-token-does-not-exists")) {
        return i18n("Token does not exists");
    } else if (str == QLatin1StringView("error-too-many-requests")) {
        const QString seconds = details["seconds"_L1].toString();
        return i18n("Error, too many requests. Please slow down. You must wait %1 seconds before trying again.", seconds);
    } else if (str == QLatin1StringView("error-user-has-no-roles")) {
        return i18n("User has no roles");
    } else if (str == QLatin1StringView("error-user-is-not-activated")) {
        return i18n("User is not activated");
    } else if (str == QLatin1StringView("error-user-limit-exceeded")) {
        return i18n("The number of users you are trying to invite to #channel_name exceeds the limit set by the administrator");
    } else if (str == QLatin1StringView("error-user-not-in-room")) {
        return i18n("User is not in this room");
    } else if (str == QLatin1StringView("error-logged-user-not-in-room")) {
        return i18n("You are not in the room `%s`");
    } else if (str == QLatin1StringView("error-user-registration-disabled")) {
        return i18n("User registration is disabled");
    } else if (str == QLatin1StringView("error-user-registration-secret")) {
        return i18n("User registration is only allowed via Secret URL");
    } else if (str == QLatin1StringView("error-you-are-last-owner")) {
        return i18n("You are the last owner. Please set new owner before leaving the room.");
    } else if (str == QLatin1StringView("error-room-archived")) {
        return i18n("The private group is archived");
    } else if (str == QLatin1StringView("error-user-already-owner")) {
        return i18n("User is already an owner");
    } else if (str == QLatin1StringView("error-user-already-leader")) {
        return i18n("User is already a leader");
    } else if (str == QLatin1StringView("error-user-already-moderator")) {
        return i18n("User is already a moderator");
    } else if (str == QLatin1StringView("error-invalid-message_id")) {
        return i18n("Invalid message id");
    } else if (str == QLatin1StringView("error-user-not-leader")) {
        return i18n("User is not a leader");
    } else if (str == QLatin1StringView("error-app-user-is-not-allowed-to-login")) {
        return i18n("App user is not allowed to login");
    } else if (str == QLatin1StringView("error-direct-message-room")) {
        return i18n("Direct Messages can not be archived");
    } else if (str == QLatin1StringView("error-message-not-found")) {
        return i18n("Message not found.");
    } else if (str == QLatin1StringView("totp-required")) {
        return i18n("Two Authentication Password Required");
    } else if (str == QLatin1StringView("totp-invalid")) {
        return i18n("Invalid Password");
    } else if (str == QLatin1StringView("error-room-not-found")) {
        return i18n("The required \\\"roomId\\\" or \\\"roomName\\\" param provided does not match any channel");
    } else if (str == QLatin1StringView("error-role-already-present")) {
        return i18n("A role with this name already exists");
    } else if (str == QLatin1StringView("error-pinning-message")) {
        return i18n("Message could not be pinned");
    } else if (str == QLatin1StringView("error-password-in-history")) {
        return i18n("Entered password has been previously used");
    } else if (str == QLatin1StringView("error-max-rooms-per-guest-reached")) {
        return i18n("The maximum number of rooms per guest has been reached.");
    } else {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << " unknown error type " << str;
        return {};
    }
}

QString RestApiAbstractJob::jobName() const
{
    return {};
}

void RestApiAbstractJob::genericResponseHandler(void (RestApiAbstractJob::*responseHandler)(const QString &, const QJsonDocument &))
{
    if (!mReply) {
        deleteLater();
        return;
    }

    if (mReply->error() != QNetworkReply::NoError) {
        if (mReply->error() == QNetworkReply::NetworkSessionFailedError) {
            // Ignore NetworkSessionFailedError. It will be handled in Connection class.
            // no deleting, we will be trying to destroy everything and relogin
            // reply will be invalid at this point, deleting it will crash us
            qCWarning(ROCKETCHATQTRESTAPI_LOG) << "NetworkSessionFailedError. Lost connection? ";
            return;
        }
        // TODO add support error 400
        (this->*responseHandler)(mReply->errorString(), convertToJsonDocument(mReply));
    } else {
        (this->*responseHandler)(QString(), convertToJsonDocument(mReply));
    }

    mReply->deleteLater();
    deleteLater();
}

QDateTime RestApiAbstractJob::updatedSince() const
{
    return mUpdatedSince;
}

void RestApiAbstractJob::setUpdatedSince(const QDateTime &newUpdatedSince)
{
    mUpdatedSince = newUpdatedSince;
}

const QString &RestApiAbstractJob::authCode() const
{
    return mAuthCode;
}

void RestApiAbstractJob::setAuthCode(const QString &newAuthCode)
{
    mAuthCode = newAuthCode;
}

const QString &RestApiAbstractJob::authMethod() const
{
    return mAuthMethod;
}

void RestApiAbstractJob::setAuthMethod(const QString &newAuthMethod)
{
    mAuthMethod = newAuthMethod;
}

void RestApiAbstractJob::submitDeleteRequest()
{
    mReply = mNetworkAccessManager->deleteResource(request());
    const QByteArray className = metaObject()->className();
    mReply->setProperty("jobClassName", className);

    connect(mReply.data(), &QNetworkReply::finished, this, [this] {
        genericResponseHandler(&RestApiAbstractJob::onDeleteRequestResponse);
    });
}

void RestApiAbstractJob::submitGetRequest()
{
    mReply = mNetworkAccessManager->get(request());
    const QByteArray className = metaObject()->className();
    mReply->setProperty("jobClassName", className);

    connect(mReply.data(), &QNetworkReply::finished, this, [this] {
        genericResponseHandler(&RestApiAbstractJob::onGetRequestResponse);
    });
}

void RestApiAbstractJob::submitPostRequest(const QJsonDocument &doc)
{
    const QByteArray baPostData = doc.isNull() ? QByteArray() : doc.toJson(QJsonDocument::Compact);
    mReply = mNetworkAccessManager->post(request(), baPostData);
    const QByteArray className = metaObject()->className();
    mReply->setProperty("jobClassName", className);

    addLoggerInfo(className + " started " + baPostData);

    connect(mReply.data(), &QNetworkReply::finished, this, [this] {
        genericResponseHandler(&RestApiAbstractJob::onPostRequestResponse);
    });
}

QJsonDocument RestApiAbstractJob::convertToJsonDocument(QNetworkReply *reply)
{
    const QByteArray data = reply->readAll();
    const QJsonDocument replyDocument = QJsonDocument::fromJson(data);
    if (replyDocument.isNull()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << " convertToJsonObject return null jsondocument. It's a bug ";
    }
    return replyDocument;
}

QueryParameters::QueryParameters() = default;

QString QueryParameters::filter() const
{
    return mFilter;
}

void QueryParameters::setFilter(const QString &filter)
{
    mFilter = filter;
}

int QueryParameters::offset() const
{
    return mOffset;
}

void QueryParameters::setOffset(int offset)
{
    mOffset = offset;
}

int QueryParameters::count() const
{
    return mCount;
}

void QueryParameters::setCount(int count)
{
    mCount = count;
}

bool QueryParameters::isValid() const
{
    return (mCount >= 0) || (mOffset >= 0) || (!mSorting.isEmpty()) || !mCustom.isEmpty() || !mSearchString.isEmpty() || !mFilter.isEmpty();
}

QMap<QString, QueryParameters::SortOrder> QueryParameters::sorting() const
{
    return mSorting;
}

void QueryParameters::setSorting(const QMap<QString, QueryParameters::SortOrder> &sorting)
{
    mSorting = sorting;
}

QString QueryParameters::type() const
{
    return mType;
}

void QueryParameters::setType(const QString &type)
{
    mType = type;
}

QMap<QString, QString> QueryParameters::custom() const
{
    return mCustom;
}

void QueryParameters::setCustom(const QMap<QString, QString> &custom)
{
    mCustom = custom;
}

void QueryParameters::generateQueryParameter(const QueryParameters &queryParameters, QUrlQuery &urlQuery)
{
    if (queryParameters.count() >= 0) {
        urlQuery.addQueryItem(QStringLiteral("count"), QString::number(queryParameters.count()));
    }
    if (queryParameters.offset() >= 0) {
        urlQuery.addQueryItem(QStringLiteral("offset"), QString::number(queryParameters.offset()));
    }
    if (!queryParameters.filter().isEmpty()) {
        urlQuery.addQueryItem(QStringLiteral("filter"), queryParameters.filter());
    }

    const QMap<QString, QString> custom = queryParameters.custom();
    if (!custom.isEmpty()) {
        QMapIterator<QString, QString> i(custom);
        QString str;
        while (i.hasNext()) {
            i.next();
            if (!str.isEmpty()) {
                str += QLatin1Char(',');
            }
            str += QLatin1Char('"') + i.key() + QLatin1Char('"') + QLatin1Char(':');
            str += QLatin1Char('"') + i.value() + QLatin1Char('"');
        }
        str = QStringLiteral("{%1}").arg(str);

        urlQuery.addQueryItem(QStringLiteral("query"), str);
    }
    if (!queryParameters.searchString().isEmpty()) {
        const QString str = QStringLiteral(R"({"name":{"$regex":"%1","$options":"i"}})").arg(queryParameters.searchString());
        urlQuery.addQueryItem(QStringLiteral("query"), str);
    }

    if (!queryParameters.sorting().isEmpty()) {
        // example    sort={"name" : -1,"status" : 1}
        QMapIterator<QString, QueryParameters::SortOrder> i(queryParameters.sorting());
        QString str;
        while (i.hasNext()) {
            i.next();
            if (!str.isEmpty()) {
                str += QLatin1Char(',');
            }
            str += QLatin1Char('"') + i.key() + QLatin1Char('"') + QLatin1Char(':');
            switch (i.value()) {
            case QueryParameters::SortOrder::Ascendant:
                str += QString::number(1);
                break;
            case QueryParameters::SortOrder::Descendant:
                str += QString::number(-1);
                break;
            case QueryParameters::SortOrder::NoSorting:
                qCWarning(ROCKETCHATQTRESTAPI_LOG) << "It's not a sorting attribute";
                break;
            }
        }
        str = QStringLiteral("{%1}").arg(str);

        // It's ok for getAllMentions....
        urlQuery.addQueryItem(QStringLiteral("sort"), str);
    }
    if (!queryParameters.type().isEmpty()) {
        urlQuery.addQueryItem(QStringLiteral("type"), queryParameters.type());
    }
}

const QString &QueryParameters::searchString() const
{
    return mSearchString;
}

void QueryParameters::setSearchString(const QString &newSearchString)
{
    mSearchString = newSearchString;
}

#include "moc_restapiabstractjob.cpp"
