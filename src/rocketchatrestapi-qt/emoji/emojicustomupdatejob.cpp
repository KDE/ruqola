/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emojicustomupdatejob.h"

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
EmojiCustomUpdateJob::EmojiCustomUpdateJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

EmojiCustomUpdateJob::~EmojiCustomUpdateJob() = default;

bool EmojiCustomUpdateJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("EmojiCustomUpdateJob::start");

    const QString fileNameAsLocalFile = mEmojiInfo.fileNameUrl.toLocalFile();
    auto file = new QFile(fileNameAsLocalFile);
    if (!file->open(QIODevice::ReadOnly)) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << " Impossible to open filename " << mEmojiInfo.fileNameUrl;
        Q_EMIT failed(i18n("File not found \'%1\'", fileNameAsLocalFile));
        delete file;
        deleteLater();
        return false;
    }
    QMimeDatabase db;
    const QMimeType mimeType = db.mimeTypeForFile(fileNameAsLocalFile);

    auto multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    if (!mEmojiInfo.fileNameUrl.isEmpty()) {
        QHttpPart filePart;
        filePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(mimeType.name()));
        const QString filePartInfo = u"form-data; name=\"emoji\"; filename=\"%1\""_s.arg(mEmojiInfo.fileNameUrl.fileName());
        filePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(filePartInfo));

        filePart.setBodyDevice(file);
        file->setParent(multiPart); // we cannot delete the file now, so delete it with the multiPart
        multiPart->append(filePart);
    }

    QHttpPart identifierPart;
    identifierPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"_id\""_L1));
    identifierPart.setBody(mEmojiInfo.emojiId.toUtf8());
    multiPart->append(identifierPart);

    QHttpPart namePart;
    namePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"name\""_L1));
    namePart.setBody(mEmojiInfo.name.toUtf8());
    multiPart->append(namePart);

    QHttpPart aliasesPart;
    aliasesPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"aliases\""_L1));
    aliasesPart.setBody(mEmojiInfo.alias.toUtf8());
    multiPart->append(aliasesPart);

    mReply = networkAccessManager()->post(request(), multiPart);
    connect(mReply.data(), &QNetworkReply::finished, this, &EmojiCustomUpdateJob::slotEmojiCustomUpdateFinished);
    multiPart->setParent(mReply); // delete the multiPart with the reply
    return true;
}

void EmojiCustomUpdateJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("EmojiCustomUpdateJob success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT emojiCustomUpdateDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("EmojiCustomUpdateJob problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

const EmojiCustomUpdateJob::EmojiInfo &EmojiCustomUpdateJob::emojiInfo() const
{
    return mEmojiInfo;
}

void EmojiCustomUpdateJob::setEmojiInfo(const EmojiInfo &newEmojiInfo)
{
    mEmojiInfo = newEmojiInfo;
}

void EmojiCustomUpdateJob::slotEmojiCustomUpdateFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());

    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject["success"_L1].toBool()) {
            addLoggerInfo("EmojiCustomUpdateJob success: "_ba + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT emojiCustomUpdateDone(replyObject);
        } else {
            emitFailedMessage(reply->errorString(), replyObject);
            addLoggerWarning("EmojiCustomUpdateJob problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

bool EmojiCustomUpdateJob::requireHttpAuthentication() const
{
    return true;
}

bool EmojiCustomUpdateJob::canStart() const
{
    if (!mEmojiInfo.isValid()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "EmojiCustomUpdateJob: mEmojiInfo is not valid";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QNetworkRequest EmojiCustomUpdateJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::EmojiCustomUpdate);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

bool EmojiCustomUpdateJob::EmojiInfo::isValid() const
{
    // Alias is optional
    return !name.isEmpty() && !fileNameUrl.isEmpty() && !emojiId.isEmpty();
}

#include "moc_emojicustomupdatejob.cpp"
