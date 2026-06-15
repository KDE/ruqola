/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "customsoundsupdatejob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <KLocalizedString>
#include <QFile>
#include <QHttpMultiPart>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

#include <QMimeDatabase>
#include <QNetworkReply>
using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
CustomSoundsUpdateJob::CustomSoundsUpdateJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

CustomSoundsUpdateJob::~CustomSoundsUpdateJob() = default;

bool CustomSoundsUpdateJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("CustomSoundsUpdateJob::start");

    const QString fileNameAsLocalFile = mSoundInfo.fileNameUrl.toLocalFile();
    auto file = new QFile(fileNameAsLocalFile);
    if (!file->open(QIODevice::ReadOnly)) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << " Impossible to open filename " << mSoundInfo.fileNameUrl;
        Q_EMIT failed(i18n("File not found \'%1\'", fileNameAsLocalFile), {});
        delete file;
        deleteLater();
        return false;
    }
    const QMimeDatabase db;
    const QMimeType mimeType = db.mimeTypeForFile(fileNameAsLocalFile);

    auto multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    if (!mSoundInfo.fileNameUrl.isEmpty()) {
        QHttpPart filePart;
        filePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(mimeType.name()));
        const QString filePartInfo = u"form-data; name=\"sound\"; filename=\"%1\""_s.arg(mSoundInfo.fileNameUrl.fileName());
        filePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(filePartInfo));

        filePart.setBodyDevice(file);
        file->setParent(multiPart); // we cannot delete the file now, so delete it with the multiPart
        multiPart->append(filePart);
    }

    QHttpPart identifierPart;
    identifierPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"_id\""_L1));
    identifierPart.setBody(mSoundInfo.soundId.toUtf8());
    multiPart->append(identifierPart);

    QHttpPart namePart;
    namePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"name\""_L1));
    namePart.setBody(mSoundInfo.name.toUtf8());
    multiPart->append(namePart);

    mReply = networkAccessManager()->post(request(), multiPart);
    connect(mReply.data(), &QNetworkReply::finished, this, &CustomSoundsUpdateJob::slotCustomSoundUpdateFinished);
    multiPart->setParent(mReply); // delete the multiPart with the reply
    return true;
}

void CustomSoundsUpdateJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("CustomSoundsUpdateJob success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT customSoundUpdateDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("CustomSoundsUpdateJob problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

const CustomSoundsUpdateJob::SoundInfo &CustomSoundsUpdateJob::soundInfo() const
{
    return mSoundInfo;
}

void CustomSoundsUpdateJob::setSoundInfo(const SoundInfo &newEmojiInfo)
{
    mSoundInfo = newEmojiInfo;
}

void CustomSoundsUpdateJob::slotCustomSoundUpdateFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());

    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject["success"_L1].toBool()) {
            addLoggerInfo("CustomSoundsUpdateJob success: "_ba + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT customSoundUpdateDone(replyObject);
        } else {
            emitFailedMessage(reply->errorString(), replyObject);
            addLoggerWarning("CustomSoundsUpdateJob problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

bool CustomSoundsUpdateJob::requireHttpAuthentication() const
{
    return true;
}

bool CustomSoundsUpdateJob::canStart() const
{
    if (!mSoundInfo.isValid()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "CustomSoundsUpdateJob: mEmojiInfo is not valid";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QNetworkRequest CustomSoundsUpdateJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::CustomSoundsUpdate);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

bool CustomSoundsUpdateJob::SoundInfo::isValid() const
{
    return !name.isEmpty() && !fileNameUrl.isEmpty() && !soundId.isEmpty();
}

#include "moc_customsoundsupdatejob.cpp"
