/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usersupdateownbasicinfojob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <KLocalizedString>

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace Qt::Literals::StringLiterals;
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
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("UsersUpdateOwnBasicInfo: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT updateOwnBasicInfoDone();
        if (mUpdateOwnBasicInfo.type & UpdateOwnBasicInfo::BasicInfoType::Password) {
            Q_EMIT passwordChanged();
        }
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("UsersUpdateOwnBasicInfo: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
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
        dataObj["email"_L1] = mUpdateOwnBasicInfo.email;
    }
    if (mUpdateOwnBasicInfo.type & UpdateOwnBasicInfo::BasicInfoType::UserName) {
        dataObj["username"_L1] = mUpdateOwnBasicInfo.userName;
    }
    if (mUpdateOwnBasicInfo.type & UpdateOwnBasicInfo::BasicInfoType::NickName) {
        dataObj["nickname"_L1] = mUpdateOwnBasicInfo.nickName;
    }
    if (mUpdateOwnBasicInfo.type & UpdateOwnBasicInfo::BasicInfoType::StatusText) {
        dataObj["statusText"_L1] = mUpdateOwnBasicInfo.statusText;
    }
    if (mUpdateOwnBasicInfo.type & UpdateOwnBasicInfo::BasicInfoType::Password) {
        dataObj["currentPassword"_L1] = QLatin1StringView(mUpdateOwnBasicInfo.currentPassword);
        dataObj["newPassword"_L1] = mUpdateOwnBasicInfo.newPassword;
    }

    jsonObj["data"_L1] = dataObj;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

bool UsersUpdateOwnBasicInfoJob::UpdateOwnBasicInfo::isValid() const
{
    return static_cast<int>(type) != static_cast<int>(UsersUpdateOwnBasicInfoJob::UpdateOwnBasicInfo::BasicInfoType::Unknown);
}

QDebug operator<<(QDebug d, const RocketChatRestApi::UsersUpdateOwnBasicInfoJob::UpdateOwnBasicInfo &t)
{
    d.space() << "type " << t.type;
    d.space() << "email " << t.email;
    d.space() << "name " << t.name;
    d.space() << "userName " << t.userName;
    d.space() << "statusText " << t.statusText;
    d.space() << "currentPassword " << t.currentPassword;
    d.space() << "newPassword " << t.newPassword;

    return d;
}

#include "moc_usersupdateownbasicinfojob.cpp"
