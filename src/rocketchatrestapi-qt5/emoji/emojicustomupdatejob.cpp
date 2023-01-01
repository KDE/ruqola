/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

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
#include <QMimeDatabase>
#include <QNetworkReply>
using namespace RocketChatRestApi;
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
        const QString filePartInfo = QStringLiteral("form-data; name=\"emoji\"; filename=\"%1\"").arg(mEmojiInfo.fileNameUrl.fileName());
        filePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(filePartInfo));

        filePart.setBodyDevice(file);
        file->setParent(multiPart); // we cannot delete the file now, so delete it with the multiPart
        multiPart->append(filePart);
    }

    QHttpPart identifierPart;
    identifierPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(QLatin1String("form-data; name=\"_id\"")));
    identifierPart.setBody(mEmojiInfo.emojiId.toUtf8());
    multiPart->append(std::move(identifierPart));

    QHttpPart namePart;
    namePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(QLatin1String("form-data; name=\"name\"")));
    namePart.setBody(mEmojiInfo.name.toUtf8());
    multiPart->append(std::move(namePart));

    QHttpPart aliasesPart;
    aliasesPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(QLatin1String("form-data; name=\"aliases\"")));
    aliasesPart.setBody(mEmojiInfo.alias.toUtf8());
    multiPart->append(std::move(aliasesPart));

    mReply = networkAccessManager()->post(request(), multiPart);
    connect(mReply.data(), &QNetworkReply::finished, this, &EmojiCustomUpdateJob::slotEmojiCustomUpdateFinished);
    multiPart->setParent(mReply); // delete the multiPart with the reply
    return true;
}

void EmojiCustomUpdateJob::onPostRequestResponse(const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("EmojiCustomUpdateJob success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT emojiCustomUpdateDone(replyObject);
    } else {
        emitFailedMessage(replyObject);
        addLoggerWarning(QByteArrayLiteral("EmojiCustomUpdateJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
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

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("EmojiCustomUpdateJob success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT emojiCustomUpdateDone(replyObject);
        } else {
            emitFailedMessage(replyObject);
            addLoggerWarning(QByteArrayLiteral("EmojiCustomUpdateJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
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
