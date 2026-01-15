/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "generalsettingswidget.h"
#include "rocketchataccount.h"
using namespace Qt::Literals::StringLiterals;

#include <KLocalizedString>

#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>

GeneralSettingsWidget::GeneralSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
    , mEnableFavoriteRooms(new QCheckBox(i18nc("@option:check", "Enable Favorite Rooms"), this))
    , mSiteUrl(new QLineEdit(this))
    , mSiteName(new QLineEdit(this))
    , mUTF8UsernamesValidation(new QLineEdit(this))
    , mUTF8ChannelNamesValidation(new QLineEdit(this))
    , mUTF8NamesSlugify(new QCheckBox(i18nc("@option:check", "UTF8 Names Slugify"), this))
    , mFirstChannelAfterLogin(new QLineEdit(this))
    , mDeepLinkUrl(new QLineEdit(this))
    , mCDNPrefix(new QLineEdit(this))
    , mUnreadCount(new QComboBox(this))
    , mUnreadCountDirectMessages(new QComboBox(this))
    , mUpdateLatestAvailableVersion(new QLineEdit(this))
    , mEnableUpdateChecker(new QCheckBox(i18nc("@option:check", "Enable the Update Checker"), this))
    , mDefaultTimeZone(new QComboBox(this))
    , mSendStatisticsRocketChat(new QCheckBox(i18nc("@option:check", "Send Statistics to Rocket.Chat"), this))
    , mMaxRoomMembersDisablingMessageNotifications(new QSpinBox(this))
    , mStreamCastAddress(new QLineEdit(this))
    , mRestApiUpperCountLimit(new QSpinBox(this))
    , mUseRestForDDPCalls(new QCheckBox(i18nc("@option:check", "Use REST instead of websocket for Meteor calls"), this))
    , mApiUserLimit(new QSpinBox(this))
{
    mEnableFavoriteRooms->setObjectName(u"mEnableFavoriteRooms"_s);
    addCheckBox(mEnableFavoriteRooms, u"Favorite_Rooms"_s);

    mSiteUrl->setObjectName(u"mSiteUrl"_s);
    addLineEdit(i18n("Site URL"), mSiteUrl, u"Site_Url"_s);

    mSiteName->setObjectName(u"mSiteName"_s);
    addLineEdit(i18n("Site Name"), mSiteName, u"Site_Name"_s);

    auto utf8Label = createBoldLabel(i18n("UTF8"));
    utf8Label->setObjectName(u"utf8Label"_s);
    mMainLayout->addWidget(utf8Label);

    mUTF8UsernamesValidation->setObjectName(u"mUTF8UsernamesValidation"_s);
    mUTF8UsernamesValidation->setToolTip(i18nc("@info:tooltip", "RegExp that will be used to validate usernames"));
    addLineEdit(i18n("UTF8 Usernames Validation"), mUTF8UsernamesValidation, u"UTF8_User_Names_Validation"_s);

    mUTF8ChannelNamesValidation->setObjectName(u"mUTF8ChannelNamesValidation"_s);
    mUTF8ChannelNamesValidation->setToolTip(i18nc("@info:tooltip", "RegExp that will be used to validate channel names"));
    addLineEdit(i18n("UTF8 Channel Names Validation"), mUTF8ChannelNamesValidation, u"UTF8_Channel_Names_Validation"_s);

    mUTF8NamesSlugify->setObjectName(u"mUTF8NamesSlugify"_s);
    addCheckBox(mUTF8NamesSlugify, u"UTF8_Names_Slugify"_s);

    mFirstChannelAfterLogin->setObjectName(u"mFirstChannelAfterLogin"_s);
    addLineEdit(i18n("First Channel After Login"), mFirstChannelAfterLogin, u"First_Channel_After_Login"_s);

    mDeepLinkUrl->setObjectName(u"mDeepLinkUrl"_s);
    addLineEdit(i18n("Deep Link Url"), mDeepLinkUrl, u"DeepLink_Url"_s);

    mCDNPrefix->setObjectName(u"mCDNPrefix"_s);
    addLineEdit(i18n("CDN Prefix"), mCDNPrefix, u"CDN_PREFIX"_s);

    mUnreadCount->setObjectName(u"mUnreadCount"_s);
    QMap<QString, QString> maps;
    maps = {
        {u"all_messages"_s, i18n("All messages")},
        {u"user_mentions_only"_s, i18n("User mentions only")},
        {u"group_mentions_only"_s, i18n("Group mentions only")},
        {u"user_and_group_mentions_only"_s, i18n("User and group mentions only")},
    };
    addComboBox(i18n("Unread Count"), maps, mUnreadCount, u"Unread_Count"_s);

    mUnreadCountDirectMessages->setObjectName(u"mUnreadCountDirectMessages"_s);
    maps = {
        {u"all_messages"_s, i18n("All messages")},
        {u"mentions_only"_s, i18n("Mentions only")},
    };
    addComboBox(i18n("Unread Count for Direct Messages"), maps, mUnreadCountDirectMessages, u"Unread_Count_DM"_s);

    auto updateLabel = createBoldLabel(i18n("Update"));
    updateLabel->setObjectName(u"updateLabel"_s);
    mMainLayout->addWidget(updateLabel);

    mUpdateLatestAvailableVersion->setObjectName(u"mUpdateLatestAvailableVersion"_s);
    addLineEdit(i18n("Update Latest Available Version"), mUpdateLatestAvailableVersion, u"Update_LatestAvailableVersion"_s);

    mEnableUpdateChecker->setObjectName(u"mEnableUpdateChecker"_s);
    addCheckBox(mEnableUpdateChecker, u"Update_EnableChecker"_s);
    mEnableUpdateChecker->setToolTip(
        i18n("Checks automatically for new updates / important messages from the Rocket.Chat developers and receives notifications when available.\n"
             "The notification appears once per new version as a clickable banner and as a message from the Rocket.Cat bot,\n"
             "both visible only for administrators."));

    auto timeZoneLabel = createBoldLabel(i18n("TimeZone"));
    timeZoneLabel->setObjectName(u"timeZoneLabel"_s);
    mMainLayout->addWidget(timeZoneLabel);

    mDefaultTimeZone->setObjectName(u"mDefaultTimeZone"_s);
    mDefaultTimeZone->setToolTip(i18nc("@info:tooltip", "Sets the default timezone that will be used when showing dashboards or sending emails"));
    maps = {
        {u"server"_s, i18n("Server Timezone")},
        {u"custom"_s, i18n("Custom Timezone")},
        {u"user"_s, i18n("User's current Timezone")},
    };
    addComboBox(i18n("Default timezone for reporting"), maps, mDefaultTimeZone, u"Default_Timezone_For_Reporting"_s);
    // TODO add custom time zone

    auto reportingLabel = createBoldLabel(i18n("Reporting"));
    reportingLabel->setObjectName(u"reportingLabel"_s);
    mMainLayout->addWidget(reportingLabel);

    mSendStatisticsRocketChat->setObjectName(u"mSendStatisticsRocketChat"_s);
    addCheckBox(mSendStatisticsRocketChat, u"Statistics_reporting"_s);

    auto notificationLabel = createBoldLabel(i18n("Notifications"));
    notificationLabel->setObjectName(u"notificationLabel"_s);
    mMainLayout->addWidget(notificationLabel);

    mMaxRoomMembersDisablingMessageNotifications->setObjectName(u"mMaxRoomMembersDisablingMessageNotifications"_s);
    mMaxRoomMembersDisablingMessageNotifications->setToolTip(
        i18n("Max number of members in room when notifications for all messages gets disabled. Users can still change per room setting to receive all "
             "notifications on an individual basis. (0 to disable)"));
    mMaxRoomMembersDisablingMessageNotifications->setMaximum(999);
    addSpinbox(i18n("Max Room Members Before Disabling All Message Notifications"),
               mMaxRoomMembersDisablingMessageNotifications,
               u"Notifications_Max_Room_Members"_s);

    auto streamCastLabel = createBoldLabel(i18n("Stream Cast"));
    streamCastLabel->setObjectName(u"streamCastLabel"_s);
    mMainLayout->addWidget(streamCastLabel);

    mStreamCastAddress->setObjectName(u"mStreamCastAddress"_s);
    addLineEdit(i18n("Stream Cast Address"), mStreamCastAddress, u"Stream_Cast_Address"_s);
    mStreamCastAddress->setToolTip(i18nc("@info:tooltip", "IP or Host of your Rocket.Chat central Stream Cast. E.g. 192.168.1.1:3000 or localhost:4000"));

    auto restApiLabel = createBoldLabel(i18n("REST API"));
    restApiLabel->setObjectName(u"restApiLabel"_s);
    mMainLayout->addWidget(restApiLabel);

    mRestApiUpperCountLimit->setObjectName(u"mRestApiUpperCountLimit"_s);
    mRestApiUpperCountLimit->setMaximum(999);
    addSpinbox(i18n("What is the maximum number of records the REST API should return (when not unlimited)?"),
               mRestApiUpperCountLimit,
               u"API_Upper_Count_Limit"_s);

    mUseRestForDDPCalls->setObjectName(u"mUseRestForDDPCalls"_s);
    if (mAccount) {
        if (!mAccount->hasAtLeastVersion(8, 0, 0)) {
            addCheckBox(mUseRestForDDPCalls, u"API_Use_REST_For_DDP_Calls"_s);
        } else {
            mUseRestForDDPCalls->hide();
        }
    }

    mApiUserLimit->setObjectName(u"mApiUserLimit"_s);
    mApiUserLimit->setMaximum(999);
    addSpinbox(i18n("User Limit for Adding All Users to Channel"), mApiUserLimit, u"API_User_Limit"_s);
}

