/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emojicustomcreatejob.h"

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
EmojiCustomCreateJob::EmojiCustomCreateJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

EmojiCustomCreateJob::~EmojiCustomCreateJob() = default;

bool EmojiCustomCreateJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("EmojiCustomCreateJob::start");
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

    QHttpPart namePart;
    namePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(QLatin1String("form-data; name=\"name\"")));
    namePart.setBody(mEmojiInfo.name.toUtf8());
    multiPart->append(std::move(namePart));

    QHttpPart aliasesPart;
    aliasesPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(QLatin1String("form-data; name=\"aliases\"")));
    aliasesPart.setBody(mEmojiInfo.alias.toUtf8());
    multiPart->append(std::move(aliasesPart));

    mReply = networkAccessManager()->post(request(), multiPart);
    connect(mReply.data(), &QNetworkReply::finished, this, &EmojiCustomCreateJob::slotEmojiCustomCreateFinished);
    multiPart->setParent(mReply); // delete the multiPart with the reply
    return true;
}

void EmojiCustomCreateJob::slotEmojiCustomCreateFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());

    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QLatin1String("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("EmojiCustomCreateJob success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT emojiCustomCreateDone(replyObject);
        } else {
            emitFailedMessage(reply->errorString(), replyObject);
            addLoggerWarning(QByteArrayLiteral("EmojiCustomCreateJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

const EmojiCustomCreateJob::EmojiInfo &EmojiCustomCreateJob::emojiInfo() const
{
    return mEmojiInfo;
}

void EmojiCustomCreateJob::setEmojiInfo(const EmojiInfo &newEmojiInfo)
{
    mEmojiInfo = newEmojiInfo;
}

bool EmojiCustomCreateJob::requireHttpAuthentication() const
{
    return true;
}

bool EmojiCustomCreateJob::canStart() const
{
    if (!mEmojiInfo.isValid()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "EmojiCustomCreateJob: mEmojiInfo is not valid";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QNetworkRequest EmojiCustomCreateJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::EmojiCustomCreate);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request, false);
    return request;
}

bool EmojiCustomCreateJob::EmojiInfo::isValid() const
{
    // Alias is optional
    return !name.isEmpty() && !fileNameUrl.isEmpty();
}

QString EmojiCustomCreateJob::errorMessage(const QString &str, const QJsonObject &details)
{
    if (str == QLatin1String("Custom_Emoji_Error_Name_Or_Alias_Already_In_Use")) {
        return i18n("The custom emoji or one of its aliases is already in use.");
    }
    return RestApiAbstractJob::errorMessage(str, details);
}

#include "moc_emojicustomcreatejob.cpp"
