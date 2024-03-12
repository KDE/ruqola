/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usersupdateownbasicinfojob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <KLocalizedString>

#include <QJsonDocument>
#include <QJsonObject>
using namespace RocketChatRestApi;
UsersUpdateOwnBasicInfoJob::UsersUpdateOwnBasicInfoJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

UsersUpdateOwnBasicInfoJob::~UsersUpdateOwnBasicInfoJob() = default;

bool UsersUpdateOwnBasicInfoJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("UsersUpdateOwnBasicInfo::start");
    submitPostRequest(json());
    return true;
}

void UsersUpdateOwnBasicInfoJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject[QLatin1StringView("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("UsersUpdateOwnBasicInfo: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT updateOwnBasicInfoDone();
        if (mUpdateOwnBasicInfo.type & UpdateOwnBasicInfo::BasicInfoType::Password) {
            Q_EMIT passwordChanged();
        }
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("UsersUpdateOwnBasicInfo: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

UsersUpdateOwnBasicInfoJob::UpdateOwnBasicInfo UsersUpdateOwnBasicInfoJob::updateOwnBasicInfo() const
{
    return mUpdateOwnBasicInfo;
}

void UsersUpdateOwnBasicInfoJob::setUpdateOwnBasicInfo(const UpdateOwnBasicInfo &updateOwnBasicInfo)
{
    mUpdateOwnBasicInfo = updateOwnBasicInfo;
}

bool UsersUpdateOwnBasicInfoJob::requireHttpAuthentication() const
{
    return true;
}

bool UsersUpdateOwnBasicInfoJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (!mUpdateOwnBasicInfo.isValid()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "UsersUpdateOwnBasicInfo: mUpdateOwnBasicInfo is not valid.";
        return false;
    }
    return true;
}

QNetworkRequest UsersUpdateOwnBasicInfoJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UsersUpdateOwnBasicInfo);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument UsersUpdateOwnBasicInfoJob::json() const
{
    QJsonObject jsonObj;
    QJsonObject dataObj;
    if (mUpdateOwnBasicInfo.type & UpdateOwnBasicInfo::BasicInfoType::Email) {
        dataObj[QLatin1StringView("email")] = mUpdateOwnBasicInfo.email;
    }
    if (mUpdateOwnBasicInfo.type & UpdateOwnBasicInfo::BasicInfoType::UserName) {
        dataObj[QLatin1StringView("username")] = mUpdateOwnBasicInfo.userName;
    }
    if (mUpdateOwnBasicInfo.type & UpdateOwnBasicInfo::BasicInfoType::NickName) {
        dataObj[QLatin1StringView("nickname")] = mUpdateOwnBasicInfo.nickName;
    }
    if (mUpdateOwnBasicInfo.type & UpdateOwnBasicInfo::BasicInfoType::StatusText) {
        dataObj[QLatin1StringView("statusText")] = mUpdateOwnBasicInfo.statusText;
    }
    if (mUpdateOwnBasicInfo.type & UpdateOwnBasicInfo::BasicInfoType::Password) {
        dataObj[QLatin1StringView("currentPassword")] = QString::fromLatin1(mUpdateOwnBasicInfo.currentPassword);
        dataObj[QLatin1StringView("newPassword")] = mUpdateOwnBasicInfo.newPassword;
    }

    jsonObj[QLatin1StringView("data")] = dataObj;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

bool UsersUpdateOwnBasicInfoJob::UpdateOwnBasicInfo::isValid() const
{
    return static_cast<int>(type) != static_cast<int>(UsersUpdateOwnBasicInfoJob::UpdateOwnBasicInfo::BasicInfoType::Unknown);
}

QDebug operator<<(QDebug d, const RocketChatRestApi::UsersUpdateOwnBasicInfoJob::UpdateOwnBasicInfo &t)
{
    d << " type " << t.type;
    d << " email " << t.email;
    d << " name " << t.name;
    d << " userName " << t.userName;
    d << " statusText " << t.statusText;
    d << " currentPassword " << t.currentPassword;
    d << " newPassword " << t.newPassword;

    return d;
}

#include "moc_usersupdateownbasicinfojob.cpp"
