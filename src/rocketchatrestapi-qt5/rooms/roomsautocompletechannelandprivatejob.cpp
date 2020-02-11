/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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

RoomsAutocompleteChannelAndPrivateJob::~RoomsAutocompleteChannelAndPrivateJob()
{
}

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
    QNetworkReply *reply = mNetworkAccessManager->get(request());
    connect(reply, &QNetworkReply::finished, this, &RoomsAutocompleteChannelAndPrivateJob::slotRoomsAutoCompleteChannelAndPrivateFinished);
    addLoggerInfo(QByteArrayLiteral("RoomsAutocompleteChannelAndPrivateJob: Ask all rooms"));
    return true;
}

void RoomsAutocompleteChannelAndPrivateJob::slotRoomsAutoCompleteChannelAndPrivateFinished()
{
    auto *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("RoomsAutocompleteChannelAndPrivateJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT roomsAutoCompleteChannelAndPrivateDone(replyObject);
        } else {
            emitFailedMessage(replyObject);
            addLoggerWarning(QByteArrayLiteral("RoomsAutocompleteChannelAndPrivateJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

QNetworkRequest RoomsAutocompleteChannelAndPrivateJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RoomsAutocompleteChannelAndPrivate);
    QUrlQuery queryUrl;
    //TODO add support for exception.
    const QString val = QStringLiteral("{\"name\": \"%1\"}").arg(mRoomsAutocompleteInfo.name);
    queryUrl.addQueryItem(QStringLiteral("selector"), val);
    url.setQuery(queryUrl);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

RoomsAutocompleteChannelAndPrivateJob::RoomsAutocompleteChannelAndPrivateInfo RoomsAutocompleteChannelAndPrivateJob::roomsCompleterInfo() const
{
    return mRoomsAutocompleteInfo;
}

void RoomsAutocompleteChannelAndPrivateJob::setRoomsCompleterInfo(const RoomsAutocompleteChannelAndPrivateJob::RoomsAutocompleteChannelAndPrivateInfo &roomCompleterInfo)
{
    mRoomsAutocompleteInfo = roomCompleterInfo;
}

bool RoomsAutocompleteChannelAndPrivateJob::RoomsAutocompleteChannelAndPrivateInfo::isValid() const
{
    //FIXME
    return !name.isEmpty();
}
