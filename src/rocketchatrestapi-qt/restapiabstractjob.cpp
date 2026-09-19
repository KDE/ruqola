/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "restapiabstractjob.h"

#include "abstractlogger.h"
#include "privateutils.h"
#include "rocketchatqtrestapi_debug.h"
#include <KLazyLocalizedString>
#include <KLocalizedString>
#include <QHash>
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
        mRestApiLogger->dataSent("RESTAPI: "_ba + str);
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

bool RestApiAbstractJob::interceptError([[maybe_unused]] const QJsonObject &replyObject)
{
    return false;
}

void RestApiAbstractJob::emitFailedMessage(const QString &replyErrorString, const QJsonObject &replyObject)
{
    if (!interceptError(replyObject)) {
        if (replyObject.isEmpty()) {
            Q_EMIT failed(replyErrorString, {});
        } else {
            Q_EMIT failed(replyErrorString, errorStr(replyObject));
        }
    }
}

std::optional<QJsonObject> RestApiAbstractJob::checkResponse(QByteArrayView name, const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo(name.toByteArray() + ": success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        return replyObject;
    }
    emitFailedMessage(replyErrorString, replyObject);
    addLoggerWarning(name.toByteArray() + ": problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    return {};
}

QString RestApiAbstractJob::errorStr(const QJsonObject &replyObject)
{
    // JSon-level error
    const QString errorType = replyObject["errorType"_L1].toString();
    if (!errorType.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "errorType" << errorType;
        QString trStr = errorMessage(errorType, replyObject["details"_L1].toObject());
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

namespace
{
// Translated messages for the error types that need nothing from the "details" object.
[[nodiscard]] const QHash<QStringView, KLazyLocalizedString> &simpleErrorMessages()
{
    static const QHash<QStringView, KLazyLocalizedString> messages = {
        {u"error-application-not-found", kli18n("Application not found")},
        {u"error-cant-invite-for-direct-room", kli18n("Can't invite user to direct rooms")},
        {u"error-channels-setdefault-is-same", kli18n("The channel default setting is the same as what it would be changed to.")},
        {u"error-channels-setdefault-missing-default-param", kli18n("The bodyParam 'default' is required")},
        {u"error-could-not-change-email", kli18n("Could not change email")},
        {u"error-could-not-change-name", kli18n("Could not change name")},
        {u"error-could-not-change-username", kli18n("Could not change username")},
        {u"error-delete-protected-role", kli18n("Cannot delete a protected role")},
        {u"error-department-not-found", kli18n("Department not found")},
        {u"error-direct-message-file-upload-not-allowed", kli18n("File sharing not allowed in direct messages")},
        {u"error-edit-permissions-not-allowed", kli18n("Editing permissions is not allowed")},
        {u"error-email-domain-blacklisted", kli18n("The email domain is blacklisted")},
        {u"error-file-too-large", kli18n("File is too large")},
        {u"error-importer-not-defined", kli18n("The importer was not defined correctly, it is missing the Import class.")},
        {u"error-import-file-extract-error", kli18n("Failed to extract import file.")},
        {u"error-import-file-is-empty", kli18n("Imported file seems to be empty.")},
        {u"error-import-file-missing", kli18n("The file to be imported was not found on the specified path.")},
        {u"error-invalid-actionlink", kli18n("Invalid action link")},
        {u"error-invalid-account", kli18n("Invalid Account")},
        {u"error-invalid-arguments", kli18n("Invalid arguments")},
        {u"error-invalid-asset", kli18n("Invalid asset")},
        {u"error-invalid-channel", kli18n("Invalid channel.")},
        {u"error-invalid-channel-start-with-chars", kli18n("Invalid channel. Start with @ or #")},
        {u"error-invalid-custom-field", kli18n("Invalid custom field")},
        {u"error-invalid-custom-field-name", kli18n("Invalid custom field name. Use only letters, numbers, hyphens and underscores.")},
        {u"error-invalid-date", kli18n("Invalid date provided.")},
        {u"error-invalid-description", kli18n("Invalid description")},
        {u"error-invalid-domain", kli18n("Invalid domain")},
        {u"error-invalid-email-address", kli18n("Invalid email address")},
        {u"error-invalid-file-height", kli18n("Invalid file height")},
        {u"error-invalid-file-type", kli18n("Invalid file type")},
        {u"error-invalid-file-width", kli18n("Invalid file width")},
        {u"error-invalid-from-address", kli18n("You informed an invalid FROM address.")},
        {u"error-invalid-integration", kli18n("Invalid integration")},
        {u"error-invalid-message", kli18n("Invalid message")},
        {u"error-invalid-method", kli18n("Invalid method")},
        {u"error-invalid-name", kli18n("Invalid name")},
        {u"error-invalid-password", kli18n("Invalid password")},
        {u"error-invalid-permission", kli18n("Invalid permission")},
        {u"error-invalid-redirectUri", kli18n("Invalid redirectUri")},
        {u"error-invalid-role", kli18n("Invalid role")},
        {u"error-invalid-room", kli18n("Invalid room")},
        {u"error-invalid-settings", kli18n("Invalid settings provided")},
        {u"error-invalid-subscription", kli18n("Invalid subscription")},
        {u"error-invalid-token", kli18n("Invalid token")},
        {u"error-invalid-triggerWords", kli18n("Invalid triggerWords")},
        {u"error-invalid-urls", kli18n("Invalid URLs")},
        {u"error-invalid-user", kli18n("Invalid user")},
        {u"error-invalid-username", kli18n("Invalid username")},
        {u"error-invalid-webhook-response", kli18n("The webhook URL responded with a status other than 200")},
        {u"error-message-deleting-blocked", kli18n("Message deleting is blocked")},
        {u"error-message-editing-blocked", kli18n("Message editing is blocked")},
        {u"error-message-size-exceeded", kli18n("Message size exceeds Message_MaxAllowedSize")},
        {u"error-missing-unsubscribe-link", kli18n("You must provide the [unsubscribe] link.")},
        {u"error-no-tokens-for-this-user", kli18n("There are no tokens for this user")},
        {u"error-not-allowed", kli18n("Not allowed")},
        {u"error-not-authorized", kli18n("Not authorized")},
        {u"not-authorized", kli18n("Not authorized")},
        {u"error-password-policy-not-met", kli18n("Password does not meet the server's policy")},
        {u"error-password-policy-not-met-maxLength", kli18n("Password does not meet the server's policy of maximum length (password too long)")},
        {u"error-password-policy-not-met-minLength", kli18n("Password does not meet the server's policy of minimum length (password too short)")},
        {u"error-password-policy-not-met-oneLowercase", kli18n("Password does not meet the server's policy of at least one lowercase character")},
        {u"error-password-policy-not-met-oneNumber", kli18n("Password does not meet the server's policy of at least one numerical character")},
        {u"error-password-policy-not-met-oneSpecial", kli18n("Password does not meet the server's policy of at least one special character")},
        {u"error-password-policy-not-met-oneUppercase", kli18n("Password does not meet the server's policy of at least one uppercase character")},
        {u"error-password-policy-not-met-repeatingCharacters",
         kli18n("Password does not meet the server's policy of forbidden repeating characters (you have too many of the same characters next to each other)")},
        {u"error-push-disabled", kli18n("Push is disabled")},
        {u"error-remove-last-owner", kli18n("This is the last owner. Please set a new owner before removing this one.")},
        {u"error-role-in-use", kli18n("Cannot delete role because it's in use")},
        {u"error-role-name-required", kli18n("Role name is required")},
        {u"error-room-is-not-closed", kli18n("Room is not closed")},
        {u"error-room-e2e-key-already-exists", kli18n("The room already has an end-to-end encryption key ID")},
        {u"error-this-is-not-a-livechat-room", kli18n("This is not a Livechat room")},
        {u"error-personal-access-tokens-are-current-disabled", kli18n("Personal Access Tokens are currently disabled")},
        {u"error-token-already-exists", kli18n("A token with this name already exists")},
        {u"error-token-does-not-exists", kli18n("Token does not exists")},
        {u"error-user-has-no-roles", kli18n("User has no roles")},
        {u"error-user-is-not-activated", kli18n("User is not activated")},
        {u"error-user-limit-exceeded", kli18n("The number of users you are trying to invite to #channel_name exceeds the limit set by the administrator")},
        {u"error-user-not-in-room", kli18n("User is not in this room")},
        {u"error-logged-user-not-in-room", kli18n("You are not in the room `%s`")},
        {u"error-user-registration-disabled", kli18n("User registration is disabled")},
        {u"error-user-registration-secret", kli18n("User registration is only allowed via Secret URL")},
        {u"error-you-are-last-owner", kli18n("You are the last owner. Please set new owner before leaving the room.")},
        {u"error-room-archived", kli18n("The private group is archived")},
        {u"error-user-already-owner", kli18n("User is already an owner")},
        {u"error-user-already-leader", kli18n("User is already a leader")},
        {u"error-user-already-moderator", kli18n("User is already a moderator")},
        {u"error-invalid-message_id", kli18n("Invalid message id")},
        {u"error-user-not-leader", kli18n("User is not a leader")},
        {u"error-app-user-is-not-allowed-to-login", kli18n("App user is not allowed to login")},
        {u"error-direct-message-room", kli18n("Direct Messages can not be archived")},
        {u"error-message-not-found", kli18n("Message not found.")},
        {u"totp-required", kli18n("Two Authentication Password Required")},
        {u"totp-invalid", kli18n("Invalid Password")},
        {u"error-room-not-found", kli18n("The required \\\"roomId\\\" or \\\"roomName\\\" param provided does not match any channel")},
        {u"error-role-already-present", kli18n("A role with this name already exists")},
        {u"error-pinning-message", kli18n("Message could not be pinned")},
        {u"error-password-in-history", kli18n("Entered password has been previously used")},
        {u"error-max-rooms-per-guest-reached", kli18n("The maximum number of rooms per guest has been reached.")},
    };
    return messages;
}

// Translated messages that substitute one or two fields of the "details" object, in "%1"/"%2" order.
struct DetailedErrorMessage {
    KLazyLocalizedString message;
    QLatin1StringView firstDetail;
    QLatin1StringView secondDetail = {};
};

[[nodiscard]] const QHash<QStringView, DetailedErrorMessage> &detailedErrorMessages()
{
    static const QHash<QStringView, DetailedErrorMessage> messages = {
        {u"error-action-not-allowed", {kli18n("'%1' is not allowed"), "action"_L1}},
        {u"error-archived-duplicate-name", {kli18n("There's an archived channel with name '%1'"), "room_name"_L1}},
        {u"error-duplicate-channel-name", {kli18n("A channel with name '%1' exists"), "channel_name"_L1}},
        {u"error-email-send-failed", {kli18n("Error trying to send email: %1"), "message"_L1}},
        {u"error-field-unavailable", {kli18n("'%1' is already in use :("), "field"_L1}},
        {u"error-input-is-not-a-valid-field", {kli18n("%1 is not a valid %2"), "input"_L1, "field"_L1}},
        {u"error-invalid-email", {kli18n("Invalid email '%1'"), "email"_L1}},
        {u"error-invalid-room-name", {kli18n("'%1' is not a valid room name"), "room_name"_L1}},
        {u"error-invalid-room-type", {kli18n("'%1' is not a valid room type."), "type"_L1}},
        {u"error-the-field-is-required", {kli18n("The field '%1' is required."), "field"_L1}},
        {u"error-too-many-requests", {kli18n("Error, too many requests. Please slow down. You must wait %1 seconds before trying again."), "seconds"_L1}},
    };
    return messages;
}
}

QString RestApiAbstractJob::errorMessage(const QString &str, const QJsonObject &details)
{
    const auto &simpleMessages = simpleErrorMessages();
    if (const auto it = simpleMessages.constFind(str); it != simpleMessages.cend()) {
        return it->toString();
    }
    const auto &detailedMessages = detailedErrorMessages();
    if (const auto it = detailedMessages.constFind(str); it != detailedMessages.cend()) {
        KLocalizedString message = it->message.subs(details[it->firstDetail].toString());
        if (!it->secondDetail.isEmpty()) {
            message = message.subs(details[it->secondDetail].toString());
        }
        return message.toString();
    }
    qCWarning(ROCKETCHATQTRESTAPI_LOG) << " unknown error type " << str;
    return {};
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
            // Ignore errors that will be handled in Connection class: it will destroy everything and relogin.
            // Delete the job (its owner may outlive the Connection), but not the reply: it is invalid at this
            // point and belongs to the QNetworkAccessManager, which dies with the Connection.
            qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Network error. Lost connection? Let's reconnect";
            deleteLater();
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

#include "moc_restapiabstractjob.cpp"
