/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "customsoundscreatejob.h"

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
using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
CustomSoundsCreateJob::CustomSoundsCreateJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

CustomSoundsCreateJob::~CustomSoundsCreateJob() = default;

bool CustomSoundsCreateJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("CustomSoundsCreateJob::start");
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

    QHttpPart namePart;
    namePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"name\""_L1));
    namePart.setBody(mSoundInfo.name.toUtf8());
    multiPart->append(namePart);

    mReply = networkAccessManager()->post(request(), multiPart);
    connect(mReply.data(), &QNetworkReply::finished, this, &CustomSoundsCreateJob::slotCustomSoundCreateFinished);
    multiPart->setParent(mReply); // delete the multiPart with the reply
    return true;
}

void CustomSoundsCreateJob::slotCustomSoundCreateFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());

    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject["success"_L1].toBool()) {
            addLoggerInfo("CustomSoundsCreateJob success: "_ba + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT customSoundCreateDone(replyObject);
        } else {
            emitFailedMessage(reply->errorString(), replyObject);
            addLoggerWarning("CustomSoundsCreateJob problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

const CustomSoundsCreateJob::SoundInfo &CustomSoundsCreateJob::soundInfo() const
{
    return mSoundInfo;
}

void CustomSoundsCreateJob::setSoundInfo(const SoundInfo &newSoundInfo)
{
    mSoundInfo = newSoundInfo;
}

bool CustomSoundsCreateJob::requireHttpAuthentication() const
{
    return true;
}

bool CustomSoundsCreateJob::canStart() const
{
    if (!mSoundInfo.isValid()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "CustomSoundsCreateJob: mSoundInfo is not valid";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QNetworkRequest CustomSoundsCreateJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::CustomSoundsCreate);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

bool CustomSoundsCreateJob::SoundInfo::isValid() const
{
    return !name.isEmpty() && !fileNameUrl.isEmpty();
}

#include "moc_customsoundscreatejob.cpp"
