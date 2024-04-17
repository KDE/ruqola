/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "saveroomsettingsjob.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <KLocalizedString>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
using namespace RocketChatRestApi;
SaveRoomSettingsJob::SaveRoomSettingsJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

SaveRoomSettingsJob::~SaveRoomSettingsJob() = default;

bool SaveRoomSettingsJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("SaveRoomSettingsJob::start");
    submitPostRequest(json());

    return true;
}

void SaveRoomSettingsJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo(QByteArrayLiteral("SaveRoomSettingsJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT saveRoomSettingsDone(replyObject.value(QStringLiteral("rid")).toString());
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("SaveRoomSettingsJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

SaveRoomSettingsJob::SaveRoomSettingsInfo SaveRoomSettingsJob::saveRoomSettingsInfo() const
{
    return mSaveRoomSettingsInfo;
}

void SaveRoomSettingsJob::setSaveRoomSettingsInfo(const SaveRoomSettingsInfo &saveRoomSettingsInfo)
{
    mSaveRoomSettingsInfo = saveRoomSettingsInfo;
}

QString SaveRoomSettingsJob::errorMessage(const QString &str, const QJsonObject &detail)
{
    if (str == "error-invalid-room-name"_L1) {
        return i18n("\'%1\' is not a valid room name", detail.value(QStringLiteral("channel_name")).toString());
    } else if (str == "error-action-not-allowed"_L1) {
        const QString detailActionStr = detail[QStringLiteral("action")].toString();
        // qDebug() << " detailActionStr " << detailActionStr;
        if (detailActionStr == "Change_Room_Encrypted"_L1) {
            return i18n("Only groups or direct channels can enable encryption");
        } else if (detailActionStr == "Editing_room"_L1) {
            return i18n("Room does not have retention policy");
        }
    }
    return RestApiAbstractJob::errorMessage(str, detail);
}

bool SaveRoomSettingsJob::requireHttpAuthentication() const
{
    return true;
}

bool SaveRoomSettingsJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (!mSaveRoomSettingsInfo.isValid()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "SaveRoomSettingsJob: mRoomId is empty";
        return false;
    }
    return true;
}

QNetworkRequest SaveRoomSettingsJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RoomsSaveSettings);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument SaveRoomSettingsJob::json() const
{
    QJsonObject jsonObj;
    jsonObj["rid"_L1] = QLatin1StringView(mSaveRoomSettingsInfo.roomId);
    if (mSaveRoomSettingsInfo.mSettingsWillBeChanged & SaveRoomSettingsInfo::SystemMessages) {
        const QJsonArray systemSettingsArray = QJsonArray::fromStringList(mSaveRoomSettingsInfo.systemMessages);
        jsonObj["systemMessages"_L1] = systemSettingsArray;
    }
    if (mSaveRoomSettingsInfo.mSettingsWillBeChanged & SaveRoomSettingsInfo::RetentionEnabled) {
        jsonObj["retentionEnabled"_L1] = mSaveRoomSettingsInfo.retentionEnabled;
    }
    if (mSaveRoomSettingsInfo.mSettingsWillBeChanged & SaveRoomSettingsInfo::RetentionExcludePinned) {
        jsonObj["retentionExcludePinned"_L1] = mSaveRoomSettingsInfo.retentionExcludePinned;
    }
    if (mSaveRoomSettingsInfo.mSettingsWillBeChanged & SaveRoomSettingsInfo::RetentionFilesOnly) {
        jsonObj["retentionFilesOnly"_L1] = mSaveRoomSettingsInfo.retentionFilesOnly;
    }
    if (mSaveRoomSettingsInfo.mSettingsWillBeChanged & SaveRoomSettingsInfo::RetentionIgnoreThreads) {
        jsonObj["retentionIgnoreThreads"_L1] = mSaveRoomSettingsInfo.retentionIgnoreThreads;
    }
    if (mSaveRoomSettingsInfo.mSettingsWillBeChanged & SaveRoomSettingsInfo::RetentionOverrideGlobal) {
        jsonObj["retentionOverrideGlobal"_L1] = mSaveRoomSettingsInfo.retentionOverrideGlobal;
    }
    if (mSaveRoomSettingsInfo.mSettingsWillBeChanged & SaveRoomSettingsInfo::RetentionMaxAge) {
        jsonObj["retentionMaxAge"_L1] = mSaveRoomSettingsInfo.retentionMaxAge;
    }
    if (mSaveRoomSettingsInfo.mSettingsWillBeChanged & SaveRoomSettingsInfo::RoomName) {
        jsonObj["roomName"_L1] = mSaveRoomSettingsInfo.roomName;
    }
    if (mSaveRoomSettingsInfo.mSettingsWillBeChanged & SaveRoomSettingsInfo::RoomTopic) {
        jsonObj["roomTopic"_L1] = mSaveRoomSettingsInfo.roomTopic;
    }
    if (mSaveRoomSettingsInfo.mSettingsWillBeChanged & SaveRoomSettingsInfo::RoomAnnouncement) {
        jsonObj["roomAnnouncement"_L1] = mSaveRoomSettingsInfo.roomAnnouncement;
    }
    if (mSaveRoomSettingsInfo.mSettingsWillBeChanged & SaveRoomSettingsInfo::RoomDescription) {
        jsonObj["roomDescription"_L1] = mSaveRoomSettingsInfo.roomDescription;
    }
    QJsonObject jsonObjFavorite;
    if (mSaveRoomSettingsInfo.mSettingsWillBeChanged & SaveRoomSettingsInfo::Favorite) {
        jsonObjFavorite["favorite"_L1] = mSaveRoomSettingsInfo.favorite;
    }
    if (mSaveRoomSettingsInfo.mSettingsWillBeChanged & SaveRoomSettingsInfo::DefaultValue) {
        jsonObjFavorite["defaultValue"_L1] = mSaveRoomSettingsInfo.defaultValue;
    }
    jsonObj["favorite"_L1] = jsonObjFavorite;

    jsonObj["roomType"_L1] = mSaveRoomSettingsInfo.roomType;
    if (mSaveRoomSettingsInfo.mSettingsWillBeChanged & SaveRoomSettingsInfo::JoinCode) {
        jsonObj["joinCode"_L1] = mSaveRoomSettingsInfo.joinCode;
    }
    if (mSaveRoomSettingsInfo.mSettingsWillBeChanged & SaveRoomSettingsInfo::ReadOnly) {
        jsonObj["readOnly"_L1] = mSaveRoomSettingsInfo.readOnly;
    }
    if (mSaveRoomSettingsInfo.mSettingsWillBeChanged & SaveRoomSettingsInfo::Encrypted) {
        jsonObj["encrypted"_L1] = mSaveRoomSettingsInfo.encrypted;
    }
    if (mSaveRoomSettingsInfo.mSettingsWillBeChanged & SaveRoomSettingsInfo::ReactWhenReadOnly) {
        jsonObj["reactWhenReadOnly"_L1] = mSaveRoomSettingsInfo.reactWhenReadOnly;
    }
    if (mSaveRoomSettingsInfo.mSettingsWillBeChanged & SaveRoomSettingsInfo::Featured) {
        jsonObj["featured"_L1] = mSaveRoomSettingsInfo.featured;
    }
    if (mSaveRoomSettingsInfo.mSettingsWillBeChanged & SaveRoomSettingsInfo::RoomAvatar) {
        // "roomAvatar":null if we revert it.
        jsonObj["roomAvatar"_L1] = mSaveRoomSettingsInfo.roomAvatar;
    }

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

bool SaveRoomSettingsJob::SaveRoomSettingsInfo::isValid() const
{
    return !roomId.isEmpty() && (mSettingsWillBeChanged != SettingChanged::Unknown);
}

QDebug operator<<(QDebug d, const RocketChatRestApi::SaveRoomSettingsJob::SaveRoomSettingsInfo &t)
{
    d << "systemMessages : " << t.systemMessages;
    d << "roomId : " << t.roomId;
    d << "roomName : " << t.roomName;
    d << "roomTopic : " << t.roomTopic;
    d << "roomAnnouncement : " << t.roomAnnouncement;
    d << "roomDescription : " << t.roomDescription;
    d << "retentionEnabled : " << t.retentionEnabled;
    d << "retentionExcludePinned : " << t.retentionExcludePinned;
    d << "retentionFilesOnly : " << t.retentionFilesOnly;
    d << "retentionIgnoreThreads : " << t.retentionIgnoreThreads;
    d << "retentionOverrideGlobal : " << t.retentionOverrideGlobal;
    d << "retentionMaxAge : " << t.retentionMaxAge;
    d << "favorite : " << t.favorite;
    d << "roomType : " << t.roomType;
    d << "readOnly : " << t.readOnly;
    d << "encrypted : " << t.encrypted;
    d << "roomAvatar : " << t.roomAvatar;
    d << "featured : " << t.featured;
    // hide password d << "joinCode : " << t.joinCode;
    d << " mSettingsWillBeChanged " << t.mSettingsWillBeChanged;
    return d;
}

#include "moc_saveroomsettingsjob.cpp"
