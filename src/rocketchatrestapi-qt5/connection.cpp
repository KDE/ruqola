/*
   SPDX-FileCopyrightText: 2017-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "connection.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include "channellistjob.h"
#include "downloadfilejob.h"
#include "misc/directoryjob.h"
#include "serverinfojob.h"
#include "settings/privateinfojob.h"
#include "spotlightjob.h"

#include "users/deleteownaccountjob.h"
#include "users/forgotpasswordjob.h"
#include "users/getavatarjob.h"
#include "users/getpresencejob.h"
#include "users/getusernamesuggestionjob.h"
#include "users/removeothertokensjob.h"
#include "users/resetavatarjob.h"
#include "users/userinfojob.h"
#include "users/userspresencejob.h"

#include "misc/owninfojob.h"

#include "emoji/loademojicustomjob.h"

#include "authentication/loginjob.h"
#include "authentication/logoutjob.h"

#include "chat/deletemessagejob.h"
#include "chat/followmessagejob.h"
#include "chat/getmentionedmessagesjob.h"
#include "chat/getmessagejob.h"
#include "chat/getpinnedmessagesjob.h"
#include "chat/getsnippetedmessagesjob.h"
#include "chat/getstarredmessagesjob.h"
#include "chat/getthreadmessagesjob.h"
#include "chat/getthreadsjob.h"
#include "chat/ignoreuserjob.h"
#include "chat/pinmessagejob.h"
#include "chat/postmessagejob.h"
#include "chat/reactonmessagejob.h"
#include "chat/reportmessagejob.h"
#include "chat/searchmessagejob.h"
#include "chat/sendmessagejob.h"
#include "chat/starmessagejob.h"
#include "chat/syncthreadmessagesjob.h"
#include "chat/unfollowmessagejob.h"
#include "chat/updatemessagejob.h"

#include "channels/archivechanneljob.h"
#include "channels/changechannelannouncementjob.h"
#include "channels/changechanneldescriptionjob.h"
#include "channels/changechannelnamejob.h"
#include "channels/changechannelreadonlyjob.h"
#include "channels/changechanneltopicjob.h"
#include "channels/channeladdleaderjob.h"
#include "channels/channeladdmoderatorjob.h"
#include "channels/channeladdownerjob.h"
#include "channels/channelclosejob.h"
#include "channels/channeldeletejob.h"
#include "channels/channelfilesjob.h"
#include "channels/channelgetallusermentionsjob.h"
#include "channels/channelgetcountersjob.h"
#include "channels/channelhistoryjob.h"
#include "channels/channelinfojob.h"
#include "channels/channelinvitejob.h"
#include "channels/channelkickjob.h"
#include "channels/channelmembersjob.h"
#include "channels/channelremoveleaderjob.h"
#include "channels/channelremovemoderatorjob.h"
#include "channels/channelremoveownerjob.h"
#include "channels/createchanneljob.h"
#include "channels/getchannelrolesjob.h"
#include "channels/leavechanneljob.h"
#include "channels/setchanneltypejob.h"
#include "channels/setjoincodechanneljob.h"

#include "groups/archivegroupsjob.h"
#include "groups/changegroupsannouncementjob.h"
#include "groups/changegroupsdescriptionjob.h"
#include "groups/changegroupsencryptedjob.h"
#include "groups/changegroupsnamejob.h"
#include "groups/changegroupsreadonlyjob.h"
#include "groups/changegroupstopicjob.h"
#include "groups/creategroupsjob.h"
#include "groups/getgrouprolesjob.h"
#include "groups/groupaddleaderjob.h"
#include "groups/groupaddmoderatorjob.h"
#include "groups/groupaddownerjob.h"
#include "groups/groupremoveleaderjob.h"
#include "groups/groupremovemoderatorjob.h"
#include "groups/groupremoveownerjob.h"
#include "groups/groupsdeletejob.h"
#include "groups/groupsinfojob.h"
#include "groups/groupsinvitejob.h"
#include "groups/groupskickjob.h"
#include "groups/leavegroupsjob.h"
#include "groups/setgrouptypejob.h"

#include "rooms/getdiscussionsjob.h"
#include "rooms/getroomsjob.h"
#include "rooms/roomfavoritejob.h"
#include "rooms/roomstartdiscussionjob.h"
#include "rooms/savenotificationjob.h"

#include "directmessage/createdmjob.h"
#include "directmessage/opendmjob.h"

#include "subscriptions/markroomasreadjob.h"
#include "subscriptions/markroomasunreadjob.h"

#include "permissions/permissionslistalljob.h"

#include "commands/getcommandsjob.h"
#include "commands/listcommandsjob.h"

#include "e2e/fetchmykeysjob.h"

#include "autotranslate/getsupportedlanguagesjob.h"
#include "autotranslate/translatesavesettingsjob.h"

#include "custom/customuserstatusdeletejob.h"
#include "custom/customuserstatuslistjob.h"

#include "2fa/user2fadisableemailjob.h"
#include "2fa/user2faenableemailjob.h"
#include "2fa/user2fasendemailcodejob.h"

#include "misc/statisticsjob.h"

#include <QNetworkAccessManager>
#include <QNetworkCookie>
#include <QNetworkCookieJar>
#include <QNetworkReply>
#include <QTimer>

using namespace std::chrono_literals;
using namespace RocketChatRestApi;

Connection::Connection(QObject *parent)
    : QObject(parent)
{
    mRestApiMethod = new RestApiMethod;
    mCookieJar = new QNetworkCookieJar(this);
    mNetworkAccessManager = new QNetworkAccessManager(this);
    mNetworkAccessManager->setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);
    mNetworkAccessManager->setCookieJar(mCookieJar);
    connect(mNetworkAccessManager, &QNetworkAccessManager::finished, this, &Connection::slotResult);
    connect(mNetworkAccessManager, &QNetworkAccessManager::sslErrors, this, &Connection::slotSslErrors);
}

Connection::~Connection()
{
    delete mRestApiMethod;
}

void Connection::setRestApiLogger(RocketChatRestApi::AbstractLogger *logger)
{
    mRuqolaLogger = logger;
}

void Connection::initializeCookies()
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
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "We can not initialize cookies as server url is empty.";
    }
}

void Connection::setAuthToken(const QString &authToken)
{
    const bool isChanged = (mAuthToken != authToken);
    mAuthToken = authToken;
    if (isChanged) {
        if (!mAuthToken.isEmpty()) {
            initializeCookies();
        }
    }
}

void Connection::setUserId(const QString &userId)
{
    const bool isChanged = (mUserId != userId);
    mUserId = userId;
    if (isChanged) {
        if (!mUserId.isEmpty()) {
            initializeCookies();
        }
    }
}

void Connection::slotResult(QNetworkReply *reply)
{
    const auto error = reply->error();
    if (error != QNetworkReply::NoError) {
        const auto jobClassName = reply->property("jobClassName").value<QByteArray>();
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << jobClassName << "error reply: " << reply->errorString();

        if (error == QNetworkReply::NetworkSessionFailedError && !mSessionFailed) {
            mSessionFailed = true;
            QTimer::singleShot(1ms, this, [this] {
                Q_EMIT networkSessionFailedError();
            });
        }
    }
}

void Connection::slotSslErrors(QNetworkReply *reply, const QList<QSslError> &error)
{
    qCDebug(ROCKETCHATQTRESTAPI_LOG) << " void RestApiRequest::slotSslErrors(QNetworkReply *reply, const QList<QSslError> &error)" << error.count();
    reply->ignoreSslErrors(error);
}

void Connection::setPassword(const QString &password)
{
    mPassword = password;
}

void Connection::setUserName(const QString &userName)
{
    mUserName = userName;
}

QString Connection::serverUrl() const
{
    return mRestApiMethod->serverUrl();
}

void Connection::setServerUrl(const QString &serverUrl)
{
    mRestApiMethod->setServerUrl(serverUrl);
}

QString Connection::authToken() const
{
    return mAuthToken;
}

QString Connection::userId() const
{
    return mUserId;
}

void Connection::login()
{
    auto job = new LoginJob(this);
    connect(job, &LoginJob::loginDone, this, &Connection::slotLogin);
    initializeRestApiJob(job);
    job->setPassword(mPassword);
    job->setUserName(mUserName);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void Connection::slotLogin(const QString &authToken, const QString &userId)
{
    mAuthToken = authToken;
    mUserId = userId;
}

void Connection::slotLogout()
{
    mAuthToken.clear();
    mUserId.clear();
}

void Connection::initializeRestApiJob(RocketChatRestApi::RestApiAbstractJob *job)
{
    connect(job, &RocketChatRestApi::RestApiAbstractJob::failed, this, &Connection::failed);

    job->setNetworkAccessManager(mNetworkAccessManager);
    job->setRestApiLogger(mRuqolaLogger);
    job->setRestApiMethod(mRestApiMethod);
    if (job->requireHttpAuthentication()) {
        job->setAuthToken(mAuthToken);
        job->setUserId(mUserId);
    }
}

void Connection::logout()
{
    auto job = new LogoutJob(this);
    connect(job, &LogoutJob::logoutDone, this, &Connection::slotLogout);
    initializeRestApiJob(job);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start LogoutJob job";
    }
}

void Connection::channelList()
{
    auto job = new ChannelListJob(this);
    connect(job, &ChannelListJob::channelListDone, this, &Connection::channelListDone);
    initializeRestApiJob(job);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start channelList job";
    }
}

void Connection::getAvatar(const UserBaseJob::UserInfo &info)
{
    auto job = new GetAvatarJob(this);
    connect(job, &GetAvatarJob::avatar, this, &Connection::avatar);
    connect(job, &GetAvatarJob::redownloadAvatar, this, &Connection::redownloadAvatar);
    initializeRestApiJob(job);
    job->setUserInfo(info);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start getAvatar job";
    }
}

void Connection::getPrivateSettings()
{
    auto job = new PrivateInfoJob(this);
    connect(job, &PrivateInfoJob::privateInfoDone, this, &Connection::privateInfoDone);
    initializeRestApiJob(job);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start getPrivateSettings job";
    }
}

void Connection::getOwnInfo()
{
    auto job = new OwnInfoJob(this);
    connect(job, &OwnInfoJob::ownInfoDone, this, &Connection::getOwnInfoDone);
    initializeRestApiJob(job);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start getOwnInfo job";
    }
}

void Connection::starMessage(const QString &messageId, bool starred)
{
    auto job = new StarMessageJob(this);
    initializeRestApiJob(job);
    job->setMessageId(messageId);
    job->setStarMessage(starred);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start starMessage job";
    }
}

DownloadFileJob *Connection::downloadFile(const QUrl &url, const QUrl &localFileUrl, const QByteArray &mimeType, bool requiredAuthentication)
{
    auto job = new DownloadFileJob(this);
    connect(job, &DownloadFileJob::downloadFileDone, this, &Connection::downloadFileDone);
    job->setUrl(url);
    job->setMimeType(mimeType);
    job->setLocalFileUrl(localFileUrl);
    job->setRequiredAuthentication(requiredAuthentication);
    initializeRestApiJob(job);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start DownloadFileJob job";
    }
    return job;
}

void Connection::serverInfo(bool useDeprecatedVersion)
{
    auto job = new ServerInfoJob(this);
    job->setUseDeprecatedVersion(useDeprecatedVersion);
    initializeRestApiJob(job);
    connect(job, &ServerInfoJob::serverInfoDone, this, &Connection::serverInfoDone);
    connect(job, &ServerInfoJob::serverInfoFailed, this, &Connection::serverInfoFailed);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start ServerInfoJob job";
    }
}

void Connection::changeChannelTopic(const QString &roomId, const QString &topic)
{
    auto job = new ChangeChannelTopicJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setTopic(topic);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start ChangeChannelTopicJob job";
    }
}

void Connection::changeGroupsTopic(const QString &roomId, const QString &topic)
{
    auto job = new ChangeGroupsTopicJob(this);
    initializeRestApiJob(job);
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job->setChannelGroupInfo(info);
    job->setTopic(topic);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start ChangeGroupsTopicJob job";
    }
}

void Connection::changeChannelReadOnly(const QString &roomId, bool b)
{
    auto job = new ChangeChannelReadonlyJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setReadOnly(b);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start ChangeChannelReadonlyJob job";
    }
}

void Connection::changeGroupsReadOnly(const QString &roomId, bool b)
{
    auto job = new ChangeGroupsReadonlyJob(this);
    initializeRestApiJob(job);
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job->setChannelGroupInfo(info);
    job->setReadOnly(b);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start changeGroupsReadOnly job";
    }
}

void Connection::changeGroupsEncrypted(const QString &roomId, bool b)
{
    auto job = new ChangeGroupsEncryptedJob(this);
    initializeRestApiJob(job);
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job->setChannelGroupInfo(info);
    job->setEncrypted(b);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start ChangeGroupsEncryptedJob job";
    }
}

void Connection::changeChannelAnnouncement(const QString &roomId, const QString &announcement)
{
    auto job = new ChangeChannelAnnouncementJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setAnnouncement(announcement);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start changeChannelAnnouncement job";
    }
}

void Connection::changeGroupsAnnouncement(const QString &roomId, const QString &announcement)
{
    auto job = new ChangeGroupsAnnouncementJob(this);
    initializeRestApiJob(job);
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job->setChannelGroupInfo(info);
    job->setAnnouncement(announcement);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start ChangeGroupsAnnouncementJob job";
    }
}

void Connection::changeChannelDescription(const QString &roomId, const QString &description)
{
    auto job = new ChangeChannelDescriptionJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setDescription(description);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start changeChannelDescription job";
    }
}

void Connection::changeGroupsDescription(const QString &roomId, const QString &description)
{
    auto job = new ChangeGroupsDescriptionJob(this);
    initializeRestApiJob(job);
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job->setChannelGroupInfo(info);
    job->setDescription(description);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start changeGroupsDescription job";
    }
}

void Connection::postMessage(const QString &roomId, const QString &text)
{
    auto job = new PostMessageJob(this);
    connect(job, &PostMessageJob::postMessageDone, this, &Connection::postMessageDone);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setText(text);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start PostMessageJob job";
    }
}

void Connection::deleteMessage(const QString &roomId, const QString &messageId)
{
    auto job = new DeleteMessageJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setMessageId(messageId);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start deleteMessage job";
    }
}

void Connection::slotAddJoinCodeToChannel(const QString &channelId, const QString &password)
{
    setJoinCodeChannel(channelId, password);
}

void Connection::createChannels(const RocketChatRestApi::CreateChannelTeamInfo &info)
{
    auto job = new CreateChannelJob(this);
    connect(job, &CreateChannelJob::addJoinCodeToChannel, this, &Connection::slotAddJoinCodeToChannel);
    initializeRestApiJob(job);
    job->setCreateChannelInfo(info);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start CreateChannelJob job";
    }
}

void Connection::createGroups(const RocketChatRestApi::CreateChannelTeamInfo &info)
{
    auto job = new CreateGroupsJob(this);
    initializeRestApiJob(job);
    job->setCreateGroupsInfo(info);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start CreateGroupsJob job";
    }
}

void Connection::leaveChannel(const QString &roomId)
{
    auto job = new LeaveChannelJob(this);
    initializeRestApiJob(job);
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job->setChannelGroupInfo(info);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start leaveChannel job";
    }
}

void Connection::leaveGroups(const QString &roomId)
{
    auto job = new LeaveGroupsJob(this);
    initializeRestApiJob(job);
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job->setChannelGroupInfo(info);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start leaveGroups job";
    }
}

void Connection::archiveChannel(const QString &roomId, bool archive)
{
    auto job = new ArchiveChannelJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setArchive(archive);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start ArchiveChannelJob job";
    }
}

void Connection::archiveGroups(const QString &roomId, bool archive)
{
    auto job = new ArchiveGroupsJob(this);
    initializeRestApiJob(job);
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job->setChannelGroupInfo(info);
    job->setArchive(archive);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start archiveGroups job";
    }
}

void Connection::updateMessage(const QString &roomId, const QString &messageId, const QString &text)
{
    auto job = new UpdateMessageJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setMessageId(messageId);
    job->setUpdatedText(text);
    connect(job, &UpdateMessageJob::updateMessageFailed, this, &Connection::updateMessageFailed);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start updateMessage job";
    }
}

void Connection::reactOnMessage(const QString &messageId, const QString &emoji, bool shouldReact)
{
    auto job = new ReactOnMessageJob(this);
    initializeRestApiJob(job);
    job->setMessageId(messageId);
    QString fixedEmoji = emoji;
    if (fixedEmoji.startsWith(QLatin1Char(':'))) {
        fixedEmoji.remove(0, 1);
    }
    if (fixedEmoji.endsWith(QLatin1Char(':'))) {
        fixedEmoji.chop(1);
    }

    job->setEmoji(fixedEmoji);
    job->setShouldReact(shouldReact);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start reactOnMessage job";
    }
}

void Connection::closeChannel(const QString &roomId, const QString &type)
{
    auto job = new ChannelCloseJob(this);
    initializeRestApiJob(job);
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job->setChannelGroupInfo(info);
    if (type == QLatin1Char('d')) {
        job->setChannelType(ChannelCloseJob::Direct);
    } else if (type == QLatin1Char('p')) {
        job->setChannelType(ChannelCloseJob::Groups);
    } else if (type == QLatin1Char('c')) {
        job->setChannelType(ChannelCloseJob::Channel);
    }
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start ChannelCloseJob job";
    }
}

void Connection::historyChannel(const RocketChatRestApi::ChannelHistoryJob::ChannelHistoryInfo &info)
{
    // TODO
    auto job = new ChannelHistoryJob(this);
    initializeRestApiJob(job);
    job->setChannelHistoryInfo(info);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start historyChannel job";
    }
}

void Connection::historyChannel(const QString &roomId, const QString &type)
{
    auto job = new ChannelHistoryJob(this);
    initializeRestApiJob(job);
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job->setChannelGroupInfo(info);
    if (type == QLatin1Char('d')) {
        ChannelHistoryJob::ChannelHistoryInfo historyInfo;
        historyInfo.channelType = ChannelHistoryJob::Direct;
        job->setChannelHistoryInfo(historyInfo);
    } else if (type == QLatin1Char('p')) {
        ChannelHistoryJob::ChannelHistoryInfo historyInfo;
        historyInfo.channelType = ChannelHistoryJob::Groups;
        job->setChannelHistoryInfo(historyInfo);
    } else if (type == QLatin1Char('c')) {
        ChannelHistoryJob::ChannelHistoryInfo historyInfo;
        historyInfo.channelType = ChannelHistoryJob::Channel;
        job->setChannelHistoryInfo(historyInfo);
    }
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start historyChannel job";
    }
}

void Connection::createDirectMessage(const QStringList &userNames)
{
    auto job = new CreateDmJob(this);
    initializeRestApiJob(job);
    job->setUserNames(userNames);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start createDirectMessage job";
    }
}

void Connection::openDirectMessage(const QString &userId)
{
    auto job = new OpenDmJob(this);
    initializeRestApiJob(job);
    job->setDirectUserId(userId);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void Connection::filesInRoom(const QString &roomId, const QString &type, int offset, int count)
{
    auto job = new ChannelFilesJob(this);
    connect(job, &ChannelFilesJob::channelFilesDone, this, &Connection::channelFilesDone);
    initializeRestApiJob(job);
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job->setChannelGroupInfo(info);
    QueryParameters parameters;
    parameters.setCount(count);
    parameters.setOffset(offset);
    QMap<QString, QueryParameters::SortOrder> map;
    map.insert(QStringLiteral("uploadedAt"), QueryParameters::SortOrder::Descendant);
    parameters.setSorting(map);
    job->setQueryParameters(parameters);

    if (type == QLatin1Char('d')) {
        job->setChannelType(ChannelFilesJob::Direct);
    } else if (type == QLatin1Char('p')) {
        job->setChannelType(ChannelFilesJob::Groups);
    } else if (type == QLatin1Char('c')) {
        job->setChannelType(ChannelFilesJob::Channel);
    }
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start filesInRoom job";
    }
}

// FIXME
void Connection::membersInRoom(const QString &roomId, const QString &type, int offset, int count)
{
    auto job = new ChannelMembersJob(this);
    QueryParameters parameters;
    parameters.setCount(count);
    parameters.setOffset(offset);
    job->setQueryParameters(parameters);
    connect(job, &ChannelMembersJob::channelMembersDone, this, &Connection::channelMembersDone);
    initializeRestApiJob(job);
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job->setChannelGroupInfo(info);
    if (type == QLatin1Char('d')) {
        job->setChannelType(ChannelMembersJob::Direct);
    } else if (type == QLatin1Char('p')) {
        job->setChannelType(ChannelMembersJob::Groups);
    } else if (type == QLatin1Char('c')) {
        job->setChannelType(ChannelMembersJob::Channel);
    }
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start membersInRoom job";
    }
}

void Connection::addUserInChannel(const QString &roomId, const QString &userId)
{
    auto job = new ChannelInviteJob(this);
    initializeRestApiJob(job);
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job->setChannelGroupInfo(info);
    job->setInviteUserId(userId);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start addUserInChannel job";
    }
}

void Connection::addUserInGroup(const QString &roomId, const QString &userId)
{
    auto job = new GroupsInviteJob(this);
    initializeRestApiJob(job);
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job->setChannelGroupInfo(info);

    job->setInviteUserId(userId);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start addUserInGroup job";
    }
}

void Connection::listEmojiCustom()
{
    auto job = new LoadEmojiCustomJob(this);
    initializeRestApiJob(job);
    connect(job, &LoadEmojiCustomJob::loadEmojiCustomDone, this, &Connection::loadEmojiCustomDone);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start listEmojiCustom job";
    }
}

void Connection::searchRoomUser(const QString &pattern)
{
    auto job = new SpotlightJob(this);
    job->setSearchPattern(pattern);
    initializeRestApiJob(job);
    connect(job, &SpotlightJob::spotlightDone, this, &Connection::spotlightDone);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start searchRoomUser job";
    }
}

void Connection::searchRooms(const QString &pattern)
{
    auto job = new DirectoryJob(this);
    DirectoryJob::DirectoryInfo info;
    info.searchType = DirectoryJob::SearchType::Rooms;
    info.pattern = pattern;
    job->setDirectoryInfo(info);
    initializeRestApiJob(job);
    connect(job, &DirectoryJob::directoryDone, this, &Connection::directoryDone);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start searchRoomUser job";
    }
}

void Connection::searchMessages(const QString &roomId, const QString &pattern, bool useRegularExpression)
{
    auto job = new SearchMessageJob(this);
    job->setRoomId(roomId);
    job->setSearchText(pattern);
    job->setUseRegularExpression(useRegularExpression);
    initializeRestApiJob(job);
    connect(job, &SearchMessageJob::searchMessageDone, this, &Connection::searchMessageDone);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start searchMessages job";
    }
}

void Connection::markRoomAsRead(const QString &roomId)
{
    auto job = new MarkRoomAsReadJob(this);
    job->setRoomId(roomId);
    initializeRestApiJob(job);
    connect(job, &MarkRoomAsReadJob::markAsReadDone, this, &Connection::markAsReadDone);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start markAsRead job";
    }
}

void Connection::markRoomAsUnRead(const QString &roomId)
{
    auto job = new MarkRoomAsUnReadJob(this);
    job->setObjectId(roomId);
    job->setUnReadObject(MarkRoomAsUnReadJob::Room);
    initializeRestApiJob(job);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start markRoomAsUnRead job";
    }
}

void Connection::markMessageAsUnReadFrom(const QString &messageId)
{
    auto job = new MarkRoomAsUnReadJob(this);
    job->setObjectId(messageId);
    job->setUnReadObject(MarkRoomAsUnReadJob::FromMessage);
    initializeRestApiJob(job);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start markMessageAsUnReadFrom job";
    }
}

void Connection::getRooms()
{
    auto job = new GetRoomsJob(this);
    initializeRestApiJob(job);
    connect(job, &GetRoomsJob::getRoomsDone, this, &Connection::getRoomsDone);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start getRooms job";
    }
}

void Connection::markAsFavorite(const QString &roomId, bool favorite)
{
    auto job = new RoomFavoriteJob(this);
    initializeRestApiJob(job);
    job->setFavorite(favorite);
    job->setRoomId(roomId);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start markAsFavorite job";
    }
}

void Connection::disableNotifications(const QString &roomId, bool value)
{
    auto job = new SaveNotificationJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setDisableNotifications(value);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start disableNotifications job";
    }
}

void Connection::muteGroupMentions(const QString &roomId, bool value)
{
    auto job = new SaveNotificationJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setMuteGroupMentions(value);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start muteGroupMentions job";
    }
}

void Connection::desktopDurationNotifications(const QString &roomId, int value)
{
    auto job = new SaveNotificationJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setDesktopNotificationDuration(value);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start desktopDurationNotifications job";
    }
}

void Connection::hideMentionStatus(const QString &roomId, bool value)
{
    auto job = new SaveNotificationJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setHideMentionStatus(value);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start desktopSoundNotifications job";
    }
}

void Connection::desktopSoundNotifications(const QString &roomId, const QString &value)
{
    auto job = new SaveNotificationJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setAudioNotificationValue(value);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start desktopSoundNotifications job";
    }
}

void Connection::changeGroupName(const QString &roomId, const QString &newName)
{
    auto job = new ChangeGroupsNameJob(this);
    initializeRestApiJob(job);
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job->setChannelGroupInfo(info);
    job->setName(newName);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start changeGroupName job";
    }
}

void Connection::changeChannelName(const QString &roomId, const QString &newName)
{
    auto job = new ChangeChannelNameJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setName(newName);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start changeChannelName job";
    }
}

void Connection::channelInfo(const QString &roomId)
{
    auto job = new ChannelInfoJob(this);
    initializeRestApiJob(job);
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job->setChannelGroupInfo(info);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start channelInfo job";
    }
}

void Connection::groupInfo(const QString &roomId)
{
    auto job = new GroupsInfoJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start groupInfo job";
    }
}

void Connection::hideUnreadStatus(const QString &roomId, bool value)
{
    auto job = new SaveNotificationJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setHideUnreadStatus(value);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start hideUnreadStatus job";
    }
}

void Connection::desktopNotifications(const QString &roomId, const QString &value)
{
    auto job = new SaveNotificationJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setDesktopNotifications(value);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start desktopNotifications job";
    }
}

void Connection::emailNotifications(const QString &roomId, const QString &value)
{
    auto job = new SaveNotificationJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setEmailNotifications(value);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start emailNotifications job";
    }
}

void Connection::mobilePushNotifications(const QString &roomId, const QString &value)
{
    auto job = new SaveNotificationJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setMobilePushNotifications(value);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start mobilePushNotifications job";
    }
}

void Connection::unreadAlert(const QString &roomId, const QString &value)
{
    auto job = new SaveNotificationJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setUnreadAlert(value);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start unreadAlert job";
    }
}

void Connection::setAvatar(const UserInfoJob::UserInfo &info, const SetAvatarJob::SetAvatarInfo &avatarInfo)
{
    auto job = new SetAvatarJob(this);
    initializeRestApiJob(job);

    job->setAvatarInfo(avatarInfo);
    job->setUserInfo(info);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start setAvatar job";
    }
}

void Connection::forgotPassword(const QString &email)
{
    auto job = new ForgotPasswordJob(this);
    initializeRestApiJob(job);
    job->setEmail(email);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start forgotPassword job";
    }
}

void Connection::userInfo(const QString &identifier, bool userName)
{
    auto job = new UserInfoJob(this);
    initializeRestApiJob(job);
    UserInfoJob::UserInfo info;
    info.userIdentifier = identifier;
    info.userInfoType = userName ? UserInfoJob::UserInfoType::UserName : UserInfoJob::UserInfoType::UserId;
    job->setUserInfo(info);
    connect(job, &UserInfoJob::userInfoDone, this, &Connection::userInfoDone);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start userInfo job";
    }
}

void Connection::ignoreUser(const QString &roomId, const QString &userId, bool ignore)
{
    auto job = new IgnoreUserJob(this);
    initializeRestApiJob(job);
    job->setIgnoreUserId(userId);
    job->setRoomId(roomId);
    job->setIgnore(ignore);
    connect(job, &IgnoreUserJob::ignoreUserDone, this, &Connection::ignoreUserDone);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start ignoreUser job";
    }
}

void Connection::userPresence(const QString &userId)
{
    auto job = new GetPresenceJob(this);
    initializeRestApiJob(job);
    UserInfoJob::UserInfo info;
    info.userIdentifier = userId;
    info.userInfoType = UserInfoJob::UserInfoType::UserId;

    job->setUserInfo(info);
    connect(job, &GetPresenceJob::getPresenceDone, this, &Connection::getPresenceDone);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start userPresence job";
    }
}

void Connection::reportMessage(const QString &messageId, const QString &message)
{
    auto job = new ReportMessageJob(this);
    initializeRestApiJob(job);
    job->setMessageId(messageId);
    job->setReportMessage(message);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start reportMessage job";
    }
}

void Connection::setGroupType(const QString &roomId, bool isPrivate)
{
    auto job = new SetGroupTypeJob(this);
    initializeRestApiJob(job);
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job->setChannelGroupInfo(info);

    job->setType(isPrivate ? SetGroupTypeJob::Private : SetGroupTypeJob::Public);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start setGroupType job";
    }
}

void Connection::setChannelType(const QString &roomId, bool isPrivate)
{
    auto job = new SetChannelTypeJob(this);
    initializeRestApiJob(job);
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job->setChannelGroupInfo(info);
    job->setType(isPrivate ? SetChannelTypeJob::Private : SetChannelTypeJob::Public);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start setChannelType job";
    }
}

void Connection::getGroupRoles(const QString &roomId)
{
    auto job = new GetGroupRolesJob(this);
    initializeRestApiJob(job);
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job->setChannelGroupInfo(info);
    connect(job, &GetGroupRolesJob::groupRolesDone, this, &Connection::groupRolesDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start getGroupRoles job";
    }
}

void Connection::getChannelRoles(const QString &roomId)
{
    auto job = new GetChannelRolesJob(this);
    initializeRestApiJob(job);
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job->setChannelGroupInfo(info);
    connect(job, &GetChannelRolesJob::channelRolesDone, this, &Connection::channelRolesDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start GetChannelRolesJob job";
    }
}

void Connection::getUsernameSuggestion()
{
    auto job = new GetUsernameSuggestionJob(this);
    initializeRestApiJob(job);
    connect(job, &GetUsernameSuggestionJob::getUsernameSuggestionDone, this, &Connection::getUsernameSuggestionDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start getUsernameSuggestion job";
    }
}

void Connection::listAllPermissions()
{
    auto job = new PermissionsListAllJob(this);
    initializeRestApiJob(job);
    connect(job, &PermissionsListAllJob::permissionListAllDone, this, &Connection::permissionListAllDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start ListPermissionsJob job";
    }
}

void Connection::listCommands()
{
    auto job = new ListCommandsJob(this);
    initializeRestApiJob(job);
    connect(job, &ListCommandsJob::listCommandsDone, this, &Connection::listCommandsDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start ListPermissionsJob job";
    }
}

void Connection::getCommand(const QString &commandName)
{
    auto job = new GetCommandsJob(this);
    initializeRestApiJob(job);
    job->setCommandName(commandName);
    connect(job, &GetCommandsJob::getCommandsDone, this, &Connection::getCommandsDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start GetCommandsJob job";
    }
}

void Connection::runCommand(const RunCommandJob::RunCommandInfo &runCommandInfo)
{
    auto job = new RunCommandJob(this);
    initializeRestApiJob(job);
    job->setRunCommandInfo(runCommandInfo);
    connect(job, &RunCommandJob::runCommandDone, this, &Connection::runCommandDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start RunCommandJob job";
    }
}

void Connection::fetchMyKeys()
{
    auto job = new FetchMyKeysJob(this);
    initializeRestApiJob(job);
    connect(job, &FetchMyKeysJob::fetchMyKeysDone, this, &Connection::fetchMyKeysDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start fetchmykeys job";
    }
}

void Connection::setJoinCodeChannel(const QString &roomId, const QString &joinCode)
{
    auto job = new SetJoinCodeChannelJob(this);
    initializeRestApiJob(job);
    job->setJoinCode(joinCode);
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job->setChannelGroupInfo(info);

    connect(job, &SetJoinCodeChannelJob::setJoinCodeDone, this, &Connection::setJoinCodeDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start setjoincode";
    }
}

void Connection::channelGetAllUserMentions(const QString &roomId, int offset, int count)
{
    auto job = new ChannelGetAllUserMentionsJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    QueryParameters parameters;
    QMap<QString, QueryParameters::SortOrder> map;
    map.insert(QStringLiteral("ts"), QueryParameters::SortOrder::Descendant);
    parameters.setSorting(map);
    parameters.setCount(count);
    parameters.setOffset(offset);
    job->setQueryParameters(parameters);

    connect(job, &ChannelGetAllUserMentionsJob::channelGetAllUserMentionsDone, this, &Connection::channelGetAllUserMentionsDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start setChannelJoin";
    }
}

void Connection::channelKick(const QString &roomId, const QString &userId)
{
    auto job = new ChannelKickJob(this);
    initializeRestApiJob(job);
    job->setKickUserId(userId);
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job->setChannelGroupInfo(info);
    connect(job, &ChannelKickJob::kickUserDone, this, &Connection::channelKickUserDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start channelKick";
    }
}

void Connection::groupKick(const QString &roomId, const QString &userId)
{
    auto job = new GroupsKickJob(this);
    initializeRestApiJob(job);
    job->setKickUserId(userId);
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job->setChannelGroupInfo(info);

    connect(job, &GroupsKickJob::kickUserDone, this, &Connection::groupKickUserDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start channelKick";
    }
}

void Connection::groupAddModerator(const QString &roomId, const QString &userId)
{
    auto job = new GroupAddModeratorJob(this);
    initializeRestApiJob(job);
    job->setAddModeratorUserId(userId);
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job->setChannelGroupInfo(info);
    connect(job, &GroupAddModeratorJob::addModeratorDone, this, &Connection::addModeratorDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start groupAddModerator";
    }
}

void Connection::groupRemoveModerator(const QString &roomId, const QString &userId)
{
    auto job = new GroupRemoveModeratorJob(this);
    initializeRestApiJob(job);
    job->setRemoveUserId(userId);
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.identifier = roomId;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    job->setChannelGroupInfo(info);
    connect(job, &GroupRemoveModeratorJob::removeModeratorDone, this, &Connection::removeModeratorDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start groupRemoveModerator";
    }
}

void Connection::groupAddLeader(const QString &roomId, const QString &userId)
{
    auto job = new GroupAddLeaderJob(this);
    initializeRestApiJob(job);
    job->setAddLeaderUserId(userId);
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job->setChannelGroupInfo(info);
    connect(job, &GroupAddLeaderJob::addLeaderDone, this, &Connection::addLeaderDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start groupAddLeader";
    }
}

void Connection::groupRemoveLeader(const QString &roomId, const QString &userId)
{
    auto job = new GroupRemoveLeaderJob(this);
    initializeRestApiJob(job);
    job->setRemoveUserId(userId);
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.identifier = roomId;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    job->setChannelGroupInfo(info);
    connect(job, &GroupRemoveLeaderJob::removeLeaderDone, this, &Connection::removeLeaderDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start groupRemoveLeader";
    }
}

void Connection::groupAddOwner(const QString &roomId, const QString &userId)
{
    auto job = new GroupAddOwnerJob(this);
    initializeRestApiJob(job);
    job->setAddownerUserId(userId);
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.identifier = roomId;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    job->setChannelGroupInfo(info);
    connect(job, &GroupAddOwnerJob::addOwnerDone, this, &Connection::addOwnerDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start groupAddOwner";
    }
}

void Connection::groupRemoveOwner(const QString &roomId, const QString &userId)
{
    auto job = new GroupRemoveOwnerJob(this);
    initializeRestApiJob(job);
    job->setRemoveUserId(userId);
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job->setChannelGroupInfo(info);

    connect(job, &GroupRemoveOwnerJob::groupRemoveOwnerDone, this, &Connection::channelRemoveOwnerDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start groupRemoveOwner";
    }
}

void Connection::channelAddModerator(const QString &roomId, const QString &userId)
{
    auto job = new ChannelAddModeratorJob(this);
    initializeRestApiJob(job);
    job->setAddModeratorUserId(userId);
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job->setChannelGroupInfo(info);
    connect(job, &ChannelAddModeratorJob::addModeratorDone, this, &Connection::addModeratorDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start channelAddModerator";
    }
}

void Connection::channelRemoveModerator(const QString &roomId, const QString &userId)
{
    auto job = new ChannelRemoveModeratorJob(this);
    initializeRestApiJob(job);
    job->setRemoveUserId(userId);
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job->setChannelGroupInfo(info);
    connect(job, &ChannelRemoveModeratorJob::removeModeratorDone, this, &Connection::removeModeratorDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start ChannelRemoveModeratorJob";
    }
}

void Connection::channelAddLeader(const QString &roomId, const QString &userId)
{
    auto job = new ChannelAddLeaderJob(this);
    initializeRestApiJob(job);
    job->setAddLeaderUserId(userId);
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job->setChannelGroupInfo(info);
    connect(job, &ChannelAddLeaderJob::addLeaderDone, this, &Connection::addLeaderDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start channelAddLeader";
    }
}

void Connection::channelRemoveLeader(const QString &roomId, const QString &userId)
{
    auto job = new ChannelRemoveLeaderJob(this);
    initializeRestApiJob(job);
    job->setRemoveUserId(userId);
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job->setChannelGroupInfo(info);
    connect(job, &ChannelRemoveLeaderJob::removeLeaderDone, this, &Connection::removeLeaderDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start channelRemoveLeader";
    }
}

void Connection::channelAddOwner(const QString &roomId, const QString &userId)
{
    auto job = new ChannelAddOwnerJob(this);
    initializeRestApiJob(job);
    job->setAddownerUserId(userId);
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job->setChannelGroupInfo(info);
    connect(job, &ChannelAddOwnerJob::addOwnerDone, this, &Connection::addOwnerDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start ChannelAddOwnerJob";
    }
}

void Connection::channelRemoveOwner(const QString &roomId, const QString &userId)
{
    auto job = new ChannelRemoveOwnerJob(this);
    initializeRestApiJob(job);
    job->setRemoveUserId(userId);
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job->setChannelGroupInfo(info);
    connect(job, &ChannelRemoveOwnerJob::channelRemoveOwnerDone, this, &Connection::channelRemoveOwnerDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start ChannelRemoveOwnerJob";
    }
}

void Connection::channelDelete(const QString &roomId)
{
    auto job = new ChannelDeleteJob(this);
    initializeRestApiJob(job);
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job->setChannelGroupInfo(info);
    connect(job, &ChannelDeleteJob::deletechannelDone, this, &Connection::deletechannelDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start ChannelDeleteJob";
    }
}

void Connection::groupDelete(const QString &roomId)
{
    auto job = new GroupsDeleteJob(this);
    initializeRestApiJob(job);
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.identifier = roomId;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    job->setChannelGroupInfo(info);
    connect(job, &GroupsDeleteJob::deleteGroupsDone, this, &Connection::deleteGroupsDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start GroupsDeleteJob";
    }
}

void Connection::pinMessage(const QString &messageId, bool pinned)
{
    auto job = new PinMessageJob(this);
    initializeRestApiJob(job);
    job->setMessageId(messageId);
    job->setPinMessage(pinned);
    connect(job, &PinMessageJob::pinMessageDone, this, &Connection::pinMessageDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start PinMessageJob";
    }
}

void Connection::followMessage(const QString &messageId)
{
    auto job = new FollowMessageJob(this);
    initializeRestApiJob(job);
    job->setMessageId(messageId);
    connect(job, &FollowMessageJob::followMessageDone, this, &Connection::followMessageDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start FollowMessageJob";
    }
}

void Connection::unFollowMessage(const QString &messageId)
{
    auto job = new UnFollowMessageJob(this);
    initializeRestApiJob(job);
    job->setMessageId(messageId);
    connect(job, &UnFollowMessageJob::unFollowMessageDone, this, &Connection::unFollowMessageDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start unFollowMessageDone";
    }
}

void Connection::createDiscussion(const QString &parentRoomId,
                                  const QString &discussionName,
                                  const QString &replyMessage,
                                  const QString &parentMessageId,
                                  const QStringList &users)
{
    auto job = new RoomStartDiscussionJob(this);
    initializeRestApiJob(job);
    job->setParentRoomId(parentRoomId);

    job->setDiscussionName(discussionName);
    job->setParentMessageId(parentMessageId);
    job->setReplyMessage(replyMessage);
    job->setUsers(users);
    connect(job, &RoomStartDiscussionJob::startDiscussionDone, this, &Connection::startDiscussionDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start roomStartDiscussion";
    }
}

void Connection::getDiscussions(const QString &roomId, int offset, int count)
{
    auto job = new GetDiscussionsJob(this);
    initializeRestApiJob(job);
    QueryParameters parameters;
    parameters.setCount(count);
    parameters.setOffset(offset);
    job->setQueryParameters(parameters);
    job->setRoomId(roomId);
    connect(job, &GetDiscussionsJob::getDiscussionsDone, this, &Connection::getDiscussionsDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start getDiscussions";
    }
}

void Connection::getThreadsList(const QString &roomId, bool onlyUnread, int offset, int count)
{
    auto job = new GetThreadsJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    QueryParameters parameters;
    QMap<QString, QueryParameters::SortOrder> map;
    map.insert(QStringLiteral("ts"), QueryParameters::SortOrder::Descendant);
    parameters.setSorting(map);
    parameters.setCount(count);
    parameters.setOffset(offset);
    if (onlyUnread) {
        job->setSearchType(GetThreadsJob::TheadSearchType::Unread);
    }
    job->setQueryParameters(parameters);
    connect(job, &GetThreadsJob::getThreadsDone, this, [this, onlyUnread](const QJsonObject &obj, const QString &roomId) {
        Q_EMIT getThreadsDone(obj, roomId, onlyUnread);
    });
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start getThreadsList";
    }
}

void Connection::getMessage(const QString &messageId, const QString &roomId)
{
    auto job = new GetMessageJob(this);
    initializeRestApiJob(job);
    job->setMessageId(messageId);
    job->setRoomId(roomId);
    connect(job, &GetMessageJob::getMessageDone, this, &Connection::getMessageDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start getMessage";
    }
}

void Connection::getPinnedMessages(const QString &roomId, int offset, int count)
{
    auto job = new GetPinnedMessagesJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    QueryParameters parameters;
    parameters.setCount(count);
    parameters.setOffset(offset);
    QMap<QString, QueryParameters::SortOrder> map;
    map.insert(QStringLiteral("ts"), QueryParameters::SortOrder::Descendant);

    job->setQueryParameters(parameters);
    connect(job, &GetPinnedMessagesJob::getPinnedMessagesDone, this, &Connection::getPinnedMessagesDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start getPinnedMessagesList";
    }
}

void Connection::getMentionedMessages(const QString &roomId, int offset, int count)
{
    auto job = new GetMentionedMessagesJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    QueryParameters parameters;
    QMap<QString, QueryParameters::SortOrder> map;
    map.insert(QStringLiteral("ts"), QueryParameters::SortOrder::Descendant);
    parameters.setSorting(map);
    parameters.setCount(count);
    parameters.setOffset(offset);
    job->setQueryParameters(parameters);

    connect(job, &GetMentionedMessagesJob::getMentionedMessagesDone, this, &Connection::getMentionedMessagesDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start getMentionedMessages";
    }
}

void Connection::getStarredMessages(const QString &roomId, int offset, int count)
{
    auto job = new GetStarredMessagesJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    QueryParameters parameters;
    QMap<QString, QueryParameters::SortOrder> map;
    map.insert(QStringLiteral("ts"), QueryParameters::SortOrder::Descendant);
    parameters.setSorting(map);
    parameters.setCount(count);
    parameters.setOffset(offset);
    job->setQueryParameters(parameters);

    connect(job, &GetStarredMessagesJob::getStarredMessagesDone, this, &Connection::getStarredMessagesDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start getStarredMessagesList";
    }
}

void Connection::getSnippetedMessages(const QString &roomId, int offset, int count)
{
    auto job = new GetSnippetedMessagesJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    QueryParameters parameters;
    parameters.setCount(count);
    parameters.setOffset(offset);

    QMap<QString, QueryParameters::SortOrder> map;
    map.insert(QStringLiteral("ts"), QueryParameters::SortOrder::Descendant);
    job->setQueryParameters(parameters);
    connect(job, &GetSnippetedMessagesJob::getSnippetedMessagesDone, this, &Connection::getSnippetedMessagesDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start getSnippetedMessagesList";
    }
}

void Connection::getThreadMessages(const QString &threadMessageId)
{
    auto job = new GetThreadMessagesJob(this);
    initializeRestApiJob(job);
    job->setThreadMessageId(threadMessageId);
    connect(job, &GetThreadMessagesJob::getThreadMessagesDone, this, &Connection::getThreadMessagesDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start getThreadMessages";
    }
}

void Connection::syncThreadMessages(const QString &threadMessageId, const QString &timestamp)
{
    auto job = new SyncThreadMessagesJob(this);
    initializeRestApiJob(job);
    job->setThreadMessageId(threadMessageId);
    job->setTimeStamp(timestamp);
    connect(job, &SyncThreadMessagesJob::syncThreadMessagesDone, this, &Connection::syncThreadMessagesDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start syncThreadMessages";
    }
}

void Connection::sendMessage(const QString &roomId, const QString &text, const QString &messageId, const QString &threadMessageId)
{
    auto job = new SendMessageJob(this);
    initializeRestApiJob(job);
    SendMessageJob::SendMessageArguments args;
    args.roomId = roomId;
    args.message = text;
    args.messageId = messageId;
    args.threadMessageId = threadMessageId;
    job->setSendMessageArguments(args);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void Connection::getSupportedLanguagesMessages(bool needTagetLanguage)
{
    auto job = new GetSupportedLanguagesJob(this);
    job->setNeedTargetLanguage(needTagetLanguage);
    initializeRestApiJob(job);

    connect(job, &GetSupportedLanguagesJob::getSupportedLanguagesDone, this, &Connection::getSupportedLanguagesDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start getSupportedLanguagesMessages";
    }
}

void Connection::autoTranslateSaveLanguageSettings(const QString &roomId, const QString &language)
{
    auto job = new TranslateSaveSettingsJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setType(TranslateSaveSettingsJob::LanguageSetting);
    job->setLanguage(language);
    connect(job, &TranslateSaveSettingsJob::translateSavesettingsDone, this, &Connection::translateSavesettingsDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start autoTranslateSaveLanguageSettings";
    }
}

void Connection::autoTranslateSaveAutoTranslateSettings(const QString &roomId, bool autoTranslate)
{
    auto job = new TranslateSaveSettingsJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setType(TranslateSaveSettingsJob::AutoTranslateSetting);
    job->setAutoTranslate(autoTranslate);
    connect(job, &TranslateSaveSettingsJob::translateSavesettingsDone, this, &Connection::translateSavesettingsDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start autoTranslateSaveAutoTranslateSettings";
    }
}

void Connection::setUserStatus(const QString &userId, SetStatusJob::StatusType status, const QString &message)
{
    auto job = new SetStatusJob(this);
    initializeRestApiJob(job);
    job->setStatusUserId(userId);
    job->setStatus(status);
    job->setStatusMessage(message);
    connect(job, &SetStatusJob::setStatusDone, this, &Connection::setStatusDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start SetStatusJob";
    }
}

void Connection::usersPresence()
{
    auto job = new UsersPresenceJob(this);
    initializeRestApiJob(job);
    connect(job, &UsersPresenceJob::usersPresenceDone, this, &Connection::usersPresenceDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start usersPresenceJob";
    }
}

void Connection::customUserStatus()
{
    auto job = new CustomUserStatusListJob(this);
    initializeRestApiJob(job);
    connect(job, &CustomUserStatusListJob::customUserStatusDone, this, &Connection::customUserStatusDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start CustomUserStatusJob";
    }
}

void Connection::usersAutocomplete(const UsersAutocompleteJob::UsersAutocompleterInfo &info)
{
    auto job = new UsersAutocompleteJob(this);
    job->setUsersCompleterInfo(info);
    initializeRestApiJob(job);
    connect(job, &UsersAutocompleteJob::usersAutocompleteDone, this, &Connection::usersAutocompleteDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start UsersAutocompleteJob";
    }
}

void Connection::findOrCreateInvite(const QString &roomId, int maxUses, int numberOfDays)
{
    auto job = new FindOrCreateInviteJob(this);
    job->setRoomId(roomId);
    job->setMaxUses(maxUses);
    job->setNumberOfDays(numberOfDays);
    initializeRestApiJob(job);
    connect(job, &FindOrCreateInviteJob::findOrCreateInviteDone, this, &Connection::findOrCreateInviteDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start findOrCreateInviteJob";
    }
}

void Connection::sendUserEmailCode(const QString &identifier)
{
    auto job = new User2FASendEmailCodeJob(this);
    job->setUsernameOrEmail(identifier);
    initializeRestApiJob(job);
    connect(job, &User2FASendEmailCodeJob::sendEmailCodeDone, this, &Connection::sendEmailCodeDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start User2FASendEmailCodeJob";
    }
}

void Connection::deleteOwnAccount(const QString &password)
{
    auto job = new DeleteOwnAccountJob(this);
    job->setPassword(password);
    initializeRestApiJob(job);
    connect(job, &DeleteOwnAccountJob::deleteOwnAccountDone, this, &Connection::deleteOwnAccountDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start DeleteOwnAccountJob";
    }
}

void Connection::registerNewUser(const RocketChatRestApi::RegisterUserJob::RegisterUserInfo &userInfo)
{
    auto job = new RegisterUserJob(this);
    job->setRegisterUserInfo(userInfo);
    initializeRestApiJob(job);
    connect(job, &RegisterUserJob::registerUserDone, this, &Connection::registerUserDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start RegisterUserJob";
    }
}

void Connection::enable2FaEmailJob(bool enable)
{
    if (enable) {
        auto job = new User2FAEnableEmailJob(this);
        initializeRestApiJob(job);
        connect(job, &User2FAEnableEmailJob::enableEmailDone, this, &Connection::enableEmailDone);
        if (!job->start()) {
            qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start User2FAEnableEmailJob";
        }
    } else {
        auto job = new User2FADisableEmailJob(this);
        initializeRestApiJob(job);
        connect(job, &User2FADisableEmailJob::disableEmailDone, this, &Connection::disableEmailDone);
        if (!job->start()) {
            qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start User2FADisableEmailJob";
        }
    }
}

void Connection::updateOwnBasicInfo(const RocketChatRestApi::UsersUpdateOwnBasicInfoJob::UpdateOwnBasicInfo &info)
{
    auto job = new UsersUpdateOwnBasicInfoJob(this);
    job->setUpdateOwnBasicInfo(info);
    initializeRestApiJob(job);
    connect(job, &UsersUpdateOwnBasicInfoJob::updateOwnBasicInfoDone, this, &Connection::updateOwnBasicInfoDone);
    // Clear all other tokens when password was changed
    connect(job, &UsersUpdateOwnBasicInfoJob::passwordChanged, this, &Connection::removeOtherTokens);

    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start UsersUpdateOwnBasicInfoJob";
    }
}

void Connection::removeOtherTokens()
{
    auto job = new RemoveOtherTokensJob(this);
    initializeRestApiJob(job);
    connect(job, &RemoveOtherTokensJob::removeOtherTokensDone, this, &Connection::removeOtherTokensDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start RemoveOtherTokensJob";
    }
}

void Connection::cleanChannelHistory(const RocketChatRestApi::RoomsCleanHistoryJob::CleanHistoryInfo &info)
{
    auto job = new RoomsCleanHistoryJob(this);
    job->setCleanHistoryInfo(info);
    initializeRestApiJob(job);
    connect(job, &RoomsCleanHistoryJob::cleanHistoryDone, this, &Connection::cleanHistoryDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start ChannelCleanHistoryJob";
    }
}

void Connection::getRoomsAdmin(const RocketChatRestApi::AdminRoomsJob::AdminRoomsJobInfo &info)
{
    auto job = new AdminRoomsJob(this);
    job->setRoomsAdminInfo(info);
    initializeRestApiJob(job);
    connect(job, &AdminRoomsJob::adminRoomsDone, this, &Connection::roomsAdminDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start RoomsAdminJob";
    }
}

void Connection::getChannelsCounter(const QString &roomId)
{
    auto job = new ChannelGetCountersJob(this);
    initializeRestApiJob(job);
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job->setChannelGroupInfo(info);
    connect(job, &ChannelGetCountersJob::channelGetCountersDone, this, &Connection::channelGetCountersDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start ChannelGetCountersJob";
    }
}

void Connection::setUserPreferences(const RocketChatRestApi::UsersSetPreferencesJob::UsersSetPreferencesInfo &info)
{
    auto job = new UsersSetPreferencesJob(this);
    job->setUsersSetPreferencesInfo(info);
    initializeRestApiJob(job);
    connect(job, &UsersSetPreferencesJob::usersSetPreferencesDone, this, &Connection::usersSetPreferencesDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start UsersSetPreferencesJob";
    }
}

void Connection::createCustomUserStatus(const CustomUserStatusCreateJob::StatusCreateInfo &statusCreateInfo)
{
    auto job = new CustomUserStatusCreateJob(this);
    job->setStatusCreateInfo(statusCreateInfo);
    initializeRestApiJob(job);
    connect(job, &CustomUserStatusCreateJob::createUserStatusDone, this, &Connection::createUserStatusDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start CustomUserStatusCreateJob";
    }
}

void Connection::deleteCustomUserStatus(const QString &customUserStatusId)
{
    auto job = new CustomUserStatusDeleteJob(this);
    job->setCustomUserStatusId(customUserStatusId);
    initializeRestApiJob(job);
    connect(job, &CustomUserStatusDeleteJob::userStatusDeletedDone, this, &Connection::userStatusDeletedDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start CustomUserStatusDeleteJob";
    }
}

void Connection::updateCustomUserStatus(const CustomUserStatusUpdateJob::StatusUpdateInfo &statusUpdateInfo)
{
    auto job = new CustomUserStatusUpdateJob(this);
    job->setStatusUpdateInfo(statusUpdateInfo);
    initializeRestApiJob(job);
    connect(job, &CustomUserStatusUpdateJob::customUserUpdateDone, this, &Connection::customUserUpdateDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start CustomUserStatusUpdateJob";
    }
}

void Connection::resetAvatar(const UserBaseJob::UserInfo &info)
{
    auto job = new ResetAvatarJob(this);
    job->setUserInfo(info);
    initializeRestApiJob(job);
    connect(job, &ResetAvatarJob::resetAvatarDone, this, &Connection::resetAvatarDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start ResetAvatarJob";
    }
}

void Connection::exportMessages(const RocketChatRestApi::RoomsExportJob::RoomsExportInfo &info)
{
    auto job = new RoomsExportJob(this);
    job->setRoomExportInfo(info);
    initializeRestApiJob(job);
    connect(job, &RoomsExportJob::roomExportDone, this, &Connection::roomExportDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start RoomsExportJob";
    }
}

void Connection::statistics(bool refresh)
{
    auto job = new StatisticsJob(this);
    job->setRefresh(refresh);
    initializeRestApiJob(job);
    connect(job, &StatisticsJob::statisticDone, this, &Connection::statisticDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start StatisticsJob";
    }
}

#include "moc_connection.cpp"
