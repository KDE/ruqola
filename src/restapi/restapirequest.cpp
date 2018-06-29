/*
   Copyright (c) 2017-2018 Montel Laurent <montel@kde.org>

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

#include "restapimethod.h"
#include "restapirequest.h"
#include "ruqola_restapi_debug.h"

#include "serverinfojob.h"
#include "uploadfilejob.h"
#include "settings/privateinfojob.h"
#include "channellistjob.h"
#include "downloadfilejob.h"
#include "spotlightjob.h"

#include "users/getavatarjob.h"
#include "users/setavatarjob.h"
#include "users/forgotpasswordjob.h"
#include "users/usersinfojob.h"
#include "users/getpresencejob.h"

#include "misc/owninfojob.h"

#include "emoji/loademojicustomjob.h"

#include "authentication/logoutjob.h"
#include "authentication/loginjob.h"

#include "chat/starmessagejob.h"
#include "chat/postmessagejob.h"
#include "chat/deletemessagejob.h"
#include "chat/updatemessagejob.h"
#include "chat/reactonmessagejob.h"
#include "chat/searchmessagejob.h"
#include "chat/ignoreuserjob.h"

#include "channels/changechanneltopicjob.h"
#include "channels/changechannelannouncementjob.h"
#include "channels/createchanneljob.h"
#include "channels/leavechanneljob.h"
#include "channels/channelclosejob.h"
#include "channels/channelhistoryjob.h"
#include "channels/changechanneldescriptionjob.h"
#include "channels/archivechanneljob.h"
#include "channels/channelfilesjob.h"
#include "channels/channelinvitejob.h"

#include "groups/changegroupsannouncementjob.h"
#include "groups/changegroupstopicjob.h"
#include "groups/creategroupsjob.h"
#include "groups/leavegroupsjob.h"
#include "groups/changegroupsdescriptionjob.h"
#include "groups/archivegroupsjob.h"
#include "groups/groupsinvitejob.h"

#include "rooms/getroomsjob.h"
#include "rooms/roomfavoritejob.h"
#include "rooms/savenotificationjob.h"

#include "directmessage/createdmjob.h"

#include "subscriptions/markroomasreadjob.h"
#include "subscriptions/markroomasunreadjob.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QNetworkCookie>
#include <QNetworkCookieJar>
#include <ruqolalogger.h>

RestApiRequest::RestApiRequest(QObject *parent)
    : QObject(parent)
{
    mRestApiMethod = new RestApiMethod;
    mCookieJar = new QNetworkCookieJar;
    mNetworkAccessManager = new QNetworkAccessManager(this);
    mNetworkAccessManager->setCookieJar(mCookieJar);
    connect(mNetworkAccessManager, &QNetworkAccessManager::finished, this, &RestApiRequest::slotResult);
    connect(mNetworkAccessManager, &QNetworkAccessManager::sslErrors, this, &RestApiRequest::slotSslErrors);
}

RestApiRequest::~RestApiRequest()
{
    delete mRestApiMethod;
}

void RestApiRequest::setRuqolaLogger(RuqolaLogger *logger)
{
    mRuqolaLogger = logger;
}

void RestApiRequest::initializeCookies()
{
    QString url = serverUrl();
    if (!url.isEmpty()) {
        QString host;
        QStringList lsthost = url.split(QStringLiteral("//"));
        if (lsthost.count() < 2) {
            host = url;
        } else {
            host = lsthost.at(1);
        }

        if (!mUserId.isEmpty()) {
            QNetworkCookie userIdCookie;
            userIdCookie.setDomain(host);
            userIdCookie.setName(QByteArrayLiteral("rc_uid"));
            userIdCookie.setValue(mUserId.toLocal8Bit());
            mCookieJar->insertCookie(userIdCookie);
        }

        if (!mAuthToken.isEmpty()) {
            QNetworkCookie tokenCookie;
            tokenCookie.setDomain(host);
            tokenCookie.setName(QByteArrayLiteral("rc_token"));
            tokenCookie.setValue(mAuthToken.toLocal8Bit());
            mCookieJar->insertCookie(tokenCookie);
        }
    } else {
        qCWarning(RUQOLA_RESTAPI_LOG) << "We can not initialize cookies as server url is empty.";
    }
}

void RestApiRequest::setAuthToken(const QString &authToken)
{
    const bool isChanged = (mAuthToken != authToken);
    mAuthToken = authToken;
    if (isChanged) {
        if (!mAuthToken.isEmpty()) {
            initializeCookies();
        }
    }
}

void RestApiRequest::setUserId(const QString &userId)
{
    const bool isChanged = (mUserId != userId);
    mUserId = userId;
    if (isChanged) {
        if (!mUserId.isEmpty()) {
            initializeCookies();
        }
    }
}

void RestApiRequest::slotResult(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        qCWarning(RUQOLA_RESTAPI_LOG) << " Error reply - "<<reply->errorString();
    }
}

void RestApiRequest::slotSslErrors(QNetworkReply *reply, const QList<QSslError> &error)
{
    qCDebug(RUQOLA_RESTAPI_LOG) << " void RestApiRequest::slotSslErrors(QNetworkReply *reply, const QList<QSslError> &error)" << error.count();
    reply->ignoreSslErrors(error);
}

void RestApiRequest::setPassword(const QString &password)
{
    mPassword = password;
}

void RestApiRequest::setUserName(const QString &userName)
{
    mUserName = userName;
}

QString RestApiRequest::serverUrl() const
{
    return mRestApiMethod->serverUrl();
}

void RestApiRequest::setServerUrl(const QString &serverUrl)
{
    mRestApiMethod->setServerUrl(serverUrl);
}

QString RestApiRequest::authToken() const
{
    return mAuthToken;
}

QString RestApiRequest::userId() const
{
    return mUserId;
}

void RestApiRequest::login()
{
    LoginJob *job = new LoginJob(this);
    connect(job, &LoginJob::loginDone, this, &RestApiRequest::slotLogin);
    initializeRestApiJob(job);
    job->setPassword(mPassword);
    job->setUserName(mUserName);
    if (!job->start()) {
        qCDebug(RUQOLA_RESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::slotLogin(const QString &authToken, const QString &userId)
{
    mAuthToken = authToken;
    mUserId = userId;
}

void RestApiRequest::slotLogout()
{
    mUserId.clear();
    mAuthToken.clear();
    Q_EMIT logoutDone();
}

void RestApiRequest::initializeRestApiJob(RestApiAbstractJob *job)
{
    job->setNetworkAccessManager(mNetworkAccessManager);
    job->setRuqolaLogger(mRuqolaLogger);
    job->setRestApiMethod(mRestApiMethod);
    if (job->requireHttpAuthentication()) {
        job->setAuthToken(mAuthToken);
        job->setUserId(mUserId);
    }
}

void RestApiRequest::logout()
{
    LogoutJob *job = new LogoutJob(this);
    connect(job, &LogoutJob::logoutDone, this, &RestApiRequest::slotLogout);
    initializeRestApiJob(job);
    if (!job->start()) {
        qCDebug(RUQOLA_RESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::channelList()
{
    ChannelListJob *job = new ChannelListJob(this);
    connect(job, &ChannelListJob::channelListDone, this, &RestApiRequest::channelListDone);
    initializeRestApiJob(job);
    if (!job->start()) {
        qCDebug(RUQOLA_RESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::getAvatar(const QString &userId)
{
    GetAvatarJob *job = new GetAvatarJob(this);
    connect(job, &GetAvatarJob::avatar, this, &RestApiRequest::avatar);
    initializeRestApiJob(job);
    job->setAvatarUserId(userId);
    if (!job->start()) {
        qCDebug(RUQOLA_RESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::getPrivateSettings()
{
    PrivateInfoJob *job = new PrivateInfoJob(this);
    connect(job, &PrivateInfoJob::privateInfoDone, this, &RestApiRequest::privateInfoDone);
    initializeRestApiJob(job);
    if (!job->start()) {
        qCDebug(RUQOLA_RESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::getOwnInfo()
{
    OwnInfoJob *job = new OwnInfoJob(this);
    connect(job, &OwnInfoJob::ownInfoDone, this, &RestApiRequest::getOwnInfoDone);
    initializeRestApiJob(job);
    if (!job->start()) {
        qCDebug(RUQOLA_RESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::starMessage(const QString &messageId, bool starred)
{
    StarMessageJob *job = new StarMessageJob(this);
    initializeRestApiJob(job);
    job->setMessageId(messageId);
    job->setStarMessage(starred);
    if (!job->start()) {
        qCDebug(RUQOLA_RESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::downloadFile(const QUrl &url, const QString &mimeType, bool storeInCache, const QUrl &localFileUrl)
{
    DownloadFileJob *job = new DownloadFileJob(this);
    //Rename signal
    connect(job, &DownloadFileJob::downloadFileDone, this, &RestApiRequest::getDataDone);
    job->setUrl(url);
    job->setMimeType(mimeType);
    job->setLocalFileUrl(localFileUrl);
    job->setStoreInCache(storeInCache);
    initializeRestApiJob(job);
    if (!job->start()) {
        qCDebug(RUQOLA_RESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::serverInfo()
{
    ServerInfoJob *job = new ServerInfoJob(this);
    initializeRestApiJob(job);
    connect(job, &ServerInfoJob::serverInfoDone, this, &RestApiRequest::getServerInfoDone);
    if (!job->start()) {
        qCDebug(RUQOLA_RESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::uploadFile(const QString &roomId, const QString &description, const QString &text, const QUrl &filename)
{
    UploadFileJob *job = new UploadFileJob(this);
    initializeRestApiJob(job);
    job->setDescription(description);
    job->setMessageText(text);
    job->setFilenameUrl(filename);
    job->setRoomId(roomId);
    if (!job->start()) {
        qCDebug(RUQOLA_RESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::changeChannelTopic(const QString &roomId, const QString &topic)
{
    ChangeChannelTopicJob *job = new ChangeChannelTopicJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setTopic(topic);
    if (!job->start()) {
        qCDebug(RUQOLA_RESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::changeGroupsTopic(const QString &roomId, const QString &topic)
{
    ChangeGroupsTopicJob *job = new ChangeGroupsTopicJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setTopic(topic);
    if (!job->start()) {
        qCDebug(RUQOLA_RESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::changeChannelAnnouncement(const QString &roomId, const QString &announcement)
{
    ChangeChannelAnnouncementJob *job = new ChangeChannelAnnouncementJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setAnnouncement(announcement);
    if (!job->start()) {
        qCDebug(RUQOLA_RESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::changeGroupsAnnouncement(const QString &roomId, const QString &announcement)
{
    ChangeGroupsAnnouncementJob *job = new ChangeGroupsAnnouncementJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setAnnouncement(announcement);
    if (!job->start()) {
        qCDebug(RUQOLA_RESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::changeChannelDescription(const QString &roomId, const QString &description)
{
    ChangeChannelDescriptionJob *job = new ChangeChannelDescriptionJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setDescription(description);
    if (!job->start()) {
        qCDebug(RUQOLA_RESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::changeGroupsDescription(const QString &roomId, const QString &description)
{
    ChangeGroupsDescriptionJob *job = new ChangeGroupsDescriptionJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setDescription(description);
    if (!job->start()) {
        qCDebug(RUQOLA_RESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::postMessage(const QString &roomId, const QString &text)
{
    PostMessageJob *job = new PostMessageJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setText(text);
    if (!job->start()) {
        qCDebug(RUQOLA_RESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::deleteMessage(const QString &roomId, const QString &messageId)
{
    DeleteMessageJob *job = new DeleteMessageJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setMessageId(messageId);
    if (!job->start()) {
        qCDebug(RUQOLA_RESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::createChannels(const QString &channelName, bool readOnly, const QStringList &members)
{
    CreateChannelJob *job = new CreateChannelJob(this);
    initializeRestApiJob(job);
    job->setChannelName(channelName);
    job->setReadOnly(readOnly);
    job->setMembers(members);
    if (!job->start()) {
        qCDebug(RUQOLA_RESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::createGroups(const QString &channelName, bool readOnly, const QStringList &members)
{
    CreateGroupsJob *job = new CreateGroupsJob(this);
    initializeRestApiJob(job);
    job->setChannelName(channelName);
    job->setReadOnly(readOnly);
    job->setMembers(members);
    if (!job->start()) {
        qCDebug(RUQOLA_RESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::leaveChannel(const QString &roomId)
{
    LeaveChannelJob *job = new LeaveChannelJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    if (!job->start()) {
        qCDebug(RUQOLA_RESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::leaveGroups(const QString &roomId)
{
    LeaveGroupsJob *job = new LeaveGroupsJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    if (!job->start()) {
        qCDebug(RUQOLA_RESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::archiveChannel(const QString &roomId)
{
    ArchiveChannelJob *job = new ArchiveChannelJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    if (!job->start()) {
        qCDebug(RUQOLA_RESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::archiveGroups(const QString &roomId)
{
    ArchiveGroupsJob *job = new ArchiveGroupsJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    if (!job->start()) {
        qCDebug(RUQOLA_RESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::updateMessage(const QString &roomId, const QString &messageId, const QString &text)
{
    UpdateMessageJob *job = new UpdateMessageJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setMessageId(messageId);
    job->setUpdatedText(text);
    if (!job->start()) {
        qCDebug(RUQOLA_RESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::reactOnMessage(const QString &messageId, const QString &emoji)
{
    ReactOnMessageJob *job = new ReactOnMessageJob(this);
    initializeRestApiJob(job);
    job->setMessageId(messageId);
    job->setEmoji(emoji);
    if (!job->start()) {
        qCDebug(RUQOLA_RESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::closeChannel(const QString &roomId, const QString &type)
{
    ChannelCloseJob *job = new ChannelCloseJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    if (type == QLatin1String("d")) {
        job->setChannelType(ChannelCloseJob::Direct);
    } else if (type == QLatin1String("p")) {
        job->setChannelType(ChannelCloseJob::Groups);
    } else if (type == QLatin1String("c")) {
        job->setChannelType(ChannelCloseJob::Channel);
    }
    if (!job->start()) {
        qCDebug(RUQOLA_RESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::historyChannel(const QString &roomId, const QString &type)
{
    ChannelHistoryJob *job = new ChannelHistoryJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    if (type == QLatin1String("d")) {
        job->setChannelType(ChannelHistoryJob::Direct);
    } else if (type == QLatin1String("p")) {
        job->setChannelType(ChannelHistoryJob::Groups);
    } else if (type == QLatin1String("c")) {
        job->setChannelType(ChannelHistoryJob::Channel);
    }
    if (!job->start()) {
        qCDebug(RUQOLA_RESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::createDirectMessage(const QString &userName)
{
    CreateDmJob *job = new CreateDmJob(this);
    initializeRestApiJob(job);
    job->setUserName(userName);
    if (!job->start()) {
        qCDebug(RUQOLA_RESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::filesInRoom(const QString &roomId, const QString &type)
{
    ChannelFilesJob *job = new ChannelFilesJob(this);
    connect(job, &ChannelFilesJob::channelFilesDone, this, &RestApiRequest::channelFilesDone);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    if (type == QLatin1String("d")) {
        job->setChannelType(ChannelFilesJob::Direct);
    } else if (type == QLatin1String("p")) {
        job->setChannelType(ChannelFilesJob::Groups);
    } else if (type == QLatin1String("c")) {
        job->setChannelType(ChannelFilesJob::Channel);
    }
    if (!job->start()) {
        qCDebug(RUQOLA_RESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::addUserInChannel(const QString &roomId, const QString &userId)
{
    ChannelInviteJob *job = new ChannelInviteJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setInviteUserId(userId);
    if (!job->start()) {
        qCDebug(RUQOLA_RESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::addUserInGroup(const QString &roomId, const QString &userId)
{
    GroupsInviteJob *job = new GroupsInviteJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setUserId(userId);
    if (!job->start()) {
        qCDebug(RUQOLA_RESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::listEmojiCustom()
{
    LoadEmojiCustomJob *job = new LoadEmojiCustomJob(this);
    initializeRestApiJob(job);
    connect(job, &LoadEmojiCustomJob::loadEmojiCustomDone, this, &RestApiRequest::loadEmojiCustomDone);
    if (!job->start()) {
        qCDebug(RUQOLA_RESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::searchRoomUser(const QString &pattern)
{
    SpotlightJob *job = new SpotlightJob(this);
    job->setSearchPattern(pattern);
    initializeRestApiJob(job);
    connect(job, &SpotlightJob::spotlightDone, this, &RestApiRequest::spotlightDone);
    if (!job->start()) {
        qCDebug(RUQOLA_RESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::searchMessages(const QString &roomId, const QString &pattern)
{
    SearchMessageJob *job = new SearchMessageJob(this);
    job->setRoomId(roomId);
    job->setSearchText(pattern);
    initializeRestApiJob(job);
    connect(job, &SearchMessageJob::searchMessageDone, this, &RestApiRequest::searchMessageDone);
    if (!job->start()) {
        qCDebug(RUQOLA_RESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::markAsRead(const QString &roomId)
{
    MarkRoomAsReadJob *job = new MarkRoomAsReadJob(this);
    job->setRoomId(roomId);
    initializeRestApiJob(job);
    if (!job->start()) {
        qCDebug(RUQOLA_RESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::markRoomAsUnRead(const QString &roomId)
{
    MarkRoomAsUnReadJob *job = new MarkRoomAsUnReadJob(this);
    job->setObjectId(roomId);
    job->setUnReadObject(MarkRoomAsUnReadJob::Room);
    initializeRestApiJob(job);
    if (!job->start()) {
        qCDebug(RUQOLA_RESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::markMessageAsUnReadFrom(const QString &messageId)
{
    MarkRoomAsUnReadJob *job = new MarkRoomAsUnReadJob(this);
    job->setObjectId(messageId);
    job->setUnReadObject(MarkRoomAsUnReadJob::FromMessage);
    initializeRestApiJob(job);
    if (!job->start()) {
        qCDebug(RUQOLA_RESTAPI_LOG) << "Impossible to start job";
    }
}


void RestApiRequest::getRooms()
{
    GetRoomsJob *job = new GetRoomsJob(this);
    initializeRestApiJob(job);
    connect(job, &GetRoomsJob::getRoomsDone, this, &RestApiRequest::getRoomsDone);
    if (!job->start()) {
        qCDebug(RUQOLA_RESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::markAsFavorite(const QString &roomId, bool favorite)
{
    RoomFavoriteJob *job = new RoomFavoriteJob(this);
    initializeRestApiJob(job);
    job->setFavorite(favorite);
    job->setRoomId(roomId);
    if (!job->start()) {
        qCDebug(RUQOLA_RESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::disableNotifications(const QString &roomId, bool value)
{
    SaveNotificationJob *job = new SaveNotificationJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setDisableNotifications(value);
    if (!job->start()) {
        qCDebug(RUQOLA_RESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::hideUnreadStatus(const QString &roomId, bool value)
{
    SaveNotificationJob *job = new SaveNotificationJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setHideUnreadStatus(value);
    if (!job->start()) {
        qCDebug(RUQOLA_RESTAPI_LOG) << "Impossible to start job";
    }
}


void RestApiRequest::audioNotifications(const QString &roomId, const QString &value)
{
    SaveNotificationJob *job = new SaveNotificationJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setAudioNotifications(value);
    if (!job->start()) {
        qCDebug(RUQOLA_RESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::desktopNotifications(const QString &roomId, const QString &value)
{
//    SaveNotificationJob *job = new SaveNotificationJob(this);
//    initializeRestApiJob(job);
//    job->setRoomId(roomId);
//    job->setDesktopNotificationDuration(value);
//    if (!job->start()) {
//    qCDebug(RUQOLA_RESTAPI_LOG) << "Impossible to start job";
//    }
}

void RestApiRequest::emailNotifications(const QString &roomId, const QString &value)
{
    SaveNotificationJob *job = new SaveNotificationJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setEmailNotifications(value);
    if (!job->start()) {
        qCDebug(RUQOLA_RESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::mobilePushNotifications(const QString &roomId, const QString &value)
{
    SaveNotificationJob *job = new SaveNotificationJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setMobilePushNotifications(value);
    if (!job->start()) {
        qCDebug(RUQOLA_RESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::unreadAlert(const QString &roomId, const QString &value)
{
    SaveNotificationJob *job = new SaveNotificationJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setUnreadAlert(value);
    if (!job->start()) {
        qCDebug(RUQOLA_RESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::setAvatar(const QString &avatarUrl)
{
    SetAvatarJob *job = new SetAvatarJob(this);
    initializeRestApiJob(job);
    job->setAvatarUrl(avatarUrl);
    if (!job->start()) {
        qCDebug(RUQOLA_RESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::forgotPassword(const QString &email)
{
    ForgotPasswordJob *job = new ForgotPasswordJob(this);
    initializeRestApiJob(job);
    job->setEmail(email);
    if (!job->start()) {
        qCDebug(RUQOLA_RESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::userInfo(const QString &identifier, bool userName)
{
    UsersInfoJob *job = new UsersInfoJob(this);
    initializeRestApiJob(job);
    job->setIdentifier(identifier);
    job->setUseUserName(userName);
    connect(job, &UsersInfoJob::usersInfoDone, this, &RestApiRequest::usersInfoDone);
    if (!job->start()) {
        qCDebug(RUQOLA_RESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::ignoreUser(const QString &roomId, const QString &userId, bool ignore)
{
    IgnoreUserJob *job = new IgnoreUserJob(this);
    initializeRestApiJob(job);
    job->setIgnoreUserId(userId);
    job->setRoomId(roomId);
    job->setIgnore(ignore);
    //connect(job, &UsersInfoJob::usersInfoDone, this, &RestApiRequest::usersInfoDone);
    if (!job->start()) {
        qCDebug(RUQOLA_RESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::userPresence(const QString &userId)
{
    GetPresenceJob *job = new GetPresenceJob(this);
    initializeRestApiJob(job);
    job->setPresenceUserId(userId);
    //connect(job, &GetPresenceJob::getPresenceDone, this, &RestApiRequest::getPresenceDone);
    if (!job->start()) {
        qCDebug(RUQOLA_RESTAPI_LOG) << "Impossible to start job";
    }
}
