/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "generalsettingswidget.h"

#include <KLocalizedString>

#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>

GeneralSettingsWidget::GeneralSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
    , mEnableFavoriteRooms(new QCheckBox(i18n("Enable Favorite Rooms"), this))
    , mSiteUrl(new QLineEdit(this))
    , mSiteName(new QLineEdit(this))
    , mUTF8UsernamesValidation(new QLineEdit(this))
    , mUTF8ChannelNamesValidation(new QLineEdit(this))
    , mUTF8NamesSlugify(new QCheckBox(i18n("UTF8 Names Slugify"), this))
    , mFirstChannelAfterLogin(new QLineEdit(this))
    , mDeepLinkUrl(new QLineEdit(this))
    , mCDNPrefix(new QLineEdit(this))
    , mUnreadCount(new QComboBox(this))
    , mUnreadCountDirectMessages(new QComboBox(this))
    , mEnableUpdateChecker(new QCheckBox(i18n("Enable the Update Checker"), this))
    , mDefaultTimeZone(new QComboBox(this))
    , mSendStatisticsRocketChat(new QCheckBox(i18n("Send Statistics to Rocket.Chat"), this))
    , mMaxRoomMembersDisablingMessageNotifications(new QSpinBox(this))
{
    mEnableFavoriteRooms->setObjectName(QStringLiteral("mEnableFavoriteRooms"));
    mMainLayout->addWidget(mEnableFavoriteRooms);
    connectCheckBox(mEnableFavoriteRooms, QStringLiteral("Favorite_Rooms"));

    mSiteUrl->setObjectName(QStringLiteral("mSiteUrl"));
    addLineEdit(i18n("Site URL"), mSiteUrl, QStringLiteral("Site_Url"));

    mSiteName->setObjectName(QStringLiteral("mSiteName"));
    addLineEdit(i18n("Site Name"), mSiteName, QStringLiteral("Site_Name"));

    mUTF8UsernamesValidation->setObjectName(QStringLiteral("mUTF8UsernamesValidation"));
    mUTF8UsernamesValidation->setToolTip(i18n("RegExp that will be used to validate usernames"));
    addLineEdit(i18n("UTF8 Usernames Validation"), mUTF8UsernamesValidation, QStringLiteral("UTF8_User_Names_Validation"));

    mUTF8ChannelNamesValidation->setObjectName(QStringLiteral("mUTF8ChannelNamesValidation"));
    mUTF8ChannelNamesValidation->setToolTip(i18n("RegExp that will be used to validate channel names"));
    addLineEdit(i18n("UTF8 Channel Names Validation"), mUTF8ChannelNamesValidation, QStringLiteral("UTF8_Channel_Names_Validation"));

    mUTF8NamesSlugify->setObjectName(QStringLiteral("mUTF8NamesSlugify"));
    mMainLayout->addWidget(mUTF8NamesSlugify);
    connectCheckBox(mUTF8NamesSlugify, QStringLiteral("UTF8_Names_Slugify"));

    mFirstChannelAfterLogin->setObjectName(QStringLiteral("mFirstChannelAfterLogin"));
    addLineEdit(i18n("First Channel After Login"), mFirstChannelAfterLogin, QStringLiteral("First_Channel_After_Login"));

    mDeepLinkUrl->setObjectName(QStringLiteral("mDeepLinkUrl"));
    addLineEdit(i18n("Deep Link Url"), mDeepLinkUrl, QStringLiteral("DeepLink_Url"));

    mCDNPrefix->setObjectName(QStringLiteral("mCDNPrefix"));
    addLineEdit(i18n("CDN Prefix"), mCDNPrefix, QStringLiteral("CDN_PREFIX"));

    mUnreadCount->setObjectName(QStringLiteral("mUnreadCount"));
    QMap<QString, QString> maps;
    maps = {
        {QStringLiteral("all_messages"), i18n("All messages")},
        {QStringLiteral("user_mentions_only"), i18n("User mentions only")},
        {QStringLiteral("group_mentions_only"), i18n("Group mentions only")},
        {QStringLiteral("user_and_group_mentions_only"), i18n("User and group mentions only")},
    };
    addComboBox(i18n("Unread Count"), maps, mUnreadCount, QStringLiteral("Unread_Count"));

    mUnreadCountDirectMessages->setObjectName(QStringLiteral("mUnreadCountDirectMessages"));
    maps = {
        {QStringLiteral("all_messages"), i18n("All messages")},
        {QStringLiteral("mentions_only"), i18n("Mentions only")},
    };
    addComboBox(i18n("Unread Count for Direct Messages"), maps, mUnreadCountDirectMessages, QStringLiteral("Unread_Count_DM"));

    auto updateLabel = createBoldLabel(i18n("Update"));
    updateLabel->setObjectName(QStringLiteral("updateLabel"));
    mMainLayout->addWidget(updateLabel);

    mEnableUpdateChecker->setObjectName(QStringLiteral("mEnableUpdateChecker"));
    mMainLayout->addWidget(mEnableUpdateChecker);
    connectCheckBox(mEnableUpdateChecker, QStringLiteral("Update_EnableChecker"));
    mEnableUpdateChecker->setToolTip(
        i18n("Checks automatically for new updates / important messages from the Rocket.Chat developers and receives notifications when available.\n"
             "The notification appears once per new version as a clickable banner and as a message from the Rocket.Cat bot,\n"
             "both visible only for administrators."));

    auto timeZoneLabel = createBoldLabel(i18n("TimeZone"));
    timeZoneLabel->setObjectName(QStringLiteral("timeZoneLabel"));
    mMainLayout->addWidget(timeZoneLabel);

    mDefaultTimeZone->setObjectName(QStringLiteral("mDefaultTimeZone"));
    mDefaultTimeZone->setToolTip(i18n("Sets the default timezone that will be used when showing dashboards or sending emails"));
    maps = {
        {QStringLiteral("server"), i18n("Server Timezone")},
        {QStringLiteral("custom"), i18n("Custom Timezone")},
        {QStringLiteral("user"), i18n("User's current Timezone")},
    };
    addComboBox(i18n("Default timezone for reporting"), maps, mDefaultTimeZone, QStringLiteral("Default_Timezone_For_Reporting"));
    // TODO add custom time zone

    auto reportingLabel = createBoldLabel(i18n("Reporting"));
    reportingLabel->setObjectName(QStringLiteral("reportingLabel"));
    mMainLayout->addWidget(reportingLabel);

    mSendStatisticsRocketChat->setObjectName(QStringLiteral("mSendStatisticsRocketChat"));
    mMainLayout->addWidget(mSendStatisticsRocketChat);
    connectCheckBox(mSendStatisticsRocketChat, QStringLiteral("Statistics_reporting"));

    auto notificationLabel = createBoldLabel(i18n("Notifications"));
    notificationLabel->setObjectName(QStringLiteral("notificationLabel"));
    mMainLayout->addWidget(notificationLabel);

    mMaxRoomMembersDisablingMessageNotifications->setObjectName(QStringLiteral("mMaxRoomMembersDisablingMessageNotifications"));
    mMaxRoomMembersDisablingMessageNotifications->setToolTip(
        i18n("Max number of members in room when notifications for all messages gets disabled. Users can still change per room setting to receive all "
             "notifications on an individual basis. (0 to disable)"));
    mMaxRoomMembersDisablingMessageNotifications->setMaximum(999);
    addSpinbox(i18n("Max Room Members Before Disabling All Message Notifications"),
               mMaxRoomMembersDisablingMessageNotifications,
               QStringLiteral("Notifications_Max_Room_Members"));
}

