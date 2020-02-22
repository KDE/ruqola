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

#include "restapiabstractjob.h"
#include "rocketchatqtrestapi_debug.h"
#include "abstractlogger.h"
#include <KLocalizedString>
#include <QJsonObject>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>
using namespace RocketChatRestApi;
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

bool RestApiAbstractJob::hasQueryParameterSupport() const
{
    return false;
}

bool RestApiAbstractJob::canStart() const
{
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

void RestApiAbstractJob::addRequestAttribute(QNetworkRequest &request) const
{
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    request.setAttribute(QNetworkRequest::HTTP2AllowedAttribute, true);
#else
    request.setAttribute(QNetworkRequest::Http2AllowedAttribute, true);
#endif
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
}

void RestApiAbstractJob::addAuthRawHeader(QNetworkRequest &request) const
{
    request.setRawHeader(QByteArrayLiteral("X-Auth-Token"), mAuthToken.toLocal8Bit());
    request.setRawHeader(QByteArrayLiteral("X-User-Id"), mUserId.toLocal8Bit());
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
        if (mQueryParameters.count() >= 0) {
            urlQuery.addQueryItem(QStringLiteral("count"), QString::number(mQueryParameters.count()));
        }
        if (mQueryParameters.offset() >= 0) {
            urlQuery.addQueryItem(QStringLiteral("offset"), QString::number(mQueryParameters.offset()));
        }
        if (!mQueryParameters.sorting().isEmpty()) {
            //example    sort={"name" : -1,"status" : 1}
            QMapIterator<QString, QueryParameters::SortOrder> i(mQueryParameters.sorting());
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

            //It's ok for getAllMentions....
            urlQuery.addQueryItem(QStringLiteral("sort"), str);
        }
    }
    //qDebug() << " urlQuery " << urlQuery.toString();
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

void RestApiAbstractJob::addLoggerWarning(const QByteArray &str)
{
    if (mRestApiLogger) {
        mRestApiLogger->dataSent("WARNING RESTAPI: " + str);
    } else {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "RESTAPI: " << str;
    }
}

void RestApiAbstractJob::emitFailedMessage(const QJsonObject &replyObject, QNetworkReply *reply)
{
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    const auto error = reply->error();
#else
    const auto error = reply->networkError();
#endif
    // HTTP-level error (e.g. host not found)
    if (error != QNetworkReply::NoError) {
        Q_EMIT failed(reply->errorString() + QLatin1Char('\n') + errorStr(replyObject));
        return;
    }
    Q_EMIT failed(errorStr(replyObject));
}

QString RestApiAbstractJob::errorStr(const QJsonObject &replyObject)
{
    // JSon-level error
    const QString errorType = replyObject[QStringLiteral("errorType")].toString();
    if (!errorType.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "errorType" << errorType;
        const QString trStr = errorMessage(errorType);
        if (!trStr.isEmpty()) {
            return trStr;
        } else {
            qCWarning(ROCKETCHATQTRESTAPI_LOG) << " errorType not defined as translated message: " << errorType;
            return i18n("Unauthorized");
        }
    } else {
        const QString error = replyObject[QStringLiteral("error")].toString();
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

QString RestApiAbstractJob::errorMessage(const QString &str)
{
    if (str == QLatin1String("error-action-not-allowed")) {
        return i18n("__action__ is not allowed");
    } else if (str == QLatin1String("error-application-not-found")) {
        return i18n("Application not found");
    } else if (str == QLatin1String("error-archived-duplicate-name")) {
        return i18n("There's an archived channel with name '__room_name__'");
    } else if (str == QLatin1String("error-avatar-invalid-url")) {
        return i18n("Invalid avatar URL: __url__");
    } else if (str == QLatin1String("error-avatar-url-handling")) {
        return i18n("Error while handling avatar setting from a URL (__url__) for __username__");
    } else if (str == QLatin1String("error-cant-invite-for-direct-room")) {
        return i18n("Can't invite user to direct rooms");
    } else if (str == QLatin1String("error-channels-setdefault-is-same")) {
        return i18n("The channel default setting is the same as what it would be changed to.");
    } else if (str == QLatin1String("error-channels-setdefault-missing-default-param")) {
        return i18n("The bodyParam 'default' is required");
    } else if (str == QLatin1String("error-could-not-change-email")) {
        return i18n("Could not change email");
    } else if (str == QLatin1String("error-could-not-change-name")) {
        return i18n("Could not change name");
    } else if (str == QLatin1String("error-could-not-change-username")) {
        return i18n("Could not change username");
    } else if (str == QLatin1String("error-delete-protected-role")) {
        return i18n("Cannot delete a protected role");
    } else if (str == QLatin1String("error-department-not-found")) {
        return i18n("Department not found");
    } else if (str == QLatin1String("error-direct-message-file-upload-not-allowed")) {
        return i18n("File sharing not allowed in direct messages");
    } else if (str == QLatin1String("error-duplicate-channel-name")) {
        return i18n("A channel with name '__channel_name__' exists");
    } else if (str == QLatin1String("error-edit-permissions-not-allowed")) {
        return i18n("Editing permissions is not allowed");
    } else if (str == QLatin1String("error-email-domain-blacklisted")) {
        return i18n("The email domain is blacklisted");
    } else if (str == QLatin1String("error-email-send-failed")) {
        return i18n("Error trying to send email: __message__");
    } else if (str == QLatin1String("error-field-unavailable")) {
        return i18n("<strong>__field__</strong> is already in use :(");
    } else if (str == QLatin1String("error-file-too-large")) {
        return i18n("File is too large");
    } else if (str == QLatin1String("error-importer-not-defined")) {
        return i18n("The importer was not defined correctly, it is missing the Import class.");
    } else if (str == QLatin1String("error-import-file-extract-error")) {
        return i18n("Failed to extract import file.");
    } else if (str == QLatin1String("error-import-file-is-empty")) {
        return i18n("Imported file seems to be empty.");
    } else if (str == QLatin1String("error-import-file-missing")) {
        return i18n("The file to be imported was not found on the specified path.");
    } else if (str == QLatin1String("error-input-is-not-a-valid-field")) {
        return i18n("__input__ is not a valid __field__");
    } else if (str == QLatin1String("error-invalid-actionlink")) {
        return i18n("Invalid action link");
    } else if (str == QLatin1String("error-invalid-account")) {
        return i18n("Invalid Account");
    } else if (str == QLatin1String("error-invalid-arguments")) {
        return i18n("Invalid arguments");
    } else if (str == QLatin1String("error-invalid-asset")) {
        return i18n("Invalid asset");
    } else if (str == QLatin1String("error-invalid-channel")) {
        return i18n("Invalid channel.");
    } else if (str == QLatin1String("error-invalid-channel-start-with-chars")) {
        return i18n("Invalid channel. Start with @ or #");
    } else if (str == QLatin1String("error-invalid-custom-field")) {
        return i18n("Invalid custom field");
    } else if (str == QLatin1String("error-invalid-custom-field-name")) {
        return i18n("Invalid custom field name. Use only letters, numbers, hyphens and underscores.");
    } else if (str == QLatin1String("error-invalid-date")) {
        return i18n("Invalid date provided.");
    } else if (str == QLatin1String("error-invalid-description")) {
        return i18n("Invalid description");
    } else if (str == QLatin1String("error-invalid-domain")) {
        return i18n("Invalid domain");
    } else if (str == QLatin1String("error-invalid-email")) {
        return i18n("Invalid email __email__");
    } else if (str == QLatin1String("error-invalid-email-address")) {
        return i18n("Invalid email address");
    } else if (str == QLatin1String("error-invalid-file-height")) {
        return i18n("Invalid file height");
    } else if (str == QLatin1String("error-invalid-file-type")) {
        return i18n("Invalid file type");
    } else if (str == QLatin1String("error-invalid-file-width")) {
        return i18n("Invalid file width");
    } else if (str == QLatin1String("error-invalid-from-address")) {
        return i18n("You informed an invalid FROM address.");
    } else if (str == QLatin1String("error-invalid-integration")) {
        return i18n("Invalid integration");
    } else if (str == QLatin1String("error-invalid-message")) {
        return i18n("Invalid message");
    } else if (str == QLatin1String("error-invalid-method")) {
        return i18n("Invalid method");
    } else if (str == QLatin1String("error-invalid-name")) {
        return i18n("Invalid name");
    } else if (str == QLatin1String("error-invalid-password")) {
        return i18n("Invalid password");
    } else if (str == QLatin1String("error-invalid-permission")) {
        return i18n("Invalid permission");
    } else if (str == QLatin1String("error-invalid-redirectUri")) {
        return i18n("Invalid redirectUri");
    } else if (str == QLatin1String("error-invalid-role")) {
        return i18n("Invalid role");
    } else if (str == QLatin1String("error-invalid-room")) {
        return i18n("Invalid room");
    } else if (str == QLatin1String("error-invalid-room-name")) {
        return i18n("<strong>__room_name__</strong> is not a valid room name");
    } else if (str == QLatin1String("error-invalid-room-type")) {
        return i18n("<strong>__type__</strong> is not a valid room type.");
    } else if (str == QLatin1String("error-invalid-settings")) {
        return i18n("Invalid settings provided");
    } else if (str == QLatin1String("error-invalid-subscription")) {
        return i18n("Invalid subscription");
    } else if (str == QLatin1String("error-invalid-token")) {
        return i18n("Invalid token");
    } else if (str == QLatin1String("error-invalid-triggerWords")) {
        return i18n("Invalid triggerWords");
    } else if (str == QLatin1String("error-invalid-urls")) {
        return i18n("Invalid URLs");
    } else if (str == QLatin1String("error-invalid-user")) {
        return i18n("Invalid user");
    } else if (str == QLatin1String("error-invalid-username")) {
        return i18n("Invalid username");
    } else if (str == QLatin1String("error-invalid-webhook-response")) {
        return i18n("The webhook URL responded with a status other than 200");
    } else if (str == QLatin1String("error-message-deleting-blocked")) {
        return i18n("Message deleting is blocked");
    } else if (str == QLatin1String("error-message-editing-blocked")) {
        return i18n("Message editing is blocked");
    } else if (str == QLatin1String("error-message-size-exceeded")) {
        return i18n("Message size exceeds Message_MaxAllowedSize");
    } else if (str == QLatin1String("error-missing-unsubscribe-link")) {
        return i18n("You must provide the [unsubscribe] link.");
    } else if (str == QLatin1String("error-no-tokens-for-this-user")) {
        return i18n("There are no tokens for this user");
    } else if (str == QLatin1String("error-not-allowed")) {
        return i18n("Not allowed");
    } else if (str == QLatin1String("error-not-authorized")) {
        return i18n("Not authorized");
    } else if (str == QLatin1String("error-password-policy-not-met")) {
        return i18n("Password does not meet the server's policy");
    } else if (str == QLatin1String("error-password-policy-not-met-maxLength")) {
        return i18n("Password does not meet the server's policy of maximum length (password too long)");
    } else if (str == QLatin1String("error-password-policy-not-met-minLength")) {
        return i18n("Password does not meet the server's policy of minimum length (password too short)");
    } else if (str == QLatin1String("error-password-policy-not-met-oneLowercase")) {
        return i18n("Password does not meet the server's policy of at least one lowercase character");
    } else if (str == QLatin1String("error-password-policy-not-met-oneNumber")) {
        return i18n("Password does not meet the server's policy of at least one numerical character");
    } else if (str == QLatin1String("error-password-policy-not-met-oneSpecial")) {
        return i18n("Password does not meet the server's policy of at least one special character");
    } else if (str == QLatin1String("error-password-policy-not-met-oneUppercase")) {
        return i18n("Password does not meet the server's policy of at least one uppercase character");
    } else if (str == QLatin1String("error-password-policy-not-met-repeatingCharacters")) {
        return i18n("Password does not meet the server's policy of forbidden repeating characters (you have too many of the same characters next to each other)");
    } else if (str == QLatin1String("error-push-disabled")) {
        return i18n("Push is disabled");
    } else if (str == QLatin1String("error-remove-last-owner")) {
        return i18n("This is the last owner. Please set a new owner before removing this one.");
    } else if (str == QLatin1String("error-role-in-use")) {
        return i18n("Cannot delete role because it's in use");
    } else if (str == QLatin1String("error-role-name-required")) {
        return i18n("Role name is required");
    } else if (str == QLatin1String("error-room-is-not-closed")) {
        return i18n("Room is not closed");
    } else if (str == QLatin1String("error-the-field-is-required")) {
        return i18n("The field __field__ is required.");
    } else if (str == QLatin1String("error-this-is-not-a-livechat-room")) {
        return i18n("This is not a Livechat room");
    } else if (str == QLatin1String("error-personal-access-tokens-are-current-disabled")) {
        return i18n("Personal Access Tokens are currently disabled");
    } else if (str == QLatin1String("error-token-already-exists")) {
        return i18n("A token with this name already exists");
    } else if (str == QLatin1String("error-token-does-not-exists")) {
        return i18n("Token does not exists");
    } else if (str == QLatin1String("error-too-many-requests")) {
        return i18n("Error, too many requests. Please slow down. You must wait __seconds__ seconds before trying again.");
    } else if (str == QLatin1String("error-user-has-no-roles")) {
        return i18n("User has no roles");
    } else if (str == QLatin1String("error-user-is-not-activated")) {
        return i18n("User is not activated");
    } else if (str == QLatin1String("error-user-limit-exceeded")) {
        return i18n("The number of users you are trying to invite to #channel_name exceeds the limit set by the administrator");
    } else if (str == QLatin1String("error-user-not-in-room")) {
        return i18n("User is not in this room");
    } else if (str == QLatin1String("error-logged-user-not-in-room")) {
        return i18n("You are not in the room `%s`");
    } else if (str == QLatin1String("error-user-registration-disabled")) {
        return i18n("User registration is disabled");
    } else if (str == QLatin1String("error-user-registration-secret")) {
        return i18n("User registration is only allowed via Secret URL");
    } else if (str == QLatin1String("error-you-are-last-owner")) {
        return i18n("You are the last owner. Please set new owner before leaving the room.");
    } else if (str == QLatin1String("error-room-archived")) {
        return i18n("The private group is archived");
    } else {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << " unknown error type " << str;
        return {};
    }
}

QString RestApiAbstractJob::jobName() const
{
    return {};
}

QueryParameters::QueryParameters()
{
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
    return (mCount >= 0) || (mOffset >= 0) || (!mSorting.isEmpty());
}

QMap<QString, QueryParameters::SortOrder> QueryParameters::sorting() const
{
    return mSorting;
}

void QueryParameters::setSorting(const QMap<QString, QueryParameters::SortOrder> &sorting)
{
    mSorting = sorting;
}
