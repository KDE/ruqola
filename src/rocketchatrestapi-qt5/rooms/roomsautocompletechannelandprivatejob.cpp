/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomsautocompletechannelandprivatejob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrlQuery>
using namespace RocketChatRestApi;
RoomsAutocompleteChannelAndPrivateJob::RoomsAutocompleteChannelAndPrivateJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

RoomsAutocompleteChannelAndPrivateJob::~RoomsAutocompleteChannelAndPrivateJob() = default;

bool RoomsAutocompleteChannelAndPrivateJob::requireHttpAuthentication() const
{
    return true;
}

bool RoomsAutocompleteChannelAndPrivateJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start RoomsAutocompleteChannelAndPrivateJob job";
        deleteLater();
        return false;
    }
    submitGetRequest();

    addStartRestApiInfo(QByteArrayLiteral("RoomsAutocompleteChannelAndPrivateJob: Ask all rooms"));
    return true;
}

void RoomsAutocompleteChannelAndPrivateJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("RoomsAutocompleteChannelAndPrivateJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT roomsAutoCompleteChannelAndPrivateDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("RoomsAutocompleteChannelAndPrivateJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

QNetworkRequest RoomsAutocompleteChannelAndPrivateJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RoomsAutocompleteChannelAndPrivate);
    QUrlQuery queryUrl;
    // TODO add support for exception.
    // const QString val = QStringLiteral("{\"term\": \"%1\"}").arg(mUsersCompleterInfo.pattern);
    const QString val = QStringLiteral("{\"name\": \"%1\"}").arg(mRoomsAutocompleteInfo.name);
    queryUrl.addQueryItem(QStringLiteral("selector"), val);
    url.setQuery(queryUrl);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    // qDebug() << " url " << url;
    return request;
}

RoomsAutocompleteChannelAndPrivateJob::RoomsAutocompleteChannelAndPrivateInfo RoomsAutocompleteChannelAndPrivateJob::roomsCompleterInfo() const
{
    return mRoomsAutocompleteInfo;
}

void RoomsAutocompleteChannelAndPrivateJob::setRoomsCompleterInfo(
    const RoomsAutocompleteChannelAndPrivateJob::RoomsAutocompleteChannelAndPrivateInfo &roomCompleterInfo)
{
    mRoomsAutocompleteInfo = roomCompleterInfo;
}

bool RoomsAutocompleteChannelAndPrivateJob::RoomsAutocompleteChannelAndPrivateInfo::isValid() const
{
    // FIXME
    return !name.isEmpty();
}