GeneralSettingsWidget::~GeneralSettingsWidget() = default;

void GeneralSettingsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    initializeWidget(mEnableFavoriteRooms, mapSettings, true);
    initializeWidget(mSiteUrl, mapSettings, QString());
    initializeWidget(mSiteName, mapSettings, QString());
    initializeWidget(mUTF8UsernamesValidation, mapSettings, QStringLiteral("[0-9a-zA-Z-_.]+"));
    initializeWidget(mUTF8ChannelNamesValidation, mapSettings, QStringLiteral("[0-9a-zA-Z-_.]+"));
    initializeWidget(mFirstChannelAfterLogin, mapSettings);
    initializeWidget(mDeepLinkUrl, mapSettings, QStringLiteral("https://go.rocket.chat"));
    initializeWidget(mCDNPrefix, mapSettings, QString());
    initializeWidget(mUnreadCount, mapSettings, QStringLiteral("user_and_group_mentions_only"));
    initializeWidget(mUnreadCountDirectMessages, mapSettings, QStringLiteral("all_messages"));
    initializeWidget(mEnableUpdateChecker, mapSettings, true);
    initializeWidget(mDefaultTimeZone, mapSettings, QStringLiteral("server"));
    initializeWidget(mUTF8NamesSlugify, mapSettings, true);
    initializeWidget(mSendStatisticsRocketChat, mapSettings, true);
    initializeWidget(mMaxRoomMembersDisablingMessageNotifications, mapSettings, 100);
}
