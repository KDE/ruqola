/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "setavatarjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <KLocalizedString>

#include <QFile>
#include <QHttpMultiPart>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMimeDatabase>
#include <QNetworkReply>
using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
SetAvatarJob::SetAvatarJob(QObject *parent)
    : UserBaseJob(parent)
{
}

SetAvatarJob::~SetAvatarJob() = default;

bool SetAvatarJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("SetAvatarJob::start");
    if (!mAvatarInfo.mAvatarUrl.isEmpty()) {
        submitPostRequest(json());
    } else {
        const QString fileNameAsLocalFile = mAvatarInfo.mImageUrl.toLocalFile();
        auto file = new QFile(fileNameAsLocalFile);
        if (!file->open(QIODevice::ReadOnly)) {
            qCWarning(ROCKETCHATQTRESTAPI_LOG) << " Impossible to open filename " << mAvatarInfo.mImageUrl;
            Q_EMIT failed(i18n("File not found \'%1\'", fileNameAsLocalFile));
            delete file;
            deleteLater();
            return false;
        }
        QMimeDatabase db;
        const QMimeType mimeType = db.mimeTypeForFile(fileNameAsLocalFile);

        auto multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

        QHttpPart filePart;
        filePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(mimeType.name()));
        const QString filePartInfo = u"form-data; name=\"image\"; filename=\"%1\""_s.arg(mAvatarInfo.mImageUrl.fileName());
        filePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(filePartInfo));

        filePart.setBodyDevice(file);
        file->setParent(multiPart); // we cannot delete the file now, so delete it with the multiPart
        multiPart->append(filePart);

        QHttpPart userPart;
        userPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"userId\""_L1));
        userPart.setBody(userId().toUtf8());
        multiPart->append(userPart);

        mReply = networkAccessManager()->post(request(), multiPart);
        // connect(reply, &QNetworkReply::uploadProgress, this, &UploadFileJob::slotUploadProgress);
        connect(mReply.data(), &QNetworkReply::finished, this, &SetAvatarJob::slotSetAvatar);
        multiPart->setParent(mReply); // delete the multiPart with the reply
    }
    return true;
}

void SetAvatarJob::slotSetAvatar()
{
    auto reply = mReply;
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        onPostRequestResponse(mReply->errorString(), replyJson);
        reply->deleteLater();
    }
    deleteLater();
}

void SetAvatarJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("SetAvatarJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT setAvatarDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("SetAvatarJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

SetAvatarJob::SetAvatarInfo SetAvatarJob::avatarInfo() const
{
    return mAvatarInfo;
}

void SetAvatarJob::setAvatarInfo(const SetAvatarInfo &avatarInfo)
{
    mAvatarInfo = avatarInfo;
}

QString SetAvatarJob::errorMessage(const QString &str, const QJsonObject &details)
{
    if (str == "error-avatar-invalid-url"_L1) {
        const QString url = details["url"_L1].toString();
        return i18n("Invalid avatar URL: %1", url);
    } else if (str == "error-avatar-url-handling"_L1) {
        const QString url = details["url"_L1].toString();
        const QString username = details["username"_L1].toString();
        return i18n("Error while handling avatar setting from a URL \"%1\" for %2", url, username);
    }

    return RestApiAbstractJob::errorMessage(str, details);
}

bool SetAvatarJob::requireHttpAuthentication() const
{
    return true;
}

bool SetAvatarJob::canStart() const
{
    if (!hasUserIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "SetAvatarJob: identifier is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (!mAvatarInfo.isValid()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "SetAvatarJob: mAvatarInfo is not valid";
        return false;
    }
    return true;
}

QNetworkRequest SetAvatarJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UsersSetAvatar);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request, !mAvatarInfo.mAvatarUrl.isEmpty()); // Don't show "json" when we send image
    return request;
}

QJsonDocument SetAvatarJob::json() const
{
    QJsonObject jsonObj;
    jsonObj["avatarUrl"_L1] = mAvatarInfo.mAvatarUrl;
    generateJson(jsonObj);
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

bool SetAvatarJob::SetAvatarInfo::isValid() const
{
    return !mAvatarUrl.isEmpty() || !mImageUrl.isEmpty();
}

#include "moc_setavatarjob.cpp"
