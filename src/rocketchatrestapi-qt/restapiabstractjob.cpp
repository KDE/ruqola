/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "restapiabstractjob.h"

#include "abstractlogger.h"
#include "privateutils.h"
#include "rocketchatqtrestapi_debug.h"
#include <KLocalizedString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

#include <QUrlQuery>

using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;

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
        request.setHeader(QNetworkRequest::ContentTypeHeader, u"application/json"_s);
    }
}

void RestApiAbstractJob::addAuthRawHeader(QNetworkRequest &request) const
{
    request.setRawHeader("X-Auth-Token"_ba, mAuthToken.toLocal8Bit());
    request.setRawHeader("X-User-Id"_ba, mUserId.toLocal8Bit());
    if (requireTwoFactorAuthentication() && mEnforcePasswordFallBack) {
        if (!mAuthMethod.isEmpty() && !mAuthCode.isEmpty()) {
            request.setRawHeader("x-2fa-code"_ba, mAuthCode.toLocal8Bit());
            request.setRawHeader("x-2fa-method"_ba, mAuthMethod.toLocal8Bit());
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
    if (replyObject.isEmpty()) {
        Q_EMIT failed(replyErrorString);
    } else {
        Q_EMIT failed(replyErrorString + u'\n' + errorStr(replyObject));
    }
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
    } else if (replyObject["status"_L1].toString() == "error"_L1) {
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
    // qDebug() << " details " << details;
    if (str == "error-action-not-allowed"_L1) {
        const QString actionName = details["action"_L1].toString();
        return i18n("'%1' is not allowed", actionName);
    } else if (str == "error-application-not-found"_L1) {
        return i18n("Application not found");
    } else if (str == "error-archived-duplicate-name"_L1) {
        const QString roomName = details["room_name"_L1].toString();
        return i18n("There's an archived channel with name '%1'", roomName);
    } else if (str == "error-cant-invite-for-direct-room"_L1) {
        return i18n("Can't invite user to direct rooms");
    } else if (str == "error-channels-setdefault-is-same"_L1) {
        return i18n("The channel default setting is the same as what it would be changed to.");
    } else if (str == "error-channels-setdefault-missing-default-param"_L1) {
        return i18n("The bodyParam 'default' is required");
    } else if (str == "error-could-not-change-email"_L1) {
        return i18n("Could not change email");
    } else if (str == "error-could-not-change-name"_L1) {
        return i18n("Could not change name");
    } else if (str == "error-could-not-change-username"_L1) {
        return i18n("Could not change username");
    } else if (str == "error-delete-protected-role"_L1) {
        return i18n("Cannot delete a protected role");
    } else if (str == "error-department-not-found"_L1) {
        return i18n("Department not found");
    } else if (str == "error-direct-message-file-upload-not-allowed"_L1) {
        return i18n("File sharing not allowed in direct messages");
    } else if (str == "error-duplicate-channel-name"_L1) {
        const QString channelName = details["channel_name"_L1].toString();
        return i18n("A channel with name '%1' exists", channelName);
    } else if (str == "error-edit-permissions-not-allowed"_L1) {
        return i18n("Editing permissions is not allowed");
    } else if (str == "error-email-domain-blacklisted"_L1) {
        return i18n("The email domain is blacklisted");
    } else if (str == "error-email-send-failed"_L1) {
        const QString message = details["message"_L1].toString();
        return i18n("Error trying to send email: %1", message);
    } else if (str == "error-field-unavailable"_L1) {
        const QString field = details["field"_L1].toString();
        return i18n("'%1' is already in use :(", field);
    } else if (str == "error-file-too-large"_L1) {
        return i18n("File is too large");
    } else if (str == "error-importer-not-defined"_L1) {
        return i18n("The importer was not defined correctly, it is missing the Import class.");
    } else if (str == "error-import-file-extract-error"_L1) {
        return i18n("Failed to extract import file.");
    } else if (str == "error-import-file-is-empty"_L1) {
        return i18n("Imported file seems to be empty.");
    } else if (str == "error-import-file-missing"_L1) {
        return i18n("The file to be imported was not found on the specified path.");
    } else if (str == "error-input-is-not-a-valid-field"_L1) {
        const QString field = details["field"_L1].toString();
        const QString input = details["input"_L1].toString();
        return i18n("%1 is not a valid %2", input, field);
    } else if (str == "error-invalid-actionlink"_L1) {
        return i18n("Invalid action link");
    } else if (str == "error-invalid-account"_L1) {
        return i18n("Invalid Account");
    } else if (str == "error-invalid-arguments"_L1) {
        return i18n("Invalid arguments");
    } else if (str == "error-invalid-asset"_L1) {
        return i18n("Invalid asset");
    } else if (str == "error-invalid-channel"_L1) {
        return i18n("Invalid channel.");
    } else if (str == "error-invalid-channel-start-with-chars"_L1) {
        return i18n("Invalid channel. Start with @ or #");
    } else if (str == "error-invalid-custom-field"_L1) {
        return i18n("Invalid custom field");
    } else if (str == "error-invalid-custom-field-name"_L1) {
        return i18n("Invalid custom field name. Use only letters, numbers, hyphens and underscores.");
    } else if (str == "error-invalid-date"_L1) {
        return i18n("Invalid date provided.");
    } else if (str == "error-invalid-description"_L1) {
        return i18n("Invalid description");
    } else if (str == "error-invalid-domain"_L1) {
        return i18n("Invalid domain");
    } else if (str == "error-invalid-email"_L1) {
        const QString email = details["email"_L1].toString();
        return i18n("Invalid email '%1'", email);
    } else if (str == "error-invalid-email-address"_L1) {
        return i18n("Invalid email address");
    } else if (str == "error-invalid-file-height"_L1) {
        return i18n("Invalid file height");
    } else if (str == "error-invalid-file-type"_L1) {
        return i18n("Invalid file type");
    } else if (str == "error-invalid-file-width"_L1) {
        return i18n("Invalid file width");
    } else if (str == "error-invalid-from-address"_L1) {
        return i18n("You informed an invalid FROM address.");
    } else if (str == "error-invalid-integration"_L1) {
        return i18n("Invalid integration");
    } else if (str == "error-invalid-message"_L1) {
        return i18n("Invalid message");
    } else if (str == "error-invalid-method"_L1) {
        return i18n("Invalid method");
    } else if (str == "error-invalid-name"_L1) {
        return i18n("Invalid name");
    } else if (str == "error-invalid-password"_L1) {
        return i18n("Invalid password");
    } else if (str == "error-invalid-permission"_L1) {
        return i18n("Invalid permission");
    } else if (str == "error-invalid-redirectUri"_L1) {
        return i18n("Invalid redirectUri");
    } else if (str == "error-invalid-role"_L1) {
        return i18n("Invalid role");
    } else if (str == "error-invalid-room"_L1) {
        return i18n("Invalid room");
    } else if (str == "error-invalid-room-name"_L1) {
        const QString roomName = details["room_name"_L1].toString();
        return i18n("'%1' is not a valid room name", roomName);
    } else if (str == "error-invalid-room-type"_L1) {
        const QString roomType = details["type"_L1].toString();
        return i18n("'%1' is not a valid room type.", roomType);
    } else if (str == "error-invalid-settings"_L1) {
        return i18n("Invalid settings provided");
    } else if (str == "error-invalid-subscription"_L1) {
        return i18n("Invalid subscription");
    } else if (str == "error-invalid-token"_L1) {
        return i18n("Invalid token");
    } else if (str == "error-invalid-triggerWords"_L1) {
        return i18n("Invalid triggerWords");
    } else if (str == "error-invalid-urls"_L1) {
        return i18n("Invalid URLs");
    } else if (str == "error-invalid-user"_L1) {
        return i18n("Invalid user");
    } else if (str == "error-invalid-username"_L1) {
        return i18n("Invalid username");
    } else if (str == "error-invalid-webhook-response"_L1) {
        return i18n("The webhook URL responded with a status other than 200");
    } else if (str == "error-message-deleting-blocked"_L1) {
        return i18n("Message deleting is blocked");
    } else if (str == "error-message-editing-blocked"_L1) {
        return i18n("Message editing is blocked");
    } else if (str == "error-message-size-exceeded"_L1) {
        return i18n("Message size exceeds Message_MaxAllowedSize");
    } else if (str == "error-missing-unsubscribe-link"_L1) {
        return i18n("You must provide the [unsubscribe] link.");
    } else if (str == "error-no-tokens-for-this-user"_L1) {
        return i18n("There are no tokens for this user");
    } else if (str == "error-not-allowed"_L1) {
        return i18n("Not allowed");
    } else if (str == "error-not-authorized"_L1 || str == "not-authorized"_L1) {
        return i18n("Not authorized");
    } else if (str == "error-password-policy-not-met"_L1) {
        return i18n("Password does not meet the server's policy");
    } else if (str == "error-password-policy-not-met-maxLength"_L1) {
        return i18n("Password does not meet the server's policy of maximum length (password too long)");
    } else if (str == "error-password-policy-not-met-minLength"_L1) {
        return i18n("Password does not meet the server's policy of minimum length (password too short)");
    } else if (str == "error-password-policy-not-met-oneLowercase"_L1) {
        return i18n("Password does not meet the server's policy of at least one lowercase character");
    } else if (str == "error-password-policy-not-met-oneNumber"_L1) {
        return i18n("Password does not meet the server's policy of at least one numerical character");
    } else if (str == "error-password-policy-not-met-oneSpecial"_L1) {
        return i18n("Password does not meet the server's policy of at least one special character");
    } else if (str == "error-password-policy-not-met-oneUppercase"_L1) {
        return i18n("Password does not meet the server's policy of at least one uppercase character");
    } else if (str == "error-password-policy-not-met-repeatingCharacters"_L1) {
        return i18n(
            "Password does not meet the server's policy of forbidden repeating characters (you have too many of the same characters next to each other)");
    } else if (str == "error-push-disabled"_L1) {
        return i18n("Push is disabled");
    } else if (str == "error-remove-last-owner"_L1) {
        return i18n("This is the last owner. Please set a new owner before removing this one.");
    } else if (str == "error-role-in-use"_L1) {
        return i18n("Cannot delete role because it's in use");
    } else if (str == "error-role-name-required"_L1) {
        return i18n("Role name is required");
    } else if (str == "error-room-is-not-closed"_L1) {
        return i18n("Room is not closed");
    } else if (str == "error-the-field-is-required"_L1) {
        const QString field = details["field"_L1].toString();
        return i18n("The field '%1' is required.", field);
    } else if (str == "error-this-is-not-a-livechat-room"_L1) {
        return i18n("This is not a Livechat room");
    } else if (str == "error-personal-access-tokens-are-current-disabled"_L1) {
        return i18n("Personal Access Tokens are currently disabled");
    } else if (str == "error-token-already-exists"_L1) {
        return i18n("A token with this name already exists");
    } else if (str == "error-token-does-not-exists"_L1) {
        return i18n("Token does not exists");
    } else if (str == "error-too-many-requests"_L1) {
        const QString seconds = details["seconds"_L1].toString();
        return i18n("Error, too many requests. Please slow down. You must wait %1 seconds before trying again.", seconds);
    } else if (str == "error-user-has-no-roles"_L1) {
        return i18n("User has no roles");
    } else if (str == "error-user-is-not-activated"_L1) {
        return i18n("User is not activated");
    } else if (str == "error-user-limit-exceeded"_L1) {
        return i18n("The number of users you are trying to invite to #channel_name exceeds the limit set by the administrator");
    } else if (str == "error-user-not-in-room"_L1) {
        return i18n("User is not in this room");
    } else if (str == "error-logged-user-not-in-room"_L1) {
        return i18n("You are not in the room `%s`");
    } else if (str == "error-user-registration-disabled"_L1) {
        return i18n("User registration is disabled");
    } else if (str == "error-user-registration-secret"_L1) {
        return i18n("User registration is only allowed via Secret URL");
    } else if (str == "error-you-are-last-owner"_L1) {
        return i18n("You are the last owner. Please set new owner before leaving the room.");
    } else if (str == "error-room-archived"_L1) {
        return i18n("The private group is archived");
    } else if (str == "error-user-already-owner"_L1) {
        return i18n("User is already an owner");
    } else if (str == "error-user-already-leader"_L1) {
        return i18n("User is already a leader");
    } else if (str == "error-user-already-moderator"_L1) {
        return i18n("User is already a moderator");
    } else if (str == "error-invalid-message_id"_L1) {
        return i18n("Invalid message id");
    } else if (str == "error-user-not-leader"_L1) {
        return i18n("User is not a leader");
    } else if (str == "error-app-user-is-not-allowed-to-login"_L1) {
        return i18n("App user is not allowed to login");
    } else if (str == "error-direct-message-room"_L1) {
        return i18n("Direct Messages can not be archived");
    } else if (str == "error-message-not-found"_L1) {
        return i18n("Message not found.");
    } else if (str == "totp-required"_L1) {
        return i18n("Two Authentication Password Required");
    } else if (str == "totp-invalid"_L1) {
        return i18n("Invalid Password");
    } else if (str == "error-room-not-found"_L1) {
        return i18n("The required \\\"roomId\\\" or \\\"roomName\\\" param provided does not match any channel");
    } else if (str == "error-role-already-present"_L1) {
        return i18n("A role with this name already exists");
    } else if (str == "error-pinning-message"_L1) {
        return i18n("Message could not be pinned");
    } else if (str == "error-password-in-history"_L1) {
        return i18n("Entered password has been previously used");
    } else if (str == "error-max-rooms-per-guest-reached"_L1) {
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

    const auto error = mReply->error();
    if (error != QNetworkReply::NoError) {
        if (networkErrorsNeedingReconnect().contains(error)) {
            // Ignore errors that will be handled in Connection class.
            // no deleting, we will be trying to destroy everything and relogin
            // reply will be invalid at this point, deleting it will crash us
            qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Network error. Lost connection? Let's reconnect";
            return;
        }
        // qDebug() << mReply->readAll();
        // JSon can be null
        (this->*responseHandler)(mReply->errorString(), convertToJsonDocument(mReply, true));
    } else {
        (this->*responseHandler)(QString(), convertToJsonDocument(mReply));
    }

    if (mReply) {
        mReply->deleteLater();
    }
    deleteLater();
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

QJsonDocument RestApiAbstractJob::convertToJsonDocument(QNetworkReply *reply, bool canBeNull)
{
    const QByteArray data = reply->readAll();
    const QJsonDocument replyDocument = QJsonDocument::fromJson(data);
    if (replyDocument.isNull() && !canBeNull) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << " convertToJsonObject return null jsondocument. It's a bug. Data:" << data;
    }
    return replyDocument;
}

QueryParameters::QueryParameters() = default;

#include "moc_restapiabstractjob.cpp"
