/*
   Copyright (c) 2017-2020 Laurent Montel <montel@kde.org>

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

#include "ruqolaserverconfig.h"
#include <QStringList>
#include "ruqola_debug.h"

RuqolaServerConfig::RuqolaServerConfig()
{
}

QString RuqolaServerConfig::uniqueId() const
{
    return mUniqueId;
}

void RuqolaServerConfig::setUniqueId(const QString &uniqueId)
{
    mUniqueId = uniqueId;
}

QString RuqolaServerConfig::jitsiMeetUrl() const
{
    return mJitsiMeetUrl;
}

void RuqolaServerConfig::setJitsiMeetUrl(const QString &jitsiMeetUrl)
{
    mJitsiMeetUrl = jitsiMeetUrl;
}

QString RuqolaServerConfig::jitsiMeetPrefix() const
{
    return mJitsiMeetPrefix;
}

void RuqolaServerConfig::setJitsiMeetPrefix(const QString &jitsiMeetPrefix)
{
    mJitsiMeetPrefix = jitsiMeetPrefix;
}

QString RuqolaServerConfig::fileUploadStorageType() const
{
    return mFileUploadStorageType;
}

void RuqolaServerConfig::setFileUploadStorageType(const QString &type)
{
    mFileUploadStorageType = type;
}

void RuqolaServerConfig::setAllowMessageEditing(bool state)
{
    mAllowEditingMessage = state;
}

bool RuqolaServerConfig::allowMessageEditing() const
{
    return mAllowEditingMessage;
}

void RuqolaServerConfig::setBlockDeletingMessageInMinutes(int minutes)
{
    mBlockDeletingMessageInMinutes = minutes;
}

void RuqolaServerConfig::setBlockEditingMessageInMinutes(int minutes)
{
    mBlockEditingMessageInMinutes = minutes;
}

int RuqolaServerConfig::blockEditingMessageInMinutes() const
{
    return mBlockEditingMessageInMinutes;
}

bool RuqolaServerConfig::otrEnabled() const
{
    return mOtrEnabled;
}

void RuqolaServerConfig::setOtrEnabled(bool otrEnabled)
{
    mOtrEnabled = otrEnabled;
}

bool RuqolaServerConfig::needAdaptNewSubscriptionRC60() const
{
    return mNeedAdaptNewSubscriptionRC60;
}

bool RuqolaServerConfig::hasAtLeastVersion(int major, int minor, int patch) const
{
//    qDebug() << " major " << major << " mServerVersionMajor " << mServerVersionMajor << " (major <= mServerVersionMajor) " << (major <= mServerVersionMajor) <<
//                " minor " << minor << " mServerVersionMinor  " << mServerVersionMinor << " (minor <= mServerVersionMinor) " << (minor <= mServerVersionMinor) <<
//                " patch " << patch << " mServerVersionPatch " << mServerVersionPatch << " (patch <= mServerVersionPatch) " << (patch <= mServerVersionPatch);
    if (mServerVersionMajor > major) {
        return true;
    }
    return (major <= mServerVersionMajor) && (minor <= mServerVersionMinor) && (patch <= mServerVersionPatch);
}

void RuqolaServerConfig::setServerVersion(const QString &version)
{
    mServerVersionStr = version;
    const QStringList lst = version.split(QLatin1Char('.'));
    //0.70.0-rc.1 has 4 "."
    if (lst.count() >= 3) {
        bool ok;
        int value = lst.at(0).toInt(&ok);
        if (ok) {
            mServerVersionMajor = value;
        }
        value = lst.at(1).toInt(&ok);
        if (ok) {
            mServerVersionMinor = value;
        }
        value = lst.at(2).toInt(&ok);
        if (ok) {
            mServerVersionPatch = value;
        } else { //Perhaps it has "rc"/"beta" etc.
            mServerVersionPatch = 0;
        }
    }
    adaptToServerVersion();
}

bool RuqolaServerConfig::ruqolaHasSupportForOauthType(AuthenticationManager::OauthType type) const
{
    return mRuqolaOauthTypes & type;
}

bool RuqolaServerConfig::canShowOauthService(AuthenticationManager::OauthType type) const
{
    return serverHasSupportForOauthType(type) && ruqolaHasSupportForOauthType(type);
}

void RuqolaServerConfig::addRuqolaAuthenticationSupport(AuthenticationManager::OauthType type)
{
    mRuqolaOauthTypes |= type;
}

bool RuqolaServerConfig::serverHasSupportForOauthType(AuthenticationManager::OauthType type) const
{
    return mServerOauthTypes & type;
}

void RuqolaServerConfig::addOauthService(const QString &service)
{
    const QString serviceLower = service.toLower();
    if (serviceLower.endsWith(QLatin1String("twitter"))) {
        mServerOauthTypes |= AuthenticationManager::OauthType::Twitter;
    } else if (serviceLower.endsWith(QLatin1String("facebook"))) {
        mServerOauthTypes |= AuthenticationManager::OauthType::FaceBook;
    } else if (serviceLower.endsWith(QLatin1String("github"))) {
        mServerOauthTypes |= AuthenticationManager::OauthType::GitHub;
    } else if (serviceLower.endsWith(QLatin1String("gitlab"))) {
        mServerOauthTypes |= AuthenticationManager::OauthType::GitLab;
    } else if (serviceLower.endsWith(QLatin1String("google"))) {
        mServerOauthTypes |= AuthenticationManager::OauthType::Google;
    } else if (serviceLower.endsWith(QLatin1String("linkedin"))) {
        mServerOauthTypes |= AuthenticationManager::OauthType::Linkedin;
    } else if (serviceLower.endsWith(QLatin1String("wordpress"))) {
        mServerOauthTypes |= AuthenticationManager::OauthType::Wordpress;
    } else if (serviceLower.endsWith(QLatin1String("_oauth_proxy_host"))) {
        //Hide warning as it's not a service
        qCDebug(RUQOLA_LOG) << "_OAuth_Proxy_host found ";
    } else if (serviceLower.endsWith(QLatin1String("_oauth_meteor"))) {
        //Hide warning as it's not a service
        qCDebug(RUQOLA_LOG) << "Accounts_OAuth_Meteor found ";
    } else {
        qCWarning(RUQOLA_LOG) << "Unknown service type: " << service;
    }
}

void RuqolaServerConfig::adaptToServerVersion()
{
    mNeedAdaptNewSubscriptionRC60 = (mServerVersionMajor >= 1) || ((mServerVersionMajor == 0) && (mServerVersionMinor >= 60));
}

int RuqolaServerConfig::blockDeletingMessageInMinutes() const
{
    return mBlockDeletingMessageInMinutes;
}

QString RuqolaServerConfig::autoTranslateGoogleKey() const
{
    return mAutoTranslateGoogleKey;
}

void RuqolaServerConfig::setAutoTranslateGoogleKey(const QString &autoTranslateGoogleKey)
{
    mAutoTranslateGoogleKey = autoTranslateGoogleKey;
}

bool RuqolaServerConfig::autoTranslateEnabled() const
{
    return mAutoTranslateEnabled;
}

void RuqolaServerConfig::setAutoTranslateEnabled(bool autoTranslateEnabled)
{
    mAutoTranslateEnabled = autoTranslateEnabled;
}

bool RuqolaServerConfig::discussionEnabled() const
{
    return mDiscussionEnabled;
}

void RuqolaServerConfig::setDiscussionEnabled(bool discussionEnabled)
{
    mDiscussionEnabled = discussionEnabled;
}

bool RuqolaServerConfig::threadsEnabled() const
{
    return mThreadsEnabled;
}

void RuqolaServerConfig::setThreadsEnabled(bool threadsEnabled)
{
    mThreadsEnabled = threadsEnabled;
}

bool RuqolaServerConfig::jitsiEnabled() const
{
    return mJitsiEnabled;
}

void RuqolaServerConfig::setJitsiEnabled(bool jitsiEnabled)
{
    mJitsiEnabled = jitsiEnabled;
}

bool RuqolaServerConfig::allowMessageDeleting() const
{
    return mAllowMessageDeleting;
}

void RuqolaServerConfig::setAllowMessageDeleting(bool allowMessageDeletingEnabled)
{
    mAllowMessageDeleting = allowMessageDeletingEnabled;
}

bool RuqolaServerConfig::allowMessageStarring() const
{
    return mAllowMessageStarring;
}

void RuqolaServerConfig::setAllowMessageStarring(bool allowMessageStarringEnabled)
{
    mAllowMessageStarring = allowMessageStarringEnabled;
}

bool RuqolaServerConfig::allowMessageSnippeting() const
{
    return mAllowMessageSnippeting;
}

void RuqolaServerConfig::setAllowMessageSnippeting(bool allowMessageSnippetingEnabled)
{
    mAllowMessageSnippeting = allowMessageSnippetingEnabled;
}

bool RuqolaServerConfig::allowMessagePinning() const
{
    return mAllowMessagePinning;
}

void RuqolaServerConfig::setAllowMessagePinning(bool allowMessagePinningEnabled)
{
    mAllowMessagePinning = allowMessagePinningEnabled;
}

int RuqolaServerConfig::serverVersionPatch() const
{
    return mServerVersionPatch;
}

int RuqolaServerConfig::serverVersionMinor() const
{
    return mServerVersionMinor;
}

int RuqolaServerConfig::serverVersionMajor() const
{
    return mServerVersionMajor;
}

QString RuqolaServerConfig::serverVersionStr() const
{
    return mServerVersionStr;
}

bool RuqolaServerConfig::encryptionEnabled() const
{
    return mEncryptionEnabled;
}

void RuqolaServerConfig::setEncryptionEnabled(bool encryptionEnabled)
{
    mEncryptionEnabled = encryptionEnabled;
}

QString RuqolaServerConfig::siteName() const
{
    return mSiteName;
}

void RuqolaServerConfig::setSiteName(const QString &siteName)
{
    mSiteName = siteName;
}

QString RuqolaServerConfig::siteUrl() const
{
    return mSiteUrl;
}

void RuqolaServerConfig::setSiteUrl(const QString &siteUrl)
{
    mSiteUrl = siteUrl;
}

AuthenticationManager::OauthTypes RuqolaServerConfig::ruqolaOauthTypes() const
{
    return mRuqolaOauthTypes;
}

AuthenticationManager::OauthTypes RuqolaServerConfig::serverOauthTypes() const
{
    return mServerOauthTypes;
}

QDebug operator <<(QDebug d, const RuqolaServerConfig &t)
{
    d << "mUniqueId  " << t.uniqueId();
    d << "mJitsiMeetUrl " << t.jitsiMeetUrl();
    d << "mJitsiMeetPrefix " << t.jitsiMeetPrefix();
    d << "mFileUploadStorageType " << t.fileUploadStorageType();
    d << "mSiteUrl " << t.siteUrl();
    d << "mSiteName " << t.siteName();
    d << "mServerOauthTypes " << t.serverOauthTypes();
    d << "mRuqolaOauthTypes " << t.ruqolaOauthTypes();
    d << "mBlockEditingMessageInMinutes " << t.blockEditingMessageInMinutes();
    d << "mAllowEditingMessage " << t.allowMessageEditing();
    d << "mOtrEnabled " << t.otrEnabled();
    d << "mNeedAdaptNewSubscriptionRC60 " << t.needAdaptNewSubscriptionRC60();
    d << "mEncryptionEnabled " << t.encryptionEnabled();
    d << "mServerVersionMajor " << t.serverVersionMajor() << " mServerVersionMinor " << t.serverVersionMinor() << " mServerVersionPatch " << t.serverVersionPatch();
    return d;
}