GeneralSettingsWidget::~GeneralSettingsWidget() = default;

void GeneralSettingsWidget::initialize(const QMap<QString, SettingsWidgetBase::SettingsInfo> &mapSettings)
{
    initializeWidget(mEnableFavoriteRooms, mapSettings, true);
    initializeWidget(mSiteUrl, mapSettings, QString());
    initializeWidget(mSiteName, mapSettings, QString());
    initializeWidget(mUTF8UsernamesValidation, mapSettings, u"[0-9a-zA-Z-_.]+"_s);
    initializeWidget(mUTF8ChannelNamesValidation, mapSettings, u"[0-9a-zA-Z-_.]+"_s);
    initializeWidget(mFirstChannelAfterLogin, mapSettings);
    initializeWidget(mDeepLinkUrl, mapSettings, u"https://go.rocket.chat"_s);
    initializeWidget(mCDNPrefix, mapSettings, QString());
    initializeWidget(mUnreadCount, mapSettings, u"user_and_group_mentions_only"_s);
    initializeWidget(mUnreadCountDirectMessages, mapSettings, u"all_messages"_s);
    initializeWidget(mEnableUpdateChecker, mapSettings, true);
    initializeWidget(mDefaultTimeZone, mapSettings, u"server"_s);
    initializeWidget(mUTF8NamesSlugify, mapSettings, true);
    initializeWidget(mSendStatisticsRocketChat, mapSettings, true);
    initializeWidget(mMaxRoomMembersDisablingMessageNotifications, mapSettings, 100);
    initializeWidget(mUpdateLatestAvailableVersion, mapSettings, u"0.0.0"_s);
    initializeWidget(mStreamCastAddress, mapSettings, QString());
    initializeWidget(mRestApiUpperCountLimit, mapSettings, 100);
    if (mAccount && !mAccount->hasAtLeastVersion(8, 0, 0)) {
        initializeWidget(mUseRestForDDPCalls, mapSettings, true);
    }
    initializeWidget(mApiUserLimit, mapSettings, 500);
}

#include "moc_generalsettingswidget.cpp"
